/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "WordpressPlugin.h"

#include "../../API/BlogPositiveBlog.h"
#include "../../API/BlogPositivePost.h"
#include <curl/curl.h>
#include <stdio.h>
#include <String.h>
#include "xmlnode.h"
#include "XmlRpcWrapper.h"

void
WordpressPlugin::GetAuthentication(BString Auth, BString* Username,
	BString* Password, BString* XmlRPCUrl)
{
	int32 SeperatorLocation = Auth.FindFirst("|||");
	int32 LastLocation = Auth.FindLast("|||");

	Auth.CopyInto(*Username, 0, SeperatorLocation);

	Auth.CopyInto(*Password, SeperatorLocation + 3,
		LastLocation - SeperatorLocation - 3);

	Auth.CopyInto(*XmlRPCUrl, LastLocation + 3, Auth.Length()-LastLocation - 3);
}


uint32
WordpressPlugin::Version()
{
	return 0;
}


char*
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


size_t WriteTobString(void* bloc, size_t size, size_t nmemb, void* userp)
{
	*((BString* )userp) << (const char* )bloc;
	return nmemb;
}


class WordpressPost : public BlogPositivePost
{
public:
				WordpressPost(BString aPostName, BString aPostContent,
					BString aPostId, BlogPositiveBlog* aBlog);
	const char*	PostId();
private:
	BString fId;
};


WordpressPost::WordpressPost(BString postName, BString postContent,
	BString postId, BlogPositiveBlog* blog)
	:
	fName(postName),
	fPage(postContent),
	fId(postId),
	fBlog(blog)
{
}


const char*
WordpressPost::PostId()
{
	return fId.String();
}


BList*
WordpressPlugin::GetBlogPosts(BlogPositiveBlog* aBlog)
{
	BString username;
	BString password;
	BString xmlrpcurl;
	BString Auth(aBlog->Authentication());

	GetAuthentication(Auth, &username, &password, &xmlrpcurl);

	XmlRpcRequest* r = new XmlRpcRequest();

	r->SetMethodName("metaWeblog.getRecentPosts");
	r->AddItem(new XmlValue(1));
	r->AddItem(new XmlValue(&username));
	r->AddItem(new XmlValue(&password));
	r->AddItem(new XmlValue(30));

	CURL* curl = curl_easy_init();

	BString responseString;
	BString dataString(r->GetData());

	curl_easy_setopt(curl, CURLOPT_URL, xmlrpcurl.String());

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataString.String());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteTobString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseString);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_perform(curl);

	XmlNode* responseNode = new XmlNode(str.String(), NULL);
	XmlNode* postNode = NULL;
	if (responseNode->FindChild("param", NULL, true) == NULL)
		return new BList();
	XmlNode* postListNode = responseNode->FindChild("param", NULL, true)
		->FindChild("array", NULL, true)->FindChild("data", NULL, true);
	BList* postList = new BList();

	while ((postNode = postListNode->FindChild("value", postNode)) != NULL)
	{
		BString aPostName;
		BString aPostContent;
		BString aPostId;
		XmlNode* firstStructNode = postNode->FindChild("struct", NULL, true);
		for (int i = 0; i < firstStructNode->Children(); i++)
		{
			XmlNode node = firstStructNode->ItemAt(i);
			BString name = node->FindChild("name")->Value();
			if (name == "title")
			{
				aPostName = a->FindChild("string", NULL, true)->Value();
			}
			if (name == "description")
			{
				aPostContent = a->FindChild("string", NULL, true)->Value();
			}
			if (name == "postid")
			{
				aPostId = a->FindChild("string", NULL, true)->Value();
			}
	}
	WordpressPost* aPost = new WordpressPost(aPostName, aPostContent,
		aPostId, aBlog);
	postList->AddItem(aPost);
	}
	return postList;
}


void
WordpressPlugin::RemovePost(BlogPositivePost* aPost)
{
	BlogPositiveBlog* aBlog = aPost->Blog();
	WordpressPost* wpPost = (WordpressPost* )aPost;

	BString username;
	BString password;
	BString xmlrpcurl;
	BString Auth(aBlog->Authentication());

	GetAuthentication(Auth, &username, &password, &xmlrpcurl);

	XmlRpcRequest* request = new XmlRpcRequest();
	request->fMethodName = new BString("wp.deletePost");
	request->AddItem(new XmlValue(1));
	request->AddItem(new XmlValue(username));
	request->AddItem(new XmlValue(password));
	request->AddItem(new XmlValue(wpPost->PostId(), "int"));

	BString dataString = request->GetData();

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, xmlrpcurl.String());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataString.String());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_perform(curl);

}


void
WordpressPlugin::SavePost(BlogPositivePost* aPost)
{
BlogPositiveBlog* aBlog = aPost->Blog();
WordpressPost* wpPost = (WordpressPost* )aPost;

	BString username;
	BString password;
	BString xmlrpcurl;
	BString Auth(aBlog->Authentication());

	GetAuthentication(Auth, &username, &password, &xmlrpcurl);

	XmlRpcRequest* request = new XmlRpcRequest();
	request->fMethodName = new BString("wp.editPost");
	request->AddItem(new XmlValue(1));
	request->AddItem(new XmlValue(&username));
	request->AddItem(new XmlValue(&password));
	request->AddItem(new XmlValue(wpPost->PostId(), "int"));

	XmlStruct* struc = new XmlStruct();
	XmlNameValuePair* p = new XmlNameValuePair();
	p->fName = new BString("post_content");
	p->fValue = new XmlValue(new BString(wpPost->Page()));
	struc->AddItem(p);
	request->AddItem(struc);

	CURL* curl = curl_easy_init();

	BString str;

	curl_easy_setopt(curl, CURLOPT_URL, xmlrpcurl.String());
	BString requestString = request->GetData();

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestString.String());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_perform(curl);
}


BlogPositivePost*
WordpressPlugin::CreateNewPost(BlogPositiveBlog* aBlog, const char* aName)
{
	BString Auth = BString(aBlog->Authentication());
	BString username;
	BString password;
	BString xmlrpcurl;

	GetAuthentication(Auth, &username, &password, &xmlrpcurl);

	XmlRpcRequest* request = new XmlRpcRequest();
	request->fMethodName = new BString("wp.newPost");
	request->AddItem(new XmlValue(1));
	request->AddItem(new XmlValue(&username));
	request->AddItem(new XmlValue(&password));
	XmlStruct* strut = new XmlStruct();
	strut->AddItem(new XmlNameValuePair("post_status",
		new XmlValue("publish")));
	strut->AddItem(new XmlNameValuePair("post_title", new XmlValue(aName)));
	request->AddItem(strut);

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, xmlrpcurl.String());

	BString responseString;
	BString requestString = request->GetData();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestString.String());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteTobString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void* )&str);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_perform(curl);

	XmlNode* responseNode = new XmlNode(responseString.String(), NULL);
	XmlNode* rstring = responseNode->FindChild("string", NULL, true);
	if (rstring != NULL) {
		WordpressPost* post = new WordpressPost(aName, "",
			rstring->Value(), aBlog);
		return post;
	}
	return NULL;
}