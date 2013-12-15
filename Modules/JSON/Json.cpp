#include <File.h>
#include <ObjectList.h>
#include <String.h>
#include <cstdlib>
#include <stdio.h>
#include <cctype>
#include "Json.h"

void
JsonValue::AddSelf(BString* str)
{
	
}

void
JsonString::AddString(const char* c)
{
	fString << c;
}


void
JsonString::AddChar(char c)
{
	fString << c;
}


void
JsonString::AddSelf(BString* str)
{
	*str << "\"";
	*str << fString;
	*str << "\"";
}

JsonPair::JsonPair(JsonString* n, JsonValue* v)
	:
	fName(n),
	fValue(v)
{
}


JsonObject::JsonObject()
	:
	fPairList(new BObjectList<JsonPair>())
{

}

void
JsonObject::AddPair(JsonString* str, JsonValue* val)
{
	fPairList->AddItem(new JsonPair(str, val));
}


void
JsonObject::AddSelf(BString* str)
{
	*str << "{";
	for (int i = 0; i < fPairList->CountItems(); i++) {
		JsonPair* pair = fPairList->ItemAt(i);
		pair->fName->AddSelf(str);
		*str << ": ";
		pair->fValue->AddSelf(str);
		if(i < fPairList->CountItems()-1) {
			*str << ", ";
		}
	}
	*str << "}";
}


JsonNumber::JsonNumber(double d)
	:
	fDouble(d)
{

}

void
JsonNumber::AddSelf(BString* str)
{
	*str << fDouble;
}


JsonArray::JsonArray()
{
	fObjectList = new BObjectList<JsonValue>();
}


void
JsonArray::AddValue(JsonValue* Value)
{
	fObjectList->AddItem(Value);
}

void
JsonArray::AddSelf(BString* str)
{
	*str << "[";
	for (int i = 0; i < fObjectList->CountItems(); i++) {
		JsonValue* val = fObjectList->ItemAt(i);
		val->AddSelf(str);
		if(i < fObjectList->CountItems()-1) {
			*str << ", ";
		}
	}
	*str << "]";
}




void
JsonBool::AddSelf(BString* str)
{
	*str << fVal ? "true" : "false";
}


void
JsonNull::AddSelf(BString* str)
{
	*str << "null";
}


void
JsonParser::TakeUntil(char end)
{
	while(Peek() != end)
		TakeOne();
}


JsonParser::JsonParser(BString string)
{
	fPointer = 0;
	fString = string;
	
	TakeWhitespace();
	char firstChar = Peek();
	
	JsonValue* thing;
	switch(firstChar) {
		case '{':
			thing = TakeObject();
			break;
		case '[':
			thing = TakeArray();
			break;
	}
	BString str;
	thing->AddSelf(&str);
}


JsonNumber*
JsonParser::TakeNumber() {
	BString numberPart;
	TakeWhitespace();
	if(Peek() == '-')
		numberPart << '-';
	while(isdigit(Peek()))
		numberPart << TakeOne();
	if(Peek() == '.')
		numberPart << '.';
	while(isdigit(Peek()))
		numberPart << TakeOne();
	if(Peek() == 'e')
		numberPart << 'e';
	while(isdigit(Peek()))
		numberPart << TakeOne();
	double i = strtod(numberPart.String(), NULL);
	return new JsonNumber(i);
}

void
JsonParser::TakeWhitespace() {
	while(Peek() != NULL && isspace(Peek()))
		TakeOne();
}


char
JsonParser::Peek() {
	if(fPointer > fString.Length())
		return NULL;
	return fString[fPointer];
}

char
JsonParser::TakeOne() {
	if(fPointer > fString.Length())
		return NULL;
	return fString[fPointer++];
}

enum States {
	StringTakeChar,
	StringTakeSpecial
};

JsonString*
JsonParser::TakeString()
{
	if(Peek() == '"')
		TakeOne();
	JsonString* String = new JsonString();
	States state = StringTakeChar;
	
	char chr;
	while(chr = TakeOne()) {
		switch (state) {
			case StringTakeChar:
				switch (chr) {
					case '\\':
						state = StringTakeSpecial;
						break;
					case '"':
						return String;
						break;
					default:
						String->AddChar(chr);
						break;
				}
				break;
			case StringTakeSpecial:
				switch(chr) {
					case 'b':
						String->AddChar('\b');
						break;
					case 'f':
						String->AddChar('\f');
						break;
					case 'n':
						String->AddChar('\n');
						break;
					case 'r':
						String->AddChar('\r');
						break;
					case 't':
						String->AddChar('\t');
						break;
					case 'u':
						TakeOne();
						TakeOne();
						TakeOne();
						TakeOne();
						break;
					default:
						String->AddChar(chr);
				}
				break;
		}
	}
}

JsonValue*
JsonParser::TakeValue()
{
	TakeWhitespace();
	switch(Peek()) {
		case '"':
			return TakeString();
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-':
			return TakeNumber();
			break;
		case '[':
			return TakeArray();
			break;
		case '{':
			return TakeObject();
			break;
		case 't':
			TakeOne();
			TakeOne();
			TakeOne();
			TakeOne();
			return new JsonBool(true);
			break;
		case 'f':
			TakeOne();
			TakeOne();
			TakeOne();
			TakeOne();
			TakeOne();
			return new JsonBool(false);
			break;
		case 'n':
			TakeOne();
			TakeOne();
			TakeOne();
			TakeOne();
			return new JsonNull();
			break;
	}
}

JsonObject*
JsonParser::TakeObject()
{
	TakeUntil('{');
	TakeOne();
	char chr;
	JsonObject* obj = new JsonObject();
	JsonString* name;
	while (chr = Peek()) {
		if (chr == '}') {
			TakeOne();
			return obj;
		}
		TakeWhitespace();
		name = TakeString();
		TakeUntil(':');
		TakeOne();
		obj->AddPair(name, TakeValue());
		TakeWhitespace();
		if(Peek() == ',')
			TakeOne();
		TakeWhitespace();
	}
}

JsonArray*
JsonParser::TakeArray()
{
	char chr;
	JsonArray* arr = new JsonArray();
	
	TakeOne();
	
	while(chr = Peek()) {
		if(chr == ']') {
			TakeOne();
			return arr;
		}
		TakeWhitespace();
		arr->AddValue(TakeValue());
		TakeWhitespace();
		if(Peek() == ',')
			TakeOne();
	}
}


void main(int argv, char** argc)
{
	BFile file("big.json", B_READ_ONLY);
	off_t size;
	file.GetSize(&size);
	char* c = new char[size];
	file.Read((void*)c, size);
	JsonParser* parse = new JsonParser(BString(c));
}
