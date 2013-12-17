/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include <curl/curl.h>

#include <String.h>
#include <UnicodeChar.h>

#include "BloggerPlugin.h"
#include "BlogPositiveBlog.h"
#include "Json.h"
#include "Keys.h"

void
GetAuthentication(BString Auth, BString* AuthKey, BString* BlogId)
{
	int32 SeperatorLocation = Auth.FindFirst("|||");

	Auth.CopyInto(*AuthKey, 0, SeperatorLocation);
	if(*AuthKey == "DEFAULT_AUTH") {
		*AuthKey = BP_ACCESS_TOKEN;
	}
	Auth.CopyInto(*BlogId, SeperatorLocation + 3, Auth.Length() - SeperatorLocation - 3);
}


size_t BloggerWriteToString(void* bloc, size_t size, size_t nmemb, void* userp)
{
	char* charBloc = static_cast<char*>(bloc);
	const char* cBlock = const_cast<const char*>(charBloc);
	*(static_cast<BString*>(userp)) << cBlock;
	return nmemb;
}


JsonObject*
Post(JsonValue* r, BString* responseString,
	BString Url, BString Auth)
{
	BString authKey;
	BString blogId;
	GetAuthentication(Auth, &authKey, &blogId);
	Url.ReplaceAll("{blogId}", blogId);
	Url.ReplaceAll("{apiBase}", "https://www.googleapis.com/blogger/v3");
	BString dataString;
	r->Serialize(&dataString);
	CURL* curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, Url.String());

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataString.String());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BloggerWriteToString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(responseString));
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_perform(curl);

	JsonParser parser(*responseString);
	JsonObject* obj = dynamic_cast<JsonObject*>(parser.Value());
	return obj;
}


JsonObject*
Get(BString* responseString, BString Url, BString Auth)
{
	BString authKey;
	BString blogId;
	GetAuthentication(Auth, &authKey, &blogId);
	Url.ReplaceAll("{blogId}", blogId);
	Url.ReplaceAll("{apiBase}", "https://www.googleapis.com/blogger/v3");
	if(Url.FindFirst("?") == B_ERROR) {
		Url << "?access_token=" << authKey;
	} else {
		Url << "&access_token=" << authKey;
	}
	printf("Url: %s\n", Url.String());
	CURL* curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, Url.String());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BloggerWriteToString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(responseString));
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_perform(curl);

	JsonParser parser(*responseString);
	JsonObject* obj = dynamic_cast<JsonObject*>(parser.Value());
	BString str;
	obj->Serialize(&str);
	printf("lol: %s\n", str.String());
	return NULL; //obj;
}


uint32
BloggerPlugin::Version()
{
	return 0;
}


const char*
BloggerPlugin::Name()
{
	return "Blogger";
}


int32
BloggerPlugin::Type()
{
	return kBlogPositiveBlogApi;
}


uint32
BloggerPlugin::MainHandler()
{
	return 'BLGR';
}


bool
BloggerPlugin::Supports(int32 Code)
{
	return Code == MainHandler();
}


PostList*
BloggerPlugin::GetBlogPosts(BlogPositiveBlog* aBlog)
{
	BString responseString;
	JsonObject* response = Get(&responseString, "{apiBase}/blogs/{blogId}/posts",
		aBlog->Authentication());
	printf("Resp: %s\n", responseString.String());
	JsonArray* posts = dynamic_cast<JsonArray*>(response->Get("posts"));
	BString resp;
	posts->Serialize(&resp);
	printf("Response: %s\n", resp.String());
	return new PostList();
}
