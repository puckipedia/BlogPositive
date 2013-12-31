/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePlugin.h"
#include "BlogPositiveBlogPlugin.h"

#include <Button.h>
#include <Catalog.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <List.h>
#include <Message.h>
#include <TextControl.h>
#include <Window.h>

#include "BlogPositiveBlog.h"
#include "BlogPositiveDelegate.h"
#include "BlogPositiveSettings.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "BlogPositive Plugin"


class BlogPositiveCreateBlog : public BWindow {
public:
							BlogPositiveCreateBlog(BlogPositiveBlogListDelegate* aView,
								BlogPositiveBlogPlugin* pl);
	void					SetBlogHandler(int32 blogHandler);
	void					MessageReceived(BMessage* message);
	int32					BlogHandler();
private:
	int32 					fBlogHandler;
	BTextControl*			fNameControl;
	BTextControl*			fAuthControl;
	BlogPositiveBlogListDelegate*	fDelegate;
};

BlogPositiveCreateBlog::BlogPositiveCreateBlog(BlogPositiveBlogListDelegate* aView,
	BlogPositiveBlogPlugin* pl)
	:
	BWindow(BRect(100, 100, 400, 190), B_TRANSLATE("Create Blog"),
		B_MODAL_WINDOW, B_NOT_RESIZABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{
	fNameControl = new BTextControl("NameControl", "Name: ",
		"", new BMessage('CBFA'));
	fNameControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);

	fAuthControl = new BTextControl("AuthControl", "Auth: ",
		"", new BMessage('CBNB'));
	fAuthControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);

	BButton* createButton = new BButton("createButton",
		B_TRANSLATE("Create"), new BMessage(kCreateBlog));
	BButton* cancelButton = new BButton("cancelButton",
		B_TRANSLATE("Cancel"), new BMessage(kCancelBlog));

	fDelegate = aView;

	fNameControl->MakeFocus();
	fBlogHandler = pl->MainHandler();

	BLayoutBuilder::Grid<>(this, 2, 2)
		.Add(fNameControl->CreateLabelLayoutItem(), 0, 0)
		.Add(fNameControl->CreateTextViewLayoutItem(), 1, 0)
		.AddGroup(B_HORIZONTAL, 4, 2, 0, 1)
			.Add(createButton)
			.Add(cancelButton)
		.End();
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
			BlogPositiveBlog* blog = new BlogPositiveBlog();
			blog->SetName(fNameControl->Text());
			blog->SetBlogHandler(fBlogHandler);
			gBlogList->AddItem(blog);
			fDelegate->ReloadBlogs();
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


const char*
BlogPositivePlugin::Name()
{
	return "Unknown";
}


uint32
BlogPositiveBlogPlugin::MainHandler()
{
	return 'BACN';
}


bool
BlogPositiveBlogPlugin::Supports(uint32 Code)
{
	return false;
}


PostList*
BlogPositiveBlogPlugin::GetBlogPosts(BlogPositiveBlog* blog)
{
	return new PostList();
}


BlogPositivePost*
BlogPositiveBlogPlugin::CreateNewPost(BlogPositiveBlog* blog, const char* name)
{
	return NULL;
}


void
BlogPositiveBlogPlugin::RemovePost(BlogPositivePost* post)
{

}


void
BlogPositiveBlogPlugin::SavePost(BlogPositivePost* post)
{

}


void
BlogPositiveBlogPlugin::OpenNewBlogWindow(BlogPositiveBlogListDelegate* blogDelegate)
{
	(new BlogPositiveCreateBlog(blogDelegate, this))->Show();
}
