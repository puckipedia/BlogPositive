/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "XmlRpcWrapper.h"

#include <List.h>
#include <String.h>

#include "xmlnode.h"

void
XmlValue::PushContent(BString* string)
{
	*string << "<value><" << boxType << ">";
	BString value(fValue);
	value.ReplaceAll("&", "&amp;");
	value.ReplaceAll("\"", "&quot;");
	value.ReplaceAll("'", "&apos;");
	value.ReplaceAll("<", "&lt;");
	value.ReplaceAll(">", "&gt;");
	*string << value;
	*string << "</" << boxType << "></value>";
}


XmlValue::XmlValue(BString string, BString aBoxType)
	:
	fValue(string),
	boxType(aBoxType)
{
}


XmlValue::XmlValue(int integer, BString aBoxType)
	:
	boxType(aBoxType)
{
	fValue << integer;
}


XmlValue::XmlValue(double doubleVal, BString aBoxType)
	:
	boxType(aBoxType)
{
	fValue << doubleVal;
}


BString
XmlValue::Value()
{
	return fValue;
}


void
XmlArray::PushContent(BString* string)
{
	*string << "<array><data>";
	for (int i = 0; i < Items(); i++) {
		ItemAt(i)->PushContent(string);
	}
	*string << "</data></array>";
}


XmlArray::XmlArray()
{
	fList = new BObjectList<BaseXmlValue>();
}


void
XmlArray::AddItem(BaseXmlValue* xmlValue)
{
	fList->AddItem(xmlValue);
}


int32
XmlArray::Items()
{
	return fList->CountItems();
}


BaseXmlValue*
XmlArray::ItemAt(int32 itemLocation)
{
	return fList->ItemAt(itemLocation);
}


XmlNameValuePair::XmlNameValuePair(BString name, BaseXmlValue* value)
	:
	fName(name),
	fValue(value)
{
}


XmlNameValuePair::XmlNameValuePair(BString name, BString value)
	:
	fName(name),
	fValue(new XmlValue(value))
{
}


BString
XmlNameValuePair::Name()
{
	return fName;
}


BaseXmlValue*
XmlNameValuePair::Value()
{
	return fValue;
}


void
XmlNameValuePair::PushContent(BString* string)
{
	*string << "<member>";
	*string << "<name>" << fName << "</name>";
	fValue->PushContent(string);
	*string << "</member>";
}


void
XmlStruct::PushContent(BString* string)
{
	*string << "<struct>";
	for (int32 i = 0; i < Items(); i++) {
		ItemAt(i)->PushContent(string);
	}
	*string << "</struct>";
}


BaseXmlValue*
XmlStruct::Get(BString name)
{
	for (int i = 0; i < fList->CountItems(); i++) {
		if (fList->ItemAt(i)->Name() == name)
			return fList->ItemAt(i)->Value();
	}
	return NULL;
}


XmlStruct::XmlStruct()
{
	fList = new BObjectList<XmlNameValuePair>();
}


void
XmlStruct::AddItem(XmlNameValuePair* pair)
{
	fList->AddItem(pair);
}


void
XmlStruct::AddItem(BString name, BString value)
{
	AddItem(new XmlNameValuePair(name, new XmlValue(value)));
}


int32
XmlStruct::Items()
{
	return fList->CountItems();
}


XmlNameValuePair*
XmlStruct::ItemAt(int32 itemIndex)
{
	return fList->ItemAt(itemIndex);
}


XmlRpcRequest::XmlRpcRequest()
{
	fXmlValues = new BObjectList<BaseXmlValue>();
}


void
XmlRpcRequest::AddItem(BaseXmlValue* xmlValue)
{
	fXmlValues->AddItem(xmlValue);
}


BString
XmlRpcRequest::GetData()
{
	BString resultString;
	resultString << "<?xml version=\"1.0\"?>\n";
	resultString << "<methodCall>";
	resultString << "<methodName>" << fMethodName << "</methodName>";
	resultString << "<params>";
	for (int i = 0; i < Items(); i++) {
		resultString << "<param>";
		ItemAt(i)->PushContent(&resultString);
		resultString << "</param>";
	}
	resultString << "</params>";
	resultString << "</methodCall>";
	return resultString;
}


const char*
XmlRpcRequest::MethodName()
{
	return fMethodName.String();
}


void
XmlRpcRequest::SetMethodName(const char* name)
{
	fMethodName = name;
}


int32
XmlRpcRequest::Items()
{
	return fXmlValues->CountItems();
}


BaseXmlValue*
XmlRpcRequest::ItemAt(int32 itemIndex)
{
	return fXmlValues->ItemAt(itemIndex);
}


XmlStruct* ParseStruct(XmlNode* baseNode);
XmlArray* ParseArray(XmlNode* baseNode);
BaseXmlValue*
ParseValue(XmlNode* baseNode)
{
	if(baseNode->Children() == 0)
	{
		return new XmlValue(*baseNode->Value());
	} else {
		BString name = baseNode->ItemAt(0)->Name();
		if (name == "struct")
			return ParseStruct(baseNode->ItemAt(0));
		if (name == "array")
			return ParseArray(baseNode->ItemAt(0));
		BString value = baseNode->ItemAt(0)->Value();
		return new XmlValue(value, name);
	}
}


XmlArray*
ParseArray(XmlNode* baseNode)
{
	XmlArray* array = new XmlArray();
	XmlNode* dataNode = baseNode->ItemAt(0);
	
	for(int i = 0; i < dataNode->Children(); i++)
	{
		array->AddItem(ParseValue(dataNode->ItemAt(i)));
	}
	
	return array;
}


XmlStruct*
ParseStruct(XmlNode* baseNode)
{
	XmlStruct* xmlStruct = new XmlStruct;
	XmlNode* memberNode = NULL;

	while ((memberNode = baseNode->FindChild("member", memberNode)) != NULL) {
		BString name = memberNode->FindChild("name")->Value();
		BaseXmlValue* value = ParseValue(memberNode->FindChild("value"));
		xmlStruct->AddItem(new XmlNameValuePair(name, value));
	}

	return xmlStruct;
}


XmlArray*
ParseResponse(XmlNode* baseNode)
{
	XmlArray* responseArray = new XmlArray;
	XmlNode* paramsNode = baseNode->FindChild("params", NULL, true);

	for (int i = 0; i < paramsNode->Children(); i++) {
		responseArray->AddItem(ParseValue(paramsNode->ItemAt(i)->ItemAt(0)));
	}

	return responseArray;
}
