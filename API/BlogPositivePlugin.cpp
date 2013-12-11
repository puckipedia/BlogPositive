/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePlugin.h"

#include <TextControl.h>
#include <Message.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <Window.h>
#include <List.h>

#include "../BlogPositiveMain/BlogPositiveMainView.h"
#include "../BlogPositiveSettings.h"


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
	BWindow(BRect(100, 100, 400, 190), "Create Blog",
		B_MODAL_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0)
{
	fNameControl = new BTextControl("NameControl", "Name: ",
		"", new BMessage('CBFA'));
	fAuthControl = new BTextControl("AuthControl", "Auth: ",
		"", new BMessage('CBNB'));

	SetLayout(new BGroupLayout(B_VERTICAL));
	AddChild(fNameControl);
	AddChild(fAuthControl);

	fMainView = aView;

	fNameControl->MakeFocus();
	fBlogHandler = pl->MainHandler();
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
