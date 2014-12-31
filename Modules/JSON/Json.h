#ifndef JSON_H
#define JSON_H

#include <ObjectList.h>

class BString;
class JsonParser;

class JsonValue {
public:
	JsonValue();
	JsonValue(JsonParser* parser);
	virtual void Serialize(BString* str);
	virtual void Parse(JsonParser* parser);
};

class JsonString : public JsonValue, public BString {
public:
	void Serialize(BString* str);
	void Parse(JsonParser* parser);
};

class JsonPair {
public:
	JsonPair(JsonString* n, JsonValue* v);
	JsonString* fName;
	JsonValue* fValue;
};

class JsonObject : public JsonValue {
public:
	JsonObject();
	virtual void AddPair(JsonString* str, JsonValue* val);
	void Serialize(BString *str);
	void Parse(JsonParser* parser);
	JsonValue* Get(BString key);
private:
	BObjectList<JsonPair>* fPairList;
};

class JsonNumber : public JsonValue {
public:
	void Serialize(BString* str);
	void Parse(JsonParser* parser);

	double Value();
private:
	double fDouble;
};

class JsonArray : public JsonValue, public BObjectList<JsonValue> {
public:
	void Serialize(BString* str);
	void Parse(JsonParser* parser);
};

class JsonBool : public JsonValue {
public:
	JsonBool(bool val) {}
	void Serialize(BString* str);
private:
	bool fVal;
};

class JsonNull : public JsonValue {
	void Serialize(BString* str);
};



class JsonParser {
public:
	JsonParser(BString str);
	JsonValue* Value();
	void TakeWhitespace();
	char Peek();
	char TakeOne();
	void TakeUntil(char end);
	void GoBack();
	JsonValue* TakeValue();
private:
	int32 fPointer;
	BString fString;
	JsonValue* fValue;
};

#endif
