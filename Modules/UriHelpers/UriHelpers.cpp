/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "UriHelpers.h"

uint8
_HexDigitToInt(char digit)
{
	if (digit <= '9')
		return digit - '0';
	if (digit <= 'Z')
		return digit + 10 - 'A';
	if (digit <= 'z')
		return digit + 10 - 'a';
}


class QueryParameterPart {
public:
	QueryParameterPart();
	QueryParameterPart(BString name, BString value);

	BString	Name();
	BString	Value();
	void	SetName(BString name);
	void	SetValue(BString value);
private:
	BString	fName;
	BString	fValue;
};


QueryParameterPart::QueryParameterPart()
{	
}


QueryParameterPart::QueryParameterPart(BString name, BString value)
	:
	fName(name),
	fValue(value)
{	
}


BString
QueryParameterPart::Name()
{
	return fName;
}


BString
QueryParameterPart::Value()
{
	return fValue;
}


void
QueryParameterPart::SetName(BString name)
{
	fName = name;
}


void
QueryParameterPart::SetValue(BString value)
{
	fValue = value;
}


QueryParameters
QueryParameters::ParseParameters(BString parameters)
{
	QueryParameters params;
	
	const char* paramString = parameters.String();
	
	int state = 0;
	
	BString name;
	BString value;
	
	while (*paramString != NULL) {

		if (*paramString == '%') {
			const char first = *(++paramString);
			if(first == NULL)
				return params;
			const char second = *(++paramString);
			if(second == NULL)
				return params;
			uint8 firstDigit = _HexDigitToInt(first);
			uint8 secondDigit = _HexDigitToInt(second);
			
			char c = firstDigit << 4 | secondDigit;
			
			switch (state) {
				case 0:
					name << c;
					break;
				case 1:
					name << c;
					break;
			}
			
			paramString++;
			continue;
		}
		
		if (*paramString == '&') {
			params.SetParameter(name, value);
			name = "";
			value = "";
			state = 0;
		}
		else if (*paramString == '=') {
			state = 1;
		}
		else {
			if (state == 0) {
				name << *paramString;
			}
			if (state == 1) {
				value << *paramString;
			}
		}
		
		paramString++;
	}
	
	params.SetParameter(name, value);
	return params;
}


QueryParameters::QueryParameters()
{
}


BString
QueryParameters::String()
{
	BString result;
	for (int i = 0; i < fParameters.CountItems(); i++) {
		QueryParameterPart* part = fParameters.ItemAt(i);
		result << _Escape(part->Name()) << "=" << _Escape(part->Value());
		
		if (i < fParameters.CountItems() - 2) {
			result << "&";
		}
	}
	return result;
}


BString
QueryParameters::GetParameter(BString name) {
	int32 index = _IndexItem(name);
	if (index < 0)
		return "";
	return fParameters.ItemAt(index)->Value();
}


void
QueryParameters::SetParameter(BString name, BString value) {
	int32 index = _IndexItem(name);
	QueryParameterPart* part = new QueryParameterPart(name, value);

	if (index < 0)
		fParameters.AddItem(part);

	QueryParameterPart* oldPart = fParameters.SwapWithItem(index, part);
	delete oldPart;
}


int32
QueryParameters::_IndexItem(BString name) {
	for (int i = 0; i < fParameters.CountItems(); i++) {
		if (fParameters.ItemAt(i)->Name() == name)
			return i;
	}
	return -1;
}


BString
QueryParameters::_Escape(BString string) {
	BString result = string;

	result.ReplaceAll(" ",  "%20");
	result.ReplaceAll("<",  "%3C");
	result.ReplaceAll(">",  "%3E");
	result.ReplaceAll("#",  "%23");
	result.ReplaceAll("%",  "%25");
	result.ReplaceAll("{",  "%7B");
	result.ReplaceAll("}",  "%7D");
	result.ReplaceAll("|",  "%7C");
	result.ReplaceAll("\\", "%5C");
	result.ReplaceAll("^",  "%5E");
	result.ReplaceAll("~",  "%7E");
	result.ReplaceAll("[",  "%5B");
	result.ReplaceAll("]",  "%5D");
	result.ReplaceAll("`",  "%60");
	result.ReplaceAll(";",  "%3B");
	result.ReplaceAll("/",  "%2F");
	result.ReplaceAll("?",  "%3F");
	result.ReplaceAll(":",  "%3A");
	result.ReplaceAll("@",  "%40");
	result.ReplaceAll("=",  "%3D");
	result.ReplaceAll("&",  "%26");
	result.ReplaceAll("$",  "%24");

	return result;
}


BString
QueryParameters::_Unescape(BString string) {
	BString result = string;

	result.ReplaceAll("%20", " ");
	result.ReplaceAll("%3C", "<");
	result.ReplaceAll("%3E", ">");
	result.ReplaceAll("%23", "#");
	result.ReplaceAll("%25", "%");
	result.ReplaceAll("%7B", "{");
	result.ReplaceAll("%7D", "}");
	result.ReplaceAll("%7C", "|");
	result.ReplaceAll("%5C", "\\");
	result.ReplaceAll("%5E", "^");
	result.ReplaceAll("%7E", "~");
	result.ReplaceAll("%5B", "[");
	result.ReplaceAll("%5D", "]");
	result.ReplaceAll("%60", "`");
	result.ReplaceAll("%3B", ";");
	result.ReplaceAll("%2F", "/");
	result.ReplaceAll("%3F", "?");
	result.ReplaceAll("%3A", ":");
	result.ReplaceAll("%40", "@");
	result.ReplaceAll("%3D", "=");
	result.ReplaceAll("%26", "&");
	result.ReplaceAll("%24", "$");

	return result;
}
