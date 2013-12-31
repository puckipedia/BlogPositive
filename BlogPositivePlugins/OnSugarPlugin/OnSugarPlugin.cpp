/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "OnSugarPlugin.h"

#include <curl/curl.h>
#include <stdio.h>

#include <Alert.h>
#include <Catalog.h>
#include <ControlLook.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
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

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "OnSugar Plugin"

#if STANDALONE
MODULES {
	MODULE(OnSugarPlugin);	
}
#endif


size_t readIntoBString(void* bloc, size_t size, size_t nmemb, void* userp)
{
	char* charBloc = static_cast<char*>(bloc);
	const char* cBlock = const_cast<const char*>(charBloc);
	*(static_cast<BString*>(userp)) << cBlock;
	return nmemb;
}

class OSCreateBlog : public BWindow {
public:
							OSCreateBlog(BlogPositiveBlogListDelegate* dele,
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
	BlogPositiveBlogListDelegate*	fDelegate;
};

const uint32 kChooseBlog = 'CHBL';

OSCreateBlog::OSCreateBlog(BlogPositiveBlogListDelegate* dele,
	BlogPositiveBlogPlugin* pl)
	:
	BWindow(BRect(100, 100, 400, 230), B_TRANSLATE("Create OnSugar Blog"),
		B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	fDelegate = dele;
	
	fNameControl = new BTextControl("NameControl", B_TRANSLATE("Name: "), "", 
		NULL);
	fNameControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fUserControl = new BTextControl("UserControl", B_TRANSLATE("Username: "),
		"", NULL);
	fUserControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fPassControl = new BTextControl("PasswordControl",
		B_TRANSLATE("Password: "), "", NULL);
	fPassControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fPassControl->TextView()->HideTyping(true);
	fUrlControl = new BTextControl("UrlControl", B_TRANSLATE("Url: "),"", NULL);
	fUrlControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);

	SetLayout(new BGroupLayout(B_VERTICAL));
	fCreateButton = new BButton("createButton",
		B_TRANSLATE("Create"), new BMessage(kCreateBlog));
	BButton* cancelButton = new BButton("cancelButton",
		B_TRANSLATE("Cancel"), new BMessage(kCancelBlog));

	fCreateButton->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED,
		B_SIZE_UNLIMITED));
	cancelButton->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED));


	fNameControl->MakeFocus();
	fBlogHandler = pl->MainHandler();
	
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(be_control_look->DefaultItemSpacing()*0.5,
			be_control_look->DefaultItemSpacing()*0.5)
		.AddGrid(2, 3)
			.SetInsets(be_control_look->DefaultItemSpacing()*0.5,
				be_control_look->DefaultItemSpacing()*0.5)
			.Add(fNameControl->CreateLabelLayoutItem(), 0, 0)
			.Add(fNameControl->CreateTextViewLayoutItem(), 1, 0)
			.Add(fUserControl->CreateLabelLayoutItem(), 0, 1)
			.Add(fUserControl->CreateTextViewLayoutItem(), 1, 1)
			.Add(fPassControl->CreateLabelLayoutItem(), 0, 2)
			.Add(fPassControl->CreateTextViewLayoutItem(), 1, 2)
			.Add(fUrlControl->CreateLabelLayoutItem(), 0, 3)
			.Add(fUrlControl->CreateTextViewLayoutItem(), 1, 3)
			.End()
		.AddGroup(B_HORIZONTAL)
			.SetInsets(be_control_look->DefaultItemSpacing()*0.5,
				be_control_look->DefaultItemSpacing()*0.5)
			.Add(cancelButton)
			.Add(fCreateButton)
		.End();
	SetDefaultButton(fCreateButton);
}


void
OSCreateBlog::SetBlogHandler(int32 blogHandler)
{
	fBlogHandler = blogHandler;
}


void
OSCreateBlog::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case kCreateBlog:
		{
			BString auth;
			auth << fUserControl->Text() << "|||";
			auth << fPassControl->Text() << "|||";
			auth << fUrlControl->Text();
			BlogPositiveBlog* blog = new BlogPositiveBlog();
			blog->SetName(fNameControl->Text());
			blog->SetAuthentication(auth.String());
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


class OnSugarPost : public BlogPositivePost
{
public:
				OnSugarPost(BString aPostName, BString aPostContent,
					BString aPostId, BlogPositiveBlog* aBlog);
	const char*	PostId();
	void		SetPostId(const char* id);
private:
	BString fId;
};


OnSugarPost::OnSugarPost(BString postName, BString postContent,
	BString postId, BlogPositiveBlog* blog)
	:
	BlogPositivePost(blog)
{
	fName.SetTo(postName);
	fPage.SetTo(postContent);
	fId.SetTo(postId);
}


const char*
OnSugarPost::PostId()
{
	return fId.String();
}


void
OnSugarPost::SetPostId(const char* id)
{
	fId = id;
}


uint32
OnSugarPlugin::Version()
{
	return 0;
}


const char*
OnSugarPlugin::Name()
{
	return B_TRANSLATE("OnSugar blog");
}


uint32
OnSugarPlugin::MainHandler()
{
	return 'OnSg';
}


bool
OnSugarPlugin::Supports(uint32 Code)
{
	return Code == MainHandler();
}


void
OnSugarPlugin::GetAuthentication(BString Auth, BString* Username,
	BString* Password, BString* BlogUrl)
{
	int32 SeperatorLocation = Auth.FindFirst("|||");
	int32 XBLocation = Auth.FindLast("|||");
	Auth.CopyInto(*Username, 0, SeperatorLocation);

	Auth.CopyInto(*Password, SeperatorLocation + 3,
		XBLocation - SeperatorLocation - 3);

	Auth.CopyInto(*BlogUrl, XBLocation + 3, Auth.Length() - XBLocation - 3);
}


XmlNode*
OnSugarPlugin::Get(BString* responseString, BString apiEndpoint, BString Auth)
{

	BString username;
	BString password;
	BString blogUrl;
	GetAuthentication(Auth, &username, &password, &blogUrl);

	blogUrl << "/api" << apiEndpoint;

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, blogUrl.String());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, readIntoBString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(responseString));
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "BlogPositive/0.1 (OnSugar/0.1)");
	curl_easy_perform(curl);
	
	XmlNode* responseNode = new XmlNode(responseString->String(), NULL);

	return responseNode;
}


XmlNode*
OnSugarPlugin::Post(BString* responseString, BString apiEndpoint,
	BString requestString, BString Auth)
{

	BString username;
	BString password;
	BString blogUrl;
	GetAuthentication(Auth, &username, &password, &blogUrl);

	blogUrl << "/api" << apiEndpoint;
	
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, blogUrl.String());
	
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestString.String());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, readIntoBString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA,
		static_cast<void*>(responseString));
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "BlogPositive/0.1 (OnSugar/0.1)");
	curl_easy_perform(curl);
	
	XmlNode* responseNode = new XmlNode(responseString->String(), NULL);
	XmlNode* err = responseNode->FindChild("error", NULL, true);
	if (err != NULL) {
		BString message;
		message.SetToFormat(B_TRANSLATE(
			"Server: An error.\n"
			"Oops: \"%s\"\n"
			"May want to fix that!"), err->Value().String());
		BAlert* alert = new BAlert(B_TRANSLATE("Oops, an error!"),
			message.String(), ":(");
		alert->Go();
	}
	return responseNode;
}


PostList*
OnSugarPlugin::GetBlogPosts(BlogPositiveBlog* blog)
{
	BString response;
	XmlNode* responseNode = Get(&response, "/posts/xml", blog->Authentication());
	if (responseNode == NULL) {
		BAlert* alert = new BAlert(B_TRANSLATE("This isn't OnSugar!"),
			B_TRANSLATE("BlogPositive has:\n"
			"a very big disaster.\n"
			"The URL's wrong!"), ":(");
		alert->Go();
		return new PostList();
	}

	XmlNode* postsNode = responseNode->FindChild("posts", NULL, true);
	if (postsNode == NULL) {
		BAlert* alert = new BAlert(B_TRANSLATE("This isn't OnSugar!"),
			B_TRANSLATE("BlogPositive has:\n"
			"a very big disaster.\n"
			"The URL's wrong!"), ":(");
		alert->Go();
		return new PostList();
	}

	XmlNode* postNode = NULL;
	
	PostList* list = new PostList();
	
	while ((postNode = postsNode->FindChild("post", postNode)) != NULL) {
		BString Title = postNode->FindChild("title")->Value();
		BString Content = postNode->FindChild("body")->Value();
		BString Id = postNode->FindChild("id")->Value();
		OnSugarPost *post = new OnSugarPost(Title, Content, Id, blog);
		list->AddItem(post);
	}
	
	return list;
}


void
OnSugarPlugin::SavePost(BlogPositivePost* bppost)
{
	OnSugarPost* post = static_cast<OnSugarPost*>(bppost);
	
	if (strlen(post->PostId()) > 0) {
		BAlert* alert = new BAlert(B_TRANSLATE("Not Supported"),
			B_TRANSLATE("I am sorry,\n"
			"But you cannot save posts now.\n"
			"The API's fault!"), B_TRANSLATE("Create new post"), 
			B_TRANSLATE("Don't create new"),
			NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		int32 response = alert->Go();
		if (response == 1) {
			return;
		}
	}
	
	BString username;
	BString password;
	BString blogurl;
	GetAuthentication(post->Blog()->Authentication(), &username, &password,
		&blogurl);
	
	BString request = "login=";
	request << username << "&password=" << password << "&type=text";
	request << "&callback=xml&title=";
	request << post->Name() << "&body=";
	
	BString body = post->Page();
	body.ReplaceAll("%", "%25");
	body.ReplaceAll("&", "%26");
	body.ReplaceAll("?", "%3F");
	body.ReplaceAll("=", "%3D");
	
	request << body;
	
	BString response;
	XmlNode* respNode = Post(&response, "/posts/create",
		request, post->Blog()->Authentication());
	printf("Response: %s\n", response.String());
	XmlNode* idNode = respNode->FindChild("id", NULL, true);
	if (idNode == NULL) {
		delete respNode;
		return;
	}
	BString id = idNode->Value();
	post->SetPostId(id);

	delete respNode;
}


BlogPositivePost*
OnSugarPlugin::CreateNewPost(BlogPositiveBlog* blog, const char* name)
{
	return new OnSugarPost(name, "", "", blog);
}


void
OnSugarPlugin::RemovePost(BlogPositivePost* aPost)
{
	BAlert* alert = new BAlert(B_TRANSLATE("Can't remove"),
		B_TRANSLATE("I'm very sorry,\n"
		"The API can't remove\n"
		"Just use the website!"), B_TRANSLATE("OK"));
	alert->Go();
}


void
OnSugarPlugin::OpenNewBlogWindow(BlogPositiveBlogListDelegate* dele)
{
	(new OSCreateBlog(dele, this))->Show();
}
