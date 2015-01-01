/*
 * Copyright 2014 Josef Gajdusek, atx@atx.name
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "GhostPlugin.h"

#include <stdio.h>
#include <curl/curl.h>

#include <Alert.h>
#include <Button.h>
#include <Catalog.h>
#include <TextControl.h>
#include <LayoutBuilder.h>
#include <Url.h>
#include <Window.h>

#include "BlogPositiveBlog.h"
#include "BlogPositiveSettings.h"
#include "PluginStub.h"
#include "Json.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Ghost Plugin"

#if STANDALONE
MODULES {
	MODULE(GhostPlugin)
}
#endif


static size_t readIntoBString(void* bloc, size_t size, size_t nmemb, void* userp)
{
	char* charBloc = static_cast<char*>(bloc);
	const char* cBlock = const_cast<const char*>(charBloc);
	*(static_cast<BString*>(userp)) << cBlock;
	return nmemb;
}


class GhostPost : public BlogPositivePost {
public:
						GhostPost(BString name, BString content,
								int id, BlogPositiveBlog* blog);
						GhostPost(JsonObject* obj, BlogPositiveBlog* blog);
	int					PostId();
private:
	int					fId;
};


GhostPost::GhostPost(BString name, BString content,
		int id, BlogPositiveBlog* blog)
	:
	BlogPositivePost(blog)
{
	SetName(name);
	SetPage(content);
	fId = id;
}


GhostPost::GhostPost(JsonObject* obj, BlogPositiveBlog* blog)
	:
	BlogPositivePost(blog)
{
	SetName(*dynamic_cast<BString*>(obj->Get("title")));
	SetPage(*dynamic_cast<BString*>(obj->Get("markdown")));
	fId = dynamic_cast<JsonNumber*>(obj->Get("id"))->Value();

	PostMetadata()->SetItem("post_status",
			new MetadataItem("Post status: ",
				*dynamic_cast<BString*>(obj->Get("status"))));
	BString* pdate = dynamic_cast<BString*>(obj->Get("published_at"));
	PostMetadata()->SetItem("post_date",
			new MetadataItem("Publish date:", pdate ? pdate->String() : ""));
}


int GhostPost::PostId()
{
	return fId;
}


JsonObject* GhostPlugin::_Request(BlogPositiveBlog* blog, BString target,
		BString method, BString data, bool authenticate)
{
	if (authenticate)
		_Authorize(blog);

	BString url = blog->Configuration()->GetString("blogUrl", "");
	url << target;

	BString resp("");

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.String());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, readIntoBString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(&resp));
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "BlogPositive/0.1 (Ghost/0.1)");
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.String());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.String());
	if (authenticate && blog->Configuration()->HasString("token")) {
		struct curl_slist* hdrs = NULL;
		hdrs = curl_slist_append(hdrs, BString("Authorization: Bearer ")
				<< blog->Configuration()->GetString("token", ""));
		// We only ever send JSON unless authenticating
		hdrs = curl_slist_append(hdrs, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdrs);
	}
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	JsonParser parser(resp);
	JsonObject* ret = dynamic_cast<JsonObject*>(parser.Value());
	if (ret->Get("errors")) {
		_DisplayErrors(dynamic_cast<JsonArray*>(ret->Get("errors")));
		return NULL;
	}
	if (ret->Get("type"))
		return NULL;
	return ret;
}


int32 GhostPlugin::_Authorize(BlogPositiveBlog* blog)
{
	CURL* curl = curl_easy_init();
	char* escUser =
		curl_easy_escape(curl, blog->Configuration()->GetString("email", ""), 0);
	char* escPwd =
		curl_easy_escape(curl, blog->Configuration()->GetString("password", ""), 0);
	JsonObject* obj = _Request(blog, "/ghost/api/v0.1/authentication/token",
			"POST",
			BString().SetToFormat(
				"grant_type=password&"
				"username=%s&"
				"password=%s&"
				"client_id=ghost-admin",
				escUser,
				escPwd),
			false);
	curl_free(escUser);
	curl_free(escPwd);
	curl_easy_cleanup(curl);

	if (!obj)
		return B_ERROR;

	blog->Configuration()->SetString("token",
			*dynamic_cast<BString*>(obj->Get("access_token")));

	delete obj;
	return B_OK;
}


PostList* GhostPlugin::GetBlogPosts(BlogPositiveBlog* blog)
{
	JsonObject* obj = _Request(blog, "/ghost/api/v0.1/posts/?status=all&limit=all");
	if (!obj)
		return new PostList();

	PostList* ret = new PostList();
	BObjectList<JsonValue>* posts =
		dynamic_cast<BObjectList<JsonValue>*>(obj->Get("posts"));
	for (int i = 0; i < posts->CountItems(); i++) {
		ret->AddItem(new GhostPost(
					dynamic_cast<JsonObject*>(posts->ItemAt(i)), blog));
	}

	delete obj;
	return ret;
}


void GhostPlugin::SavePost(BlogPositivePost* post)
{
	GhostPost* gpost = static_cast<GhostPost*>(post);
	_Request(post->Blog(), BString("/ghost/api/v0.1/posts/") << gpost->PostId(),
			"PUT",
			BString().SetToFormat(
				"{\"posts\": [{"
				"\"id\": %d,"
				"\"markdown\": \"%s\","
				"\"published_at\": \"%s\","
				"\"state\": \"%s\""
				"}]}",
			gpost->PostId(), JsonString::Escape(post->Page()).String(),
			post->PostMetadata()->GetItem("post_date")->Value().String(),
			post->PostMetadata()->GetItem("post_status")->Value().String()));
}


BlogPositivePost* GhostPlugin::CreateNewPost(BlogPositiveBlog* blog,
		const char* name)
{
	JsonObject* obj = _Request(blog, "/ghost/api/v0.1/posts/", "POST",
			BString().SetToFormat(
				"{\"posts\": [{\"title\": \"%s\", \"markdown\":\"\"}]}",
				JsonString::Escape(name).String()));
	if (!obj)
		return NULL;

	JsonArray* arr = dynamic_cast<JsonArray*>(obj->Get("posts"));
	JsonObject* post = dynamic_cast<JsonObject*>(arr->ItemAt(0));

	delete obj;
	return new GhostPost(post, blog);
}


void GhostPlugin::RemovePost(BlogPositivePost* post)
{
	GhostPost* gpost = static_cast<GhostPost*>(post);

	_Request(gpost->Blog(),
			BString("/ghost/api/v0.1/posts/") << gpost->PostId(), "DELETE");
}


void GhostPlugin::_DisplayErrors(JsonArray* arr)
{
	for (int i = 0; i < arr->CountItems(); i++) {
		JsonObject* obj = dynamic_cast<JsonObject*>(arr->ItemAt(i));
		BString* message = dynamic_cast<BString*>(obj->Get("message"));
		fprintf(stderr, "Ghost: %s\n", message->String());
		BAlert* alert = new BAlert("Ghost error", *message, ":(");
		alert->Go();
	}
}


class GCreateBlog : public BWindow {
public:
						GCreateBlog(BlogPositiveBlogListDelegate* dele,
								BlogPositiveBlogPlugin* pl);

	void				MessageReceived(BMessage* message);

	void				SetBlogHandler(int32 blogHandler);
	int32				BlogHandler();

private:
	int32				fBlogHandler;
	BTextControl*		fNameControl;
	BTextControl*		fEmailControl;
	BTextControl*		fPassControl;
	BTextControl*		fUrlControl;
	BlogPositiveBlogListDelegate* fDelegate;
};


GCreateBlog::GCreateBlog(BlogPositiveBlogListDelegate* dele,
	BlogPositiveBlogPlugin* pl)
	:
	BWindow(BRect(100, 100, 400, 230), B_TRANSLATE("Create Ghost blog"),
			B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	fDelegate = dele;

	fNameControl = new BTextControl("Name: ", "", NULL);
	fNameControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fEmailControl = new BTextControl("E-Mail: ", "", NULL);
	fEmailControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fPassControl = new BTextControl("Password: ", "", NULL);
	fPassControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fPassControl->TextView()->HideTyping(true);
	fUrlControl = new BTextControl("URL: ", "", NULL);
	fUrlControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);

	BButton* createButton =
		new BButton(B_TRANSLATE("Create"), new BMessage(kCreateBlog));
	BButton* cancelButton =
		new BButton(B_TRANSLATE("Cancel"), new BMessage(kCancelBlog));

	fNameControl->MakeFocus();
	SetDefaultButton(createButton);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGrid(2, 3)
			.Add(fNameControl->CreateLabelLayoutItem(), 0, 0)
			.Add(fNameControl->CreateTextViewLayoutItem(), 1, 0)
			.Add(fEmailControl->CreateLabelLayoutItem(), 0, 1)
			.Add(fEmailControl->CreateTextViewLayoutItem(), 1, 1)
			.Add(fPassControl->CreateLabelLayoutItem(), 0, 2)
			.Add(fPassControl->CreateTextViewLayoutItem(), 1, 2)
			.Add(fUrlControl->CreateLabelLayoutItem(), 0, 3)
			.Add(fUrlControl->CreateTextViewLayoutItem(), 1, 3)
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(cancelButton)
			.Add(createButton)
		.End();

	fBlogHandler = pl->MainHandler();;
}


void GCreateBlog::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case kCreateBlog:
		{
			BlogPositiveBlog* blog = new BlogPositiveBlog();
			blog->SetName(fNameControl->Text());

			BMessage* cfg = blog->Configuration();
			cfg->SetString("email", fEmailControl->Text());
			cfg->SetString("password", fPassControl->Text());
			cfg->SetString("blogUrl", fUrlControl->Text());
			blog->SetBlogHandler(BlogHandler());
			gBlogList->AddItem(blog);
			fDelegate->ReloadBlogs();
			Quit();
			break;
		}
		case kCancelBlog:
			Quit();
			break;
		default:
			BWindow::MessageReceived(message);
			break;
	}
}


void GCreateBlog::SetBlogHandler(int32 blogHandler)
{
	fBlogHandler = blogHandler;
}


int32 GCreateBlog::BlogHandler()
{
	return fBlogHandler;
}


void GhostPlugin::OpenNewBlogWindow(BlogPositiveBlogListDelegate* dele)
{
	(new GCreateBlog(dele, this))->Show();
}


uint32 GhostPlugin::Version()
{
	return 0;
}


const char* GhostPlugin::Name()
{
	return "Ghost";
}


uint32 GhostPlugin::MainHandler()
{
	return 'Ghst';
}


bool GhostPlugin::Supports(uint32 code)
{
	return code == MainHandler();
}
