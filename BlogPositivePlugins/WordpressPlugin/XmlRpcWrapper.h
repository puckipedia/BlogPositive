#ifndef XML_RPC_WRAPPER_H
#define XML_RPC_WRAPPER_H
#include <String.h>
#include <List.h>
class BaseXmlValue
{
public:
    virtual void PushContent(BString *string)
    {
    }
};

class XmlValue : public BaseXmlValue
{
public:
    void PushContent(BString *string)
    {
//	*string << "<value>" << *fValue << "</value>";
	*string << "<value><" << *boxType << ">" << *fValue << "</" << *boxType << "></value>";
    }
    XmlValue(BString *string)
    {
	fValue = string;
	boxType = new BString("string");
    }
    XmlValue(BString *string, BString *boxx)
    {
	fValue = string;
	boxType = boxx;
    }
    XmlValue(int fAs)
    {
	fValue = new BString();
	*fValue << fAs;
	boxType = new BString("i4");
    }
    XmlValue(double fDs)
    {
	fValue = new BString();
	*fValue << fDs;
	boxType = new BString("string");
    }
    private:
    BString *fValue;
    BString *boxType;
};

class XmlArray : public BaseXmlValue
{
public:
    void PushContent(BString *string)
    {
	*string << "<array><data>";
	for(int i = 0; i < Items(); i++)
	{
	    ItemAt(i)->PushContent(string);
	}
	*string << "</data></array>";
    }
    XmlArray()
	{
	    list = new BList();
	}
    void AddItem(BaseXmlValue *xValue)
    {
	list->AddItem(xValue);
    }
    int32 Items()
	{
	    return list->CountItems();
	}
    BaseXmlValue *ItemAt(int32 i)
	{
	    return (BaseXmlValue *)list->ItemAt(i);
	}
private:
    BList *list;
};

class XmlNameValuePair : public BaseXmlValue
{
public:
    XmlNameValuePair() {}
    XmlNameValuePair(BString *name, BaseXmlValue *value)
	{
	    fName = name;
	    fValue = value;
	}
    void PushContent(BString *string)
	{
	    *string << "<value><member><name>" << *fName << "</name>";
	    fValue->PushContent(string);
	    *string << "</member></value>";
	}
    BString *fName;
    BaseXmlValue *fValue;
};

class XmlStruct : public BaseXmlValue
{
public:
    void PushContent(BString *string)
	{
	    *string << "<struct>";
	    for(int i = 0; i < Items(); i++)
	    {
		ItemAt(i)->PushContent(string);
	    }
	    *string << "</struct>";
	}
    XmlStruct()
	{
	    list = new BList();
	}
    void AddItem(XmlNameValuePair *pair)
	{
	    list->AddItem(pair);
	}
    int32 Items()
	{
	    return list->CountItems();
	}
    XmlNameValuePair *ItemAt(int32 i)
	{
	    return (XmlNameValuePair *)list->ItemAt(i);
	}
private:
    BList *list;
};

class XmlRpcRequest
{
public:
    XmlRpcRequest()
	{
	    fXmlValues = new BList();
	}
    void AddItem(BaseXmlValue *val)
	{
	    fXmlValues->AddItem(val);
	}
    BString *GetData()
	{
	    BString *string = new BString();
	    *string << "<?xml version=\"1.0\"?>\n<methodCall><methodName>" << *fMethodName << "</methodName><params>";
	    for(int i = 0; i < fXmlValues->CountItems(); i++)
	    {
		*string << "<param>";
		((BaseXmlValue *)fXmlValues->ItemAt(i))->PushContent(string);
		*string << "</param>";
	    }
	    *string << "</params></methodCall>";
	    return string;
	}
    BString *fMethodName;
private:
    BList *fXmlValues;
};

#endif
