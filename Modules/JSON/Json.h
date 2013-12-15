#ifndef JSON_H
#define JSON_H

#include <ObjectList.h>

class BString;

class JsonValue {
public:
	virtual void AddSelf(BString* str);
};

class JsonString : public JsonValue {
public:
	virtual void AddString(const char* c);
	virtual void AddChar(char c);
	void AddSelf(BString* str);
private:
	BString fString;
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
	void AddSelf(BString *str);
private:
	BObjectList<JsonPair>* fPairList;
};

class JsonNumber : public JsonValue {
public:
	JsonNumber(double d);
	void AddSelf(BString* str);
private:
	double fDouble;
};

class JsonArray : public JsonValue {
public:
	JsonArray();
	void AddValue(JsonValue* Value);
	void AddSelf(BString* str);
private:
	BObjectList<JsonValue>* fObjectList;
};

class JsonBool : public JsonValue {
public:
	JsonBool(bool val) {}
	void AddSelf(BString* str);
private:
	bool fVal;
};

class JsonNull : public JsonValue {
	void AddSelf(BString* str);
};



class JsonParser {
public:
	JsonParser(BString str);
private:
	void TakeWhitespace();
	char Peek();
	char TakeOne();
	JsonString* TakeString();
	JsonValue* TakeValue();
	JsonNumber* TakeNumber();
	JsonArray* TakeArray();
	JsonObject* TakeObject();
	void TakeUntil(char end);
	void GoBack();
private:
	int32 fPointer;
	BString fString;
};

#endif
