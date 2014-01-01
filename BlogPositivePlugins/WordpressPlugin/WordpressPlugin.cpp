/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "WordpressPlugin.h"

#include <curl/curl.h>
#include <stdio.h>

#include <Alert.h>
#include <Catalog.h>
#include <ControlLook.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <PopUpMenu.h>
#include <Size.h>
#include <String.h>
#include <TextControl.h>
#include <Window.h>

#include "BlogPositiveBlog.h"
#include "BlogPositiveDelegate.h"
#include "BlogPositivePost.h"
#include "BlogPositiveSettings.h"
#include "PluginStub.h"
#include "xmlnode.h"
#include "XmlRpcWrapper.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Wordpress Plugin"

#if STANDALONE
MODULES {
	MODULE(WordpressPlugin);
}
#endif

size_t WriteTobString(void* bloc, size_t size, size_t nmemb, void* userp)
{
	char* charBloc = static_cast<char*>(bloc);
	const char* cBlock = const_cast<const char*>(charBloc);
	*(static_cast<BString*>(userp)) << cBlock;
	return nmemb;
}


BlogList*
GetAllBlogs(BString userName, BString password, BString xmlrpcurl)
{
	if(xmlrpcurl.FindFirst("xmlrpc.php") == B_ERROR) {
		xmlrpcurl << "/xmlrpc.php";
	}
	XmlRpcRequest r;
	r.SetMethodName("wp.getUsersBlogs");
	r.AddItem(new XmlValue(userName));
	r.AddItem(new XmlValue(password));
	
	BString dataString(r.GetData());
	CURL* curl = curl_easy_init();
	BString responseString;
	curl_easy_setopt(curl, CURLOPT_URL, xmlrpcurl.String());

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataString.String());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteTobString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(&responseString));
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_perform(curl);

	XmlNode* responseNode = new XmlNode(responseString.String(), NULL);
	XmlNode* blogNode = NULL;
	BlogList* blogList = new BlogList();
	if(responseNode->FindChild("param", NULL, true) == NULL)
		return blogList;
	XmlNode* blogListNode = responseNode->FindChild("param", NULL, true)
		->FindChild("array", NULL, true)->FindChild("data", NULL, true);

	while ((blogNode = blogListNode->FindChild("value", blogNode)) != NULL)
	{
		BString blogId;
		BString blogName;
		BString xmlrpcUrl;
		XmlNode* firstStructNode = blogNode->FindChild("struct", NULL, true);
		for (int i = 0; i < firstStructNode->Children(); i++)
		{
			XmlNode* node = firstStructNode->ItemAt(i);
			BString name = node->FindChild("name")->Value();
			if (name == "blogid")
			{
				blogId = node->FindChild("string", NULL, true)->Value();
			}
			if (name == "blogName")
			{
				blogName = node->FindChild("string", NULL, true)->Value();
			}
			if (name == "xmlrpc")
			{
				xmlrpcUrl = node->FindChild("string", NULL, true)->Value();
			}
		}
		BlogPositiveBlog* blog = new BlogPositiveBlog();
		blog->SetName(blogName);
		BMessage* message = blog->Configuration();
		message->AddString("username", userName);
		message->AddString("password", password);
		message->AddString("xmlrpcurl", xmlrpcUrl);
		message->AddString("blogid", blogId);
		blogList->AddItem(blog);
	}
	return blogList;
}


class WPCreateBlog : public BWindow {
public:
							WPCreateBlog(BlogPositiveBlogListDelegate* dele,
								BlogPositiveBlogPlugin* pl);
	void					SetBlogHandler(int32 blogHandler);
	void					MessageReceived(BMessage* message);
	int32					BlogHandler();
private:
	int32 						fBlogHandler;
	BButton*					fCreateButton;
	BTextControl*				fNameControl;
	BTextControl*				fUserControl;
	BTextControl*				fPassControl;
	BTextControl*				fUrlControl;
	BTextControl*				fBlogIdControl;
	BlogPositiveBlogListDelegate*	fDelegate;
};

const uint32 kChooseBlog = 'CHBL';

WPCreateBlog::WPCreateBlog(BlogPositiveBlogListDelegate* dele,
	BlogPositiveBlogPlugin* pl)
	:
	BWindow(BRect(100, 100, 400, 230), B_TRANSLATE("Create Wordpress Blog"),
		B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	fDelegate = dele;
	
	fNameControl = new BTextControl("NameControl", "Name: ", "", NULL);
	fNameControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fUserControl = new BTextControl("UserControl", "Username: ", "", NULL);
	fUserControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fPassControl = new BTextControl("UserControl", "Password: ", "", NULL);
	fPassControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fPassControl->TextView()->HideTyping(true);
	fUrlControl = new BTextControl("UserControl", "Url: ","", NULL);
	fUrlControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fBlogIdControl = new BTextControl("UserControl", "Blog ID: ", "", NULL);
	fBlogIdControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);

	SetLayout(new BGroupLayout(B_VERTICAL));
	fCreateButton = new BButton("createButton",
		B_TRANSLATE("Create"), new BMessage(kCreateBlog));
	BButton* cancelButton = new BButton("cancelButton",
		B_TRANSLATE("Cancel"), new BMessage(kCancelBlog));
	BButton* getBlogsButton = new BButton("chooseButton",
		B_TRANSLATE("List"), new BMessage(kChooseBlog));

	fCreateButton->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED));
	cancelButton->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED));
	getBlogsButton->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED));

	fNameControl->MakeFocus();
	fBlogHandler = pl->MainHandler();
	
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(be_control_look->DefaultItemSpacing()*0.5,
			be_control_look->DefaultItemSpacing()*0.5)
		.AddGrid(3, 4)
			.SetInsets(be_control_look->DefaultItemSpacing()*0.5,
				be_control_look->DefaultItemSpacing()*0.5)
			.Add(fNameControl->CreateLabelLayoutItem(), 0, 0)
			.Add(fNameControl->CreateTextViewLayoutItem(), 1, 0, 2)
			.Add(fUserControl->CreateLabelLayoutItem(), 0, 1)
			.Add(fUserControl->CreateTextViewLayoutItem(), 1, 1, 2)
			.Add(fPassControl->CreateLabelLayoutItem(), 0, 2)
			.Add(fPassControl->CreateTextViewLayoutItem(), 1, 2, 2)
			.Add(fUrlControl->CreateLabelLayoutItem(), 0, 3)
			.Add(fUrlControl->CreateTextViewLayoutItem(), 1, 3, 2)
			.Add(fBlogIdControl->CreateLabelLayoutItem(), 0, 4)
			.Add(fBlogIdControl->CreateTextViewLayoutItem(), 1, 4)
			.Add(getBlogsButton, 2, 4)
			.End()
		.AddGroup(B_HORIZONTAL)
			.SetInsets(be_control_look->DefaultItemSpacing()*0.25,
				be_control_look->DefaultItemSpacing()*0.25)
			.Add(cancelButton)
			.Add(fCreateButton)
		.End();
	SetDefaultButton(getBlogsButton);
}


void
WPCreateBlog::SetBlogHandler(int32 blogHandler)
{
	fBlogHandler = blogHandler;
}


void
WPCreateBlog::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case kChooseBlog:
		{
			SetDefaultButton(fCreateButton);
			BPoint point;
			uint32 modifiers;
			fUserControl->GetMouse(&point, &modifiers);
			fUserControl->ConvertToScreen(&point);
			
			BPopUpMenu* menu = new BPopUpMenu("choiceMenu");
			BlogList* list = GetAllBlogs(fUserControl->Text(), fPassControl->Text(), fUrlControl->Text());

			if(list->CountItems() == 1) {
				delete menu;
				BlogPositiveBlog* blog = list->ItemAt(0);
				fBlogIdControl->SetText(blog->Configuration()->GetString("blogid", ""));
				delete list;
				break;
			}

			for(int i = 0; i < list->CountItems(); i++) {
				BlogPositiveBlog* blog = list->ItemAt(i);
				BMessage* choseMessage = new BMessage('ABCD');
				choseMessage->AddInt32("num", i);
				BMenuItem* item = new BMenuItem(blog->Name(), choseMessage);
				menu->AddItem(item);
			}
			BMenuItem* choseni = menu->Go(point, false, true);
			BMessage* chosen = NULL;
			if (choseni)
				chosen = choseni->Message();
			if(chosen) {
				BlogPositiveBlog* blog = list->ItemAt(chosen->GetInt32("num", -1));
				fBlogIdControl->SetText(blog->Configuration()->GetString("blogid", ""));
			}
			break;
		}
		case kCreateBlog:
		{
			BString xmlrpcurl = fUrlControl->Text();
			if(strstr(fUrlControl->Text(), "xmlrpc.php") == NULL) {
				xmlrpcurl << "/xmlrpc.php";
			}

			BlogPositiveBlog* blog = new BlogPositiveBlog();
			
			blog->SetName(fNameControl->Text());
			
			BMessage* message = blog->Configuration();
			message->AddString("username", fUserControl->Text());
			message->AddString("password", fPassControl->Text());
			message->AddString("blogid", fBlogIdControl->Text());
			message->AddString("xmlrpcurl", xmlrpcurl);

			blog->SetBlogHandler(fBlogHandler);

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


uint32
WordpressPlugin::Version()
{
	return 0;
}


const char*
WordpressPlugin::Name()
{
	return B_TRANSLATE("Wordpress.com or self-hosted");
}


uint32
WordpressPlugin::MainHandler()
{
	return 'WoPr';
}


bool
WordpressPlugin::Supports(uint32 Code)
{
	return Code == MainHandler();
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
	BlogPositivePost(blog)
{
	fName.SetTo(postName);
	fPage.SetTo(postContent);
	fId.SetTo(postId);
}


const char*
WordpressPost::PostId()
{
	return fId.String();
}


void ShowError(XmlNode* response, long responseCode)
{
	const char* errorMessageTitle = "";
	const char* errorMessageContent = "";
	XmlNode* node;
	switch (responseCode) {
		case 404:
			errorMessageTitle = B_TRANSLATE("404 - Blog not found");
			errorMessageContent = B_TRANSLATE("Time goes on and on:\n"
				"The blog you seek is not here\n"
				"Maybe a typo?");
			break;
		case 500:
			errorMessageTitle = B_TRANSLATE("500 - An error occured");
			errorMessageContent = B_TRANSLATE("Sometimes things go wrong:\n"
				"Maybe the server is down,\n"
				"Maybe it needs help.");
			break;
		default:
			node = response->FindChild("string", NULL, true);
			if(node != NULL) {
				errorMessageTitle = B_TRANSLATE("Wordpress error");
				BString* errorString = new BString();
				const char* error = B_TRANSLATE_COMMENT("This: Not a haiku\n"
					"But WordPress had an error:\n"
					"%s", "%s will be the wordpress returned error");
				errorString->SetToFormat(error, node->Value().String());
				errorMessageContent = errorString->String();
				
			} else {
				errorMessageTitle = B_TRANSLATE("??? - Unknown error");
				errorMessageContent = B_TRANSLATE("Errors do occur\n"
					"But this is very special\n"
					"Please do try again!");
			}
	}
				printf("--%s--\n%s\n", errorMessageTitle, errorMessageContent);
	BAlert* alertBox = new BAlert(errorMessageTitle, errorMessageContent,
		":(", NULL, NULL, B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_STOP_ALERT);
	alertBox->Go();
}


XmlNode*
WordpressPlugin::Request(XmlRpcRequest* r, BString* responseString,
	BlogPositiveBlog* blog)
{
	BString xmlrpcurl = blog->Configuration()->GetString("xmlrpcurl", "");
	BString dataString(r->GetData());
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, xmlrpcurl.String());

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataString.String());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteTobString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(responseString));
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_perform(curl);
	
	XmlNode* responseNode = new XmlNode(responseString->String(), NULL);
	if(responseNode->FindChild("fault", NULL, true) != NULL)
	{
		long responseCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
		ShowError(responseNode, responseCode);
	}
	return responseNode;
}


PostList*
WordpressPlugin::GetBlogPosts(BlogPositiveBlog* aBlog)
{
	XmlRpcRequest r;

	BString username = aBlog->Configuration()->GetString("username", "");
	BString password = aBlog->Configuration()->GetString("password", "");
	BString blogId = aBlog->Configuration()->GetString("blogid", "");

	r.SetMethodName("wp.getPosts");
	r.AddItem(new XmlValue(blogId));
	r.AddItem(new XmlValue(username));
	r.AddItem(new XmlValue(password));

	BString responseString;
	XmlNode* responseNode
		= Request(&r, &responseString, aBlog);

	XmlNode* postNode = NULL;
	if (responseNode->FindChild("param", NULL, true) == NULL) {
			return new PostList();
	}
	XmlNode* postListNode = responseNode->FindChild("param", NULL, true)
		->FindChild("array", NULL, true)->FindChild("data", NULL, true);
	PostList* postList = new PostList();

	while ((postNode = postListNode->FindChild("value", postNode)) != NULL)
	{
		BString aPostName;
		BString aPostContent;
		BString aPostId;
		BString aPostStatus;
		
		XmlNode* firstStructNode = postNode->FindChild("struct", NULL, true);
		for (int i = 0; i < firstStructNode->Children(); i++)
		{
			XmlNode* node = firstStructNode->ItemAt(i);
			BString name = node->FindChild("name")->Value();
			if (name == "post_title")
			{
				aPostName = node->FindChild("string", NULL, true)->Value();
			}
			if (name == "post_content")
			{
				aPostContent = node->FindChild("string", NULL, true)->Value();
			}
			if (name == "post_id")
			{
				aPostId = node->FindChild("string", NULL, true)->Value();
			}
			if (name == "post_status")
			{
				aPostStatus = node->FindChild("string", NULL, true)->Value();
			}
			
		}
		WordpressPost* aPost = new WordpressPost(aPostName, aPostContent,
			aPostId, aBlog);
		aPost->PostMetadata()->SetItem("post_status", new MetadataItem("Post status: ", aPostStatus));
		postList->AddItem(aPost);
	}
	return postList;
}


void
WordpressPlugin::RemovePost(BlogPositivePost* aPost)
{
	BlogPositiveBlog* aBlog = aPost->Blog();
	WordpressPost* wpPost = static_cast<WordpressPost*>(aPost);

	BString username = aBlog->Configuration()->GetString("username", "");
	BString password = aBlog->Configuration()->GetString("password", "");
	BString blogId = aBlog->Configuration()->GetString("blogid", "");
	BString xmlrpcurl = aBlog->Configuration()->GetString("xmlrpcurl", "");

	XmlRpcRequest* request = new XmlRpcRequest();
	request->SetMethodName("wp.deletePost");
	request->AddItem(new XmlValue(blogId));
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
	WordpressPost* wpPost = static_cast<WordpressPost*>(aPost);

	BString username = aBlog->Configuration()->GetString("username", "");
	BString password = aBlog->Configuration()->GetString("password", "");
	BString blogId = aBlog->Configuration()->GetString("blogid", "");
	BString xmlrpcurl = aBlog->Configuration()->GetString("xmlrpcurl", "");

	XmlRpcRequest* request = new XmlRpcRequest();
	request->SetMethodName("wp.editPost");
	request->AddItem(new XmlValue(blogId));
	request->AddItem(new XmlValue(username));
	request->AddItem(new XmlValue(password));
	request->AddItem(new XmlValue(wpPost->PostId(), "int"));

	XmlStruct* struc = new XmlStruct();
	XmlNameValuePair* p = new XmlNameValuePair("post_content", wpPost->Page());
	struc->AddItem(p);
	p = new XmlNameValuePair("post_status", wpPost->PostMetadata()->GetItem("post_status")->Value());
	struc->AddItem(p);
	p = new XmlNameValuePair("post_title", wpPost->Name());
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
	BString username = aBlog->Configuration()->GetString("username", "");
	BString password = aBlog->Configuration()->GetString("password", "");
	BString blogId = aBlog->Configuration()->GetString("blogid", "");
	BString xmlrpcurl = aBlog->Configuration()->GetString("xmlrpcurl", "");

	XmlRpcRequest* request = new XmlRpcRequest();
	request->SetMethodName("wp.newPost");
	request->AddItem(new XmlValue(1));
	request->AddItem(new XmlValue(username));
	request->AddItem(new XmlValue(password));
	XmlStruct* strut = new XmlStruct();
	strut->AddItem(new XmlNameValuePair("post_status",
		new XmlValue("draft")));
	strut->AddItem(new XmlNameValuePair("post_title", new XmlValue(aName)));
	request->AddItem(strut);

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, xmlrpcurl.String());

	BString responseString;
	BString requestString = request->GetData();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestString.String());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteTobString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(&responseString));
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_perform(curl);

	delete request;
	XmlNode* responseNode = new XmlNode(responseString.String(), NULL);
	XmlNode* rstring = responseNode->FindChild("string", NULL, true);
	if(rstring == NULL) {
		long responseCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
		ShowError(responseNode, responseCode);
	}
	if (rstring != NULL) {
		WordpressPost* post = new WordpressPost(aName, "",
			rstring->Value(), aBlog);
		post->PostMetadata()->SetItem("post_status", new MetadataItem("Post status:", "draft"));
		delete rstring;
		delete responseNode;
		return post;
	}
	delete rstring;
	delete responseNode;
	return NULL;
}


void
WordpressPlugin::OpenNewBlogWindow(BlogPositiveBlogListDelegate* dele)
{
	(new WPCreateBlog(dele, this))->Show();
}
