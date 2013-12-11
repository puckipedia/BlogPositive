/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePlugin.h"

#include <Button.h>
#include <Catalog.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <List.h>
#include <Message.h>
#include <TextControl.h>
#include <Window.h>

#include "../BlogPositiveMain/BlogPositiveMainView.h"
#include "../BlogPositiveSettings.h"

const uint32 kCreateBlog = 'BPCB';
const uint32 kCancelBlog = 'BPKB';

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "BlogPositive Plugin"


class BlogPositiveCreateBlog : public BWindow {
public:
							BlogPositiveCreateBlog(BlogPositiveMainView* aView,
								BlogPositivePlugin* pl);
	void					SetBlogHandler(int32 blogHandler);
	void					MessageReceived(BMessage* message);
	int32					BlogHandler();
private:
	int32 					fBlogHandler;
	BTextControl*			fNameControl;
	BTextControl*			fAuthControl;
	BlogPositiveMainView*	fMainView;
};

BlogPositiveCreateBlog::BlogPositiveCreateBlog(BlogPositiveMainView* aView,
	BlogPositivePlugin* pl)
	:
	BWindow(BRect(100, 100, 400, 190), B_TRANSLATE("Create Blog"),
		B_DOCUMENT_WINDOW, 0)
{
	fNameControl = new BTextControl("NameControl", "Name: ",
		"", new BMessage('CBFA'));
	fAuthControl = new BTextControl("AuthControl", "Auth: ",
		"", new BMessage('CBNB'));

	SetLayout(new BGroupLayout(B_VERTICAL));
	BView* mainView = new BView("mainView", B_SUPPORTS_LAYOUT);
	mainView->SetLayout(new BGroupLayout(B_VERTICAL, 0));
	mainView->AddChild(fNameControl);
	mainView->AddChild(fAuthControl);

	BView* buttonView = new BView("buttonView", B_SUPPORTS_LAYOUT);
	buttonView->SetLayout(new BGroupLayout(B_HORIZONTAL));
	
	BButton* createButton = new BButton("ceateButton",
		B_TRANSLATE("Create"), new BMessage(kCreateBlog));
	BButton* cancelButton = new BButton("ceateButton",
		B_TRANSLATE("Cancel"), new BMessage(kCancelBlog));
	buttonView->AddChild(cancelButton);
	buttonView->AddChild(createButton);
	mainView->AddChild(buttonView);

	AddChild(mainView);

	fMainView = aView;

	fNameControl->MakeFocus();
	fBlogHandler = pl->MainHandler();

	mainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	buttonView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fNameControl->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fAuthControl->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	createButton->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	cancelButton->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}


void
BlogPositiveCreateBlog::SetBlogHandler(int32 blogHandler)
{
	fBlogHandler = blogHandler;
}


void
BlogPositiveCreateBlog::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case 'CBFA':
			fAuthControl->MakeFocus();
			break;
		case 'CBNB':
			// Fallthrough on purpose
		case kCreateBlog:
		{
			BlogPositiveSettings* settings = new BlogPositiveSettings("bloglist");
			BlogList* lis = BlogPositiveBlog::DeserializeList(settings, "blogs");
			BlogPositiveBlog* blog = new BlogPositiveBlog();
			blog->SetName(fNameControl->Text());
			blog->SetAuthentication(fAuthControl->Text());
			blog->SetBlogHandler(fBlogHandler);
			lis->AddItem(blog);
			if (fMainView->LockLooper())
			{
				fMainView->Reload(lis);
				fMainView->UnlockLooper();
			}
			BlogPositiveSettings::SaveOther(
				BlogPositiveBlog::SerializeList(lis, "blogs"), "bloglist");
			Hide();
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
BlogPositivePlugin::Version()
{
	return 0;
}


uint32
BlogPositivePlugin::MainHandler()
{
	return 'BACN';
}


const char*
BlogPositivePlugin::Name()
{
	return "Unknown";
}


int32
BlogPositivePlugin::Type()
{
	return kBlogPositiveBlogApi;
}


bool
BlogPositivePlugin::Supports(int32 Code)
{
	return false;
}


PostList*
BlogPositivePlugin::GetBlogPosts(BlogPositiveBlog* blog)
{
	return new PostList();
}


BlogPositivePost*
BlogPositivePlugin::CreateNewPost(BlogPositiveBlog* blog, const char* name)
{
	return NULL;
}


void
BlogPositivePlugin::RemovePost(BlogPositivePost* post)
{

}


void
BlogPositivePlugin::SavePost(BlogPositivePost* post)
{

}


void
BlogPositivePlugin::OpenNewBlogWindow(BlogPositiveMainView* mainView)
{
	(new BlogPositiveCreateBlog(mainView, this))->Show();
}
