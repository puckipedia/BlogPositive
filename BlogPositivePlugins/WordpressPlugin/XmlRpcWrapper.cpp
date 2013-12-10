/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "XmlRpcWrapper.h"

#include <List.h>
#include <String.h>

void
XmlValue::PushContent(BString string)
{
	string << "<value><" << boxType << ">";
	string << fValue;
	string << "</" << boxType << "></value>";
}


XmlValue::XmlValue(BString string, BString aBoxType = "string")
	:
	fValue(string),
	boxType(aBoxType)
{
}


XmlValue::XmlValue(int integer, BString aBoxType = "i4")
	:
	boxType(aBoxType)
{
	fValue << integer;
}


XmlValue::XmlValue(double doubleVal, BString aBoxType = "string")
	:
	boxType(aBoxType)
{
	fValue << doubleVal;
}


void
XmlArray::PushContent(BString string)
{
	string << "<array><data>";
	for (int i = 0; i < Items(); i++)
	{
		ItemAt(i)->PushContent(string);
	}
	string << "</data></array>";
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


void
XmlNameValuePair::PushContent(BString string)
{
	string << "<value><member>";
	string << "<name>" << fName << "</name>";
	fValue->PushContent(string);
	string << "</member></value>";
}


void
XmlStruct::PushContent(BString string)
{
	string << "<struct>";
	for (int32 i = 0; i < Items(); i++)
	{
		ItemAt(i)->PushContent(string);
	}
	string << "</struct>";
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
		ItemAt(i)->PushContent(resultString);
		resultString << "</param>";
	}
	resultString << "</params>";
	resultString << "</methodCall";
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
