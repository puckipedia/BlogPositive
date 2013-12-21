/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "LiveJournalPlugin.h"

#include <curl/curl.h>
#include <stdio.h>
#include <time.h>

#include <Alert.h>
#include <Catalog.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <PopUpMenu.h>
#include <String.h>
#include <TextControl.h>
#include <Window.h>

#include "BlogPositiveBlog.h"
#include "BlogPositivePost.h"
#include "BlogPositiveMainView.h"
#include "BlogPositiveSettings.h"
#include "xmlnode.h"
#include "XmlRpcWrapper.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "LiveJournal Plugin"

size_t JournalBString(void* bloc, size_t size, size_t nmemb, void* userp)
{
	char* charBloc = static_cast<char*>(bloc);
	const char* cBlock = const_cast<const char*>(charBloc);
	*(static_cast<BString*>(userp)) << cBlock;
	return nmemb;
}


class LJCreateBlog : public BWindow {
public:
							LJCreateBlog(BlogPositiveMainView* aView,
								BlogPositivePlugin* pl);
	void					SetBlogHandler(int32 blogHandler);
	void					MessageReceived(BMessage* message);
	int32					BlogHandler();
private:
	int32 					fBlogHandler;
	BButton*				fCreateButton;
	BTextControl*			fNameControl;
	BTextControl*			fUserControl;
	BTextControl*			fPassControl;
	BTextControl*			fJournalControl;
	BlogPositiveMainView*	fMainView;
};


LJCreateBlog::LJCreateBlog(BlogPositiveMainView* aView,
	BlogPositivePlugin* pl)
	:
	BWindow(BRect(100, 100, 400, 230), B_TRANSLATE("Create Blog"),
		B_MODAL_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	fMainView = aView;
	
	fNameControl = new BTextControl("NameControl", "Name: ", "", NULL);
	fNameControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fUserControl = new BTextControl("UserControl", "Username: ", "", NULL);
	fUserControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fPassControl = new BTextControl("PassControl", "Password: ", "", NULL);
	fPassControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fPassControl->TextView()->HideTyping(true);
	fJournalControl = new BTextControl("JournalControl", "Journal name: ","", NULL);
	fJournalControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);

	SetLayout(new BGroupLayout(B_VERTICAL));
	fCreateButton = new BButton("createButton",
		B_TRANSLATE("Create"), new BMessage(kCreateBlog));
	BButton* cancelButton = new BButton("cancelButton",
		B_TRANSLATE("Cancel"), new BMessage(kCancelBlog));
	fNameControl->MakeFocus();
	fBlogHandler = pl->MainHandler();
	
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.AddGrid(2, 3)
			.Add(fNameControl->CreateLabelLayoutItem(), 0, 0)
			.Add(fNameControl->CreateTextViewLayoutItem(), 1, 0)
			.Add(fUserControl->CreateLabelLayoutItem(), 0, 1)
			.Add(fUserControl->CreateTextViewLayoutItem(), 1, 1)
			.Add(fPassControl->CreateLabelLayoutItem(), 0, 2)
			.Add(fPassControl->CreateTextViewLayoutItem(), 1, 2)
			.Add(fJournalControl->CreateLabelLayoutItem(), 0, 3)
			.Add(fJournalControl->CreateTextViewLayoutItem(), 1, 3)
			.End()
		.AddGroup(B_HORIZONTAL)
			.Add(cancelButton)
			.Add(fCreateButton)
		.End();
	SetDefaultButton(fCreateButton);
}


void
LJCreateBlog::SetBlogHandler(int32 blogHandler)
{
	fBlogHandler = blogHandler;
}


void
LJCreateBlog::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case kCreateBlog:
		{
			BString auth;
			auth << fUserControl->Text() << "|||";
			auth << fPassControl->Text() << "|||";
			auth << fJournalControl->Text();

			BlogPositiveSettings* settings = new BlogPositiveSettings("bloglist");
			BlogList* lis = BlogPositiveBlog::DeserializeList(settings, "blogs");
			BlogPositiveBlog* blog = new BlogPositiveBlog();
			blog->SetName(fNameControl->Text());
			blog->SetAuthentication(auth.String());
			blog->SetBlogHandler(fBlogHandler);
			lis->AddItem(blog);
			if (fMainView->LockLooper())
			{
				fMainView->Reload(lis);
				fMainView->UnlockLooper();
			}
			BlogPositiveSettings::SaveOther(
				BlogPositiveBlog::SerializeList(lis, "blogs"), "bloglist");
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


void
LiveJournalPlugin::GetAuthentication(BString Auth, BString* Username,
	BString* Password, BString* journal)
{
	int32 SeperatorLocation = Auth.FindFirst("|||");
	int32 XBLocation = Auth.FindLast("|||");
	Auth.CopyInto(*Username, 0, SeperatorLocation);

	Auth.CopyInto(*Password, SeperatorLocation + 3,
		XBLocation - SeperatorLocation - 3);

	Auth.CopyInto(*journal, XBLocation + 3, Auth.Length() - 3);
}


uint32
LiveJournalPlugin::Version()
{
	return 0;
}


const char*
LiveJournalPlugin::Name()
{
	return B_TRANSLATE("LiveJournal blog");
}


int32
LiveJournalPlugin::Type()
{
	return kBlogPositiveBlogApi;
}


uint32
LiveJournalPlugin::MainHandler()
{
	return 'LiJo';
}


bool
LiveJournalPlugin::Supports(int32 Code)
{
	return Code == 'LiJo';
}


class LiveJournalPost : public BlogPositivePost
{
public:
				LiveJournalPost(BString aPostName, BString aPostContent,
					BString aPostId, BlogPositiveBlog* aBlog);
	const char*	PostId();
	void		SetPostId(const char* postid);
private:
	BString fId;
};


LiveJournalPost::LiveJournalPost(BString postName, BString postContent,
	BString postId, BlogPositiveBlog* blog)
	:
	BlogPositivePost(blog)
{
	fName.SetTo(postName);
	fPage.SetTo(postContent);
	fId.SetTo(postId);
}


const char*
LiveJournalPost::PostId()
{
	return fId.String();
}


void
LiveJournalPost::SetPostId(const char* postid)
{
	fId = postid;
}


void LiveJournalShowError(XmlNode* response, long responseCode)
{
	const char* errorMessageTitle = "";
	const char* errorMessageContent = "";
	XmlNode* node;
	switch (responseCode) {
		default:
			node = response->FindChild("string", NULL, true);
			if(node != NULL) {
				errorMessageTitle = B_TRANSLATE("LiveJournal error");
				BString* errorString = new BString();
				const char* error = B_TRANSLATE_COMMENT("This: Not a haiku\n"
					"But LiveJournal had an error:\n"
					"%s", "%s will be the livejournal returned error");
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
LiveJournalPlugin::Request(XmlRpcRequest* r, BString* responseString,
	BString Auth)
{
	BString dataString(r->GetData());
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "http://livejournal.com/interface/xmlrpc");

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataString.String());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, JournalBString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(responseString));
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "BlogPositive/0.1");
	curl_easy_perform(curl);
	
	XmlNode* responseNode = new XmlNode(responseString->String(), NULL);
	
	if(responseNode->FindChild("fault", NULL, true) != NULL)
	{
		long responseCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
		LiveJournalShowError(responseNode, responseCode);
	}
	return responseNode;
}


PostList*
LiveJournalPlugin::GetBlogPosts(BlogPositiveBlog* aBlog)
{
	XmlRpcRequest r;

	BString username;
	BString password;
	BString journal;
	GetAuthentication(aBlog->Authentication(), &username, &password,
		&journal);

	r.SetMethodName("LJ.XMLRPC.getevents");
	XmlStruct requestStruct;
	requestStruct.AddItem("username", username);
	requestStruct.AddItem("password", password);
	requestStruct.AddItem("ver", "1");
	requestStruct.AddItem("selecttype", "lastn");
	requestStruct.AddItem("usejournal", journal);
	requestStruct.AddItem(new XmlNameValuePair("noprops", new XmlValue("1", "boolean")));
	requestStruct.AddItem("lineendings", "unix");

	r.AddItem(&requestStruct);

	BString responseString;
	XmlNode* responseNode
		= Request(&r, &responseString, aBlog->Authentication());

	XmlArray* responseArray = ParseResponse(responseNode);
	BString derp;
	responseArray->PushContent(&derp);
	XmlStruct* responseStruct = static_cast<XmlStruct*>(responseArray->ItemAt(0));
	XmlArray* blogPosts = static_cast<XmlArray*>(responseStruct->Get("events"));
	PostList* postList = new PostList();
	for (int i = 0; i < blogPosts->Items(); i++) {
		XmlStruct* post = static_cast<XmlStruct*>(blogPosts->ItemAt(i));
		BString subject = "---No Subject---";
		if(post->Get("subject") != NULL)
			subject = static_cast<XmlValue*>(post->Get("subject"))->Value();
		BString text = static_cast<XmlValue*>(post->Get("event"))->Value();
		BString id = static_cast<XmlValue*>(post->Get("itemid"))->Value();
		LiveJournalPost* ljPost = new LiveJournalPost(subject, text, id, aBlog);
		postList->AddItem(ljPost);
	}
	
	return postList;
}


void
LiveJournalPlugin::SavePost(BlogPositivePost* post)
{
	LiveJournalPost* ljPost = static_cast<LiveJournalPost*>(post);
	XmlRpcRequest r;

	BString username;
	BString password;
	BString journal;
	GetAuthentication(post->Blog()->Authentication(), &username, &password,
		&journal);

	r.SetMethodName("LJ.XMLRPC.editevent");
	XmlStruct requestStruct;
	requestStruct.AddItem("username", username);
	requestStruct.AddItem("password", password);
	requestStruct.AddItem("ver", "1");
	requestStruct.AddItem("itemid", ljPost->PostId());
	requestStruct.AddItem("event", ljPost->Page());
	if(strcmp(ljPost->Name(), "---No Subject---") != 0)
		requestStruct.AddItem("subject", ljPost->Name());
	requestStruct.AddItem("lineendings", "unix");

	r.AddItem(&requestStruct);

	BString responseString;
	XmlNode* responseNode
		= Request(&r, &responseString, post->Blog()->Authentication());
	XmlArray* responseArray = ParseResponse(responseNode);
	XmlStruct* str = static_cast<XmlStruct*>(responseArray->ItemAt(0));
	
	ljPost->SetPostId(static_cast<XmlValue*>(str->Get("itemid"))->Value());
}


void
LiveJournalPlugin::RemovePost(BlogPositivePost* post)
{
	LiveJournalPost* ljPost = static_cast<LiveJournalPost*>(post);
	XmlRpcRequest r;

	BString username;
	BString password;
	BString journal;
	GetAuthentication(post->Blog()->Authentication(), &username, &password,
		&journal);

	r.SetMethodName("LJ.XMLRPC.editevent");
	XmlStruct requestStruct;
	requestStruct.AddItem("username", username);
	requestStruct.AddItem("password", password);
	requestStruct.AddItem("ver", "1");
	requestStruct.AddItem("itemid", ljPost->PostId());
	requestStruct.AddItem("event", "");
	requestStruct.AddItem("subject", "");
	requestStruct.AddItem("lineendings", "unix");

	r.AddItem(&requestStruct);

	BString responseString;
	XmlNode* responseNode
		= Request(&r, &responseString, post->Blog()->Authentication());
}


BlogPositivePost*
LiveJournalPlugin::CreateNewPost(BlogPositiveBlog* aBlog, const char* aName)
{
	XmlRpcRequest r;

	BString username;
	BString password;
	BString journal;
	GetAuthentication(aBlog->Authentication(), &username, &password,
		&journal);

	r.SetMethodName("LJ.XMLRPC.postevent");
	XmlStruct requestStruct;
	requestStruct.AddItem("username", username);
	requestStruct.AddItem("password", password);
	requestStruct.AddItem("ver", "1");
	requestStruct.AddItem("event", "-");
	requestStruct.AddItem("subject", aName);
	requestStruct.AddItem("lineendings", "unix");
	time_t ti = time(NULL);
	struct tm* tim = localtime(&ti);
	requestStruct.AddItem(new XmlNameValuePair("year",
		new XmlValue(1900+tim->tm_year)));
	requestStruct.AddItem(new XmlNameValuePair("mon",
		new XmlValue(1+tim->tm_mon)));
	requestStruct.AddItem(new XmlNameValuePair("day",
		new XmlValue(tim->tm_mday)));
	requestStruct.AddItem(new XmlNameValuePair("hour",
		new XmlValue(tim->tm_hour)));
	requestStruct.AddItem(new XmlNameValuePair("min",
		new XmlValue(1+tim->tm_min)));

	r.AddItem(&requestStruct);

	BString responseString;
	XmlNode* responseNode
		= Request(&r, &responseString, aBlog->Authentication());
	XmlArray* responseArray = ParseResponse(responseNode);
	XmlStruct* str = static_cast<XmlStruct*>(responseArray->ItemAt(0));
	LiveJournalPost* ljPost = new LiveJournalPost(aName, "-", "", aBlog);
	ljPost->SetPostId(static_cast<XmlValue*>(str->Get("itemid"))->Value());
	return ljPost;
}


void
LiveJournalPlugin::OpenNewBlogWindow(BlogPositiveMainView* mainView)
{
	(new LJCreateBlog(mainView, this))->Show();
}

