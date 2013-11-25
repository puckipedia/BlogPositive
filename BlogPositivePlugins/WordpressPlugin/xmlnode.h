#ifndef XMLNODE
#define	XMLNODE

#include <stdio.h>
#include <cctype>

#include <be/AppKit.h>
#include <be/InterfaceKit.h>
#include <be/SupportKit.h>
#include <ObjectList.h>


// NODEs have children
// SINGLEs have a Value()
// STRINGs are handled internally
#define XML_TYPE_NODE		0
#define XML_TYPE_SINGLE		1
#define XML_TYPE_SIMPLE		1
#define XML_TYPE_STRING		2
#define XML_TYPE_COMMENT	3


#define XML_ENCODING_NONE	0xFFFFFFFF

#define XMLNODE_NONE		0xFFFFFFFF


class XmlNode: public BArchivable
{
public:
	// empty node with name
	XmlNode(XmlNode* parent, const char* name = "");	
	XmlNode(BMessage *archive);

	// build tree from already loaded file:
	XmlNode(const char* buf, XmlNode* parent);	

	XmlNode(const XmlNode& other);

	XmlNode& operator=(const XmlNode& other);

	static BArchivable* Instatiate(BMessage* msg);
	
	status_t				Archive(BMessage *msg, bool deep = true) const;
	static	BArchivable*	Instantiate(BMessage *data);
	
	virtual ~XmlNode();
	
	void					SetMarked(bool mark=true);
	bool					Marked();

	// 
	const char* Name() const;
	void SetName(const char* name);
	uint32 Type() const;
	
	XmlNode*	Parent() const;
	
	const BString& Value() const;
	int			ValueAsInt() const;
	void SetValue(const BString& value);
	
	// Attributes:
	
	uint32		Attributes() const;
	
	void		AddAttribute(const char* name, const char* value);
	void		AddAttribute(const char* name, int value);
	
	uint32		IndexOfAttribute(const char* name) const;
	
	status_t	RemoveAttribute(uint32 index);
	status_t	RemoveAttribute(const char* key);
	void		RemoveAllAttributes();
	
	const char*	Attribute(const char* key) const;
	int			AttributeAsInt(const char* key, int defaultValue=0) const;
	bool		AttributeAsBool(const char* key, bool defaultValue=false) const;

	const char*	Attribute(int32 index) const;
	//int			AttributeAsInt(uint32 index, int defaultValue=0) const;
	//bool		AttributeAsBool(uint32 index, bool defaultValue=false) const;

	const char*	AttributeKey(int32 index) const;
	
	/*
	void		SetAttribute(const char* key, const char* value);
	*/
	
	
	// Children:
	uint32		Children() const;
	void		AddChild(XmlNode* Child, int pos=-1);
	//void 		InsertChild(XmlNode* Child, int pos=0);
	void		RemoveChild(uint32 index);
	void		RemoveChild(XmlNode* node);
	void		RemoveAllChildren();
	//XmlNode*	DetachChild(uint32 index);
	//XmlNode*	DetachChild(XmlNode* node);
	XmlNode*	ItemAt(uint32 index) const;
	uint32		IndexOf(XmlNode* child) const;
	
	// Find a child matching 'name'
	// You can specify a child in this list for FindNext functionality OR search recursivly;
	XmlNode*	FindChild(const char* name, XmlNode* prev=NULL, bool recursive=false);

	// get child by path. returns NULL if not found.
	XmlNode*	GetChild(const char* path);
	
	// If Child does not exist, it will be created
	XmlNode*	CreateChild(const char* path, const char* value=NULL);
	XmlNode*	CreateChild(const char* path, int value);	
	
	bool		RemoveChild(const char* path);
	
	XmlNode*	DetachChild(uint32 index);
	
	const char*	Parse(const char* buf);

public:	
	void Display() const;
private:
	void Display(int level) const;
public:	

	// saves this node and its children to the specified file, producing a valid XML-file
	bool		SaveToFile(const char* filename) const;
    BString *SavetoString() const;
protected:
	// internal: Save this node and its children, does not output header and footer, use SaveToFile instead!
    bool SaveString(BString *str, int depth) const;
	bool		SaveNode(int ref, int depth) const;

public:
	bool		LoadFile(const char* filename);
	
	
	// conveniance function
	// turns this node into a comment: <!-- commentstring -->
	// only useful for new nodes
	void		Comment(const char* commentstring);
	
private:
	// parses a given string and adds it to the attribute list
	int			ParseAttributes(const char* b);

	// pointer for parser:
	const char*	p;

	BString		mName;
	uint32		mType;
	
	BString		mData;
	
	XmlNode*	mParent;
	
	BList		mAttribute;
	BObjectList<XmlNode> mChild;
	
	// for convert_to_utf8, values declared in <be/support/UTF8.h>
	static uint32	encoding;
};

#endif
