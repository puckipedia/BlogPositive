/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "WordpressPlugin.h"

#include <String.h>
#include <curl/curl.h>
#include <stdio.h>
#include "../../API/BlogPositiveBlog.h"
#include "xmlnode.h"
#include "XmlRpcWrapper.h"

uint32
WordpressPlugin::Version()
{
    return 0;
}

char *
WordpressPlugin::Name()
{
    return "Wordpress.com or self-hosted";
}

int32
WordpressPlugin::Type()
{
    return kBlogPositiveBlogApi;
}

uint32
WordpressPlugin::MainHandler()
{
    return 'WoPr';
}

bool
WordpressPlugin::Supports(int32 Code)
{
    return Code == 'WoPr';
}
size_t WriteTobString(void *bloc, size_t size, size_t nmemb, void *userp)
{
    printf("Got part! %d %d\n", size, nmemb);
    *((BString *)userp) << (const char *)bloc;
    return nmemb;
}


class WordpressPost : public BlogPositivePost
{
public:
    WordpressPost(BString aPostName, BString aPostContent, BString aPostId, BlogPositiveBlog *aBlog)
    {
	fName = new BString(aPostName);
	fPage = new BString(aPostContent);
	fId = new BString(aPostId);
	fBlog = aBlog;
	printf("Post! %s %s %s\n", aPostName.String(), aPostContent.String(), aPostId.String());
    }
    const char *
    PostId()
    {
	return fId->String();
    }
private:
    BString *fId;
};

#define CreateStringValue(name, value)			\
    XmlNode * name = new XmlNode(params, "param"); {	\
	XmlNode *val = new XmlNode(name, "value");	\
	XmlNode *string = new XmlNode(val, "string");	\
	string->SetValue(value);			\
	val->AddChild(string);				\
	name->AddChild(val);				\
    }
#define CreateIntValue(name, value, cf)			\
    XmlNode * name = new XmlNode(params, "param"); {	\
	XmlNode *val = new XmlNode(name, "value");	\
	XmlNode *string = new XmlNode(val, "int");	\
	string->SetValue(value);			\
	string->SetCdata(cf);				\
	val->AddChild(string);				\
	name->AddChild(val);				\
    }
#define CreateValue(name, value) \
    XmlNode * name = new XmlNode(params, "param"); {	\
	XmlNode *val = new XmlNode(name, "value"); \
        val->SetValue(value);				\
	name->AddChild(val); \
    }

#define Struct(__ding, __name, __val, __a, __cda) do {			\
	XmlNode *member = new XmlNode(__ding, "member");	\
	XmlNode *name = new XmlNode(member, "name");		\
	name->SetValue(BString(__name));			\
	XmlNode *value = new XmlNode(member, "value");		\
	XmlNode *in = new XmlNode(value, __a); \
	in->SetValue(BString(__val));		 \
	value->AddChild(in);\
	member->AddChild(name); member->AddChild(value);	\
	__ding->AddChild(member);				\
	in->SetCdata(__cda);					\
    }	while(0)							

BList *
WordpressPlugin::GetBlogPosts(BlogPositiveBlog *aBlog)
{
    BString Auth = BString(aBlog->Authentication());

    int32 SeperatorLocation = Auth.FindFirst("|||");
    int32 LastLocation = Auth.FindLast("|||");
    
    BString username;
    Auth.CopyInto(username, 0, SeperatorLocation);

    BString password;
    Auth.CopyInto(password, SeperatorLocation+3, LastLocation-SeperatorLocation-3);
    
    BString xmlrpcurl;
    Auth.CopyInto(xmlrpcurl, LastLocation+3, Auth.Length()-LastLocation-3);

    XmlRpcRequest *r = new XmlRpcRequest();
    r->fMethodName = new BString("metaWeblog.getRecentPosts");
    r->AddItem(new XmlValue(1));
    r->AddItem(new XmlValue(&username));
    r->AddItem(new XmlValue(&password));
    r->AddItem(new XmlValue(30));
    CURL *curl = curl_easy_init();
    
    BString str;

    curl_easy_setopt(curl, CURLOPT_URL, xmlrpcurl.String());
    BString *stttr = r->GetData();
    printf("LOADING POSTS? %s %s %s %s\n", username.String(), password.String(), xmlrpcurl.String(), stttr->String());

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, stttr->String());
    delete stttr;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteTobString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&str);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_perform(curl);

    printf("-%s-\n", str.String());
    XmlNode *responseNode = new XmlNode(str.String(), NULL);
    XmlNode *postNode = NULL;
    if(responseNode->FindChild("param", NULL, true) == NULL)
	return new BList();
    XmlNode *postListNode = responseNode->FindChild("param", NULL, true)->FindChild("array", NULL, true)->FindChild("data", NULL, true);
    BList *postList = new BList();
    while((postNode = postListNode->FindChild("value", postNode)) != NULL)
    {
	const BString *aPostName;
	const BString *aPostContent;
	const BString *aPostId;
	XmlNode *firstStructNode = postNode->FindChild("struct", NULL, true);
	for(int i = 0; i < firstStructNode->Children(); i++)
	{
	    XmlNode *a = firstStructNode->ItemAt(i);
	    BString *name = (BString *)&a->FindChild("name")->Value();
	    if(name->Compare(BString("title")) == 0)
	    {
		aPostName = &a->FindChild("string", NULL, true)->Value();
	    }
	    if(name->Compare(BString("description")) == 0)
	    {
		aPostContent = &a->FindChild("string", NULL, true)->Value();
	    }
	    if(name->Compare(BString("postid")) == 0)
	    {
		aPostId = &a->FindChild("string", NULL, true)->Value();
	    }
	}
	WordpressPost *aPost = new WordpressPost((BString)*aPostName, (BString)*aPostContent, (BString)*aPostId, aBlog);
	postList->AddItem(aPost);
    }
    return postList;
}

void
WordpressPlugin::RemovePost(BlogPositivePost *aPost)
{
    BlogPositiveBlog *aBlog = aPost->Blog();
    WordpressPost *wpPost = (WordpressPost *)aPost;

    BString Auth = BString(aBlog->Authentication());

    int32 SeperatorLocation = Auth.FindFirst("|||");
    int32 LastLocation = Auth.FindLast("|||");
    
    BString username;
    Auth.CopyInto(username, 0, SeperatorLocation);

    BString password;
    Auth.CopyInto(password, SeperatorLocation+3, LastLocation-SeperatorLocation-3);
    
    BString xmlrpcurl;
    Auth.CopyInto(xmlrpcurl, LastLocation+3, Auth.Length()-LastLocation-3);

    XmlRpcRequest *request = new XmlRpcRequest();
    request->fMethodName = new BString("wp.deletePost");
    request->AddItem(new XmlValue(1));
    request->AddItem(new XmlValue(&username));
    request->AddItem(new XmlValue(&password));
    request->AddItem(new XmlValue(new BString(wpPost->PostId()), new BString("int")));

    CURL *curl = curl_easy_init();

    BString str;

    curl_easy_setopt(curl, CURLOPT_URL, xmlrpcurl.String());
    BString *stttr = request->GetData();
    printf("REMOVING! %s %s %s %s\n", username.String(), password.String(), xmlrpcurl.String(), stttr->String());

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, stttr->String());
    delete stttr;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteTobString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&str);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_perform(curl);

    printf("-%s-\n", str.String());
}
void
WordpressPlugin::SavePost(BlogPositivePost *aPost)
{
    BlogPositiveBlog *aBlog = aPost->Blog();
    WordpressPost *wpPost = (WordpressPost *)aPost;

    BString Auth = BString(aBlog->Authentication());

    int32 SeperatorLocation = Auth.FindFirst("|||");
    int32 LastLocation = Auth.FindLast("|||");
    
    BString username;
    Auth.CopyInto(username, 0, SeperatorLocation);

    BString password;
    Auth.CopyInto(password, SeperatorLocation+3, LastLocation-SeperatorLocation-3);
    
    BString xmlrpcurl;
    Auth.CopyInto(xmlrpcurl, LastLocation+3, Auth.Length()-LastLocation-3);

    XmlRpcRequest *request = new XmlRpcRequest();
    request->fMethodName = new BString("wp.editPost");
    request->AddItem(new XmlValue(1));  
    request->AddItem(new XmlValue(&username));
    request->AddItem(new XmlValue(&password));
    request->AddItem(new XmlValue(new BString(wpPost->PostId()), new BString("int")));
    XmlStruct *struc = new XmlStruct();
    XmlNameValuePair *p = new XmlNameValuePair();
    p->fName = new BString("post_content");
    p->fValue = new XmlValue(new BString(wpPost->Page()));
    struc->AddItem(p);
    request->AddItem(struc);
    CURL *curl = curl_easy_init();

    BString str;

    curl_easy_setopt(curl, CURLOPT_URL, xmlrpcurl.String());
    BString *stttr = request->GetData();
    printf("POSTING! %s %s %s %s\n", username.String(), password.String(), xmlrpcurl.String(), stttr->String());

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, stttr->String());
    delete stttr;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteTobString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&str);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_perform(curl);

    printf("-%s-\n", str.String());
    XmlNode *responseNode = new XmlNode(str.String(), NULL);
    
}

BlogPositivePost *
WordpressPlugin::CreateNewPost(BlogPositiveBlog *aBlog, const char *aName)
{
    BString Auth = BString(aBlog->Authentication());

    int32 SeperatorLocation = Auth.FindFirst("|||");
    int32 LastLocation = Auth.FindLast("|||");
    
    BString username;
    Auth.CopyInto(username, 0, SeperatorLocation);

    BString password;
    Auth.CopyInto(password, SeperatorLocation+3, LastLocation-SeperatorLocation-3);
    
    BString xmlrpcurl;
    Auth.CopyInto(xmlrpcurl, LastLocation+3, Auth.Length()-LastLocation-3);


//    BString *profile = GetProfile(aBlog);
    XmlRpcRequest *request = new XmlRpcRequest();
    request->fMethodName = new BString("wp.newPost");
    request->AddItem(new XmlValue(1));
    request->AddItem(new XmlValue(&username));
    request->AddItem(new XmlValue(&password));
    XmlStruct *strut = new XmlStruct();
    strut->AddItem(new XmlNameValuePair(new BString("post_status"), new XmlValue(new BString("publish"))));
    strut->AddItem(new XmlNameValuePair(new BString("post_title"), new XmlValue(new BString(aName))));
    request->AddItem(strut);

    CURL *curl = curl_easy_init();

    BString str;

    curl_easy_setopt(curl, CURLOPT_URL, xmlrpcurl.String());
    BString *stttr = request->GetData();
    printf("POSTING! %s %s %s %s\n", username.String(), password.String(), xmlrpcurl.String(), stttr->String());

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, stttr->String());
    delete stttr;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteTobString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&str);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_perform(curl);

    printf("-%s-\n", str.String());
    XmlNode *responseNode = new XmlNode(str.String(), NULL);
    XmlNode *rstring = responseNode->FindChild("string", NULL, true);
    if(rstring != NULL) {
	WordpressPost *post = new WordpressPost(BString(aName), BString(), (BString)rstring->Value(), aBlog);
	return post;
    }
    return NULL;
}
