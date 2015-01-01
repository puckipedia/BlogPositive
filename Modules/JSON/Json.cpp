#include <File.h>
#include <ObjectList.h>
#include <String.h>
#include <cstdlib>
#include <stdio.h>
#include <cctype>
#include "Json.h"


JsonValue::JsonValue()
{
	
}

template<class T>
T*
ParseNew(JsonParser* parser)
{
	T* t = new T();
	t->Parse(parser);
	return t;
}

void
JsonValue::Serialize(BString* str)
{
	
}


void
JsonValue::Parse(JsonParser* parser)
{
	
}


BString
JsonString::Escape(BString str)
{
	BString ret(str);
	ret.ReplaceAll("\\", "\\\\");
	ret.ReplaceAll("\"", "\\\"");
	ret.ReplaceAll("\b", "\\b");
	ret.ReplaceAll("\f", "\\g");
	ret.ReplaceAll("\n", "\\n");
	ret.ReplaceAll("\r", "\\r");
	ret.ReplaceAll("\t", "\\t");
	return ret;
}


void
JsonString::Serialize(BString* str)
{
	*str << "\"";
	*str << Escape(*this);
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


JsonValue*
JsonObject::Get(BString key)
{
	for(int i = 0; i < fPairList->CountItems(); i++) {
		JsonPair* pair = fPairList->ItemAt(i);
		if(*pair->fName == key)
			return pair->fValue;
	}
	return NULL;
}

void
JsonObject::Serialize(BString* str)
{
	*str << "{";
	for (int i = 0; i < fPairList->CountItems(); i++) {
		JsonPair* pair = fPairList->ItemAt(i);
		pair->fName->Serialize(str);
		*str << ": ";
		pair->fValue->Serialize(str);
		if(i < fPairList->CountItems()-1) {
			*str << ", ";
		}
	}
	*str << "}";
}


void
JsonNumber::Serialize(BString* str)
{
	*str << fDouble;
}


void
JsonArray::Serialize(BString* str)
{
	*str << "[";
	for (int i = 0; i < CountItems(); i++) {
		JsonValue* val = ItemAt(i);
		val->Serialize(str);
		if(i < CountItems()-1) {
			*str << ", ";
		}
	}
	*str << "]";
}




void
JsonBool::Serialize(BString* str)
{
	*str << fVal ? "true" : "false";
}


void
JsonNull::Serialize(BString* str)
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
			thing = ParseNew<JsonObject>(this);
			break;
		case '[':
			thing = ParseNew<JsonArray>(this);
			break;
	}
	fValue = thing;
}


void
JsonNumber::Parse(JsonParser* parser) {
	BString numberPart;
	parser->TakeWhitespace();
	if(parser->Peek() == '-') {
		numberPart << '-';
		parser->TakeOne();
	}
	while(isdigit(parser->Peek()))
		numberPart << parser->TakeOne();
	if(parser->Peek() == '.') {
		numberPart << '.';
		parser->TakeOne();
	}
	while(isdigit(parser->Peek()))
		numberPart << parser->TakeOne();
	if(parser->Peek() == 'e') {
		numberPart << 'e';
		parser->TakeOne();
	}
	while(isdigit(parser->Peek()))
		numberPart << parser->TakeOne();
	double i = strtod(numberPart.String(), NULL);
	fDouble = i;
}

double
JsonNumber::Value() {
	return fDouble;
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

void
JsonString::Parse(JsonParser* parser)
{
	if(parser->Peek() == '"')
		parser->TakeOne();
	States state = StringTakeChar;
	
	char chr;
	while(chr = parser->TakeOne()) {
		switch (state) {
			case StringTakeChar:
				switch (chr) {
					case '\\':
						state = StringTakeSpecial;
						break;
					case '"':
						return;
						break;
					default:
						*this << chr;
						break;
				}
				break;
			case StringTakeSpecial:
				switch(chr) {
					case 'b':
						*this << '\b';
						break;
					case 'f':
						*this << '\f';
						break;
					case 'n':
						*this << '\n';
						break;
					case 'r':
						*this << '\r';
						break;
					case 't':
						*this << '\t';
						break;
					case 'u':
						parser->TakeOne();
						parser->TakeOne();
						parser->TakeOne();
						parser->TakeOne();
						break;
					default:
						*this << chr;
				}
				state = StringTakeChar;
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
			return ParseNew<JsonString>(this);
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
			return ParseNew<JsonNumber>(this);
			break;
		case '[':
			return ParseNew<JsonArray>(this);
			break;
		case '{':
			return ParseNew<JsonObject>(this);
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

void
JsonObject::Parse(JsonParser* parser)
{
	parser->TakeUntil('{');
	parser->TakeOne();
	char chr;
	JsonString* name;
	while (chr = parser->Peek()) {
		if (chr == '}') {
			parser->TakeOne();
			return;
		}
		parser->TakeWhitespace();
		name = ParseNew<JsonString>(parser);
		parser->TakeUntil(':');
		parser->TakeOne();
		AddPair(name, parser->TakeValue());
		parser->TakeWhitespace();
		if(parser->Peek() == ',')
			parser->TakeOne();
		parser->TakeWhitespace();
	}
}

void
JsonArray::Parse(JsonParser* parser)
{
	char chr;
	parser->TakeOne();
	
	while(chr = parser->Peek()) {
		if(chr == ']') {
			parser->TakeOne();
			return;
		}
		parser->TakeWhitespace();
		AddItem(parser->TakeValue());
		parser->TakeWhitespace();
		if(parser->Peek() == ',')
			parser->TakeOne();
	}
}

JsonValue*
JsonParser::Value()
{
	return fValue;
}
