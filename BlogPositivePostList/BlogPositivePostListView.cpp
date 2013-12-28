/*
* Copyright 2013 Puck Meerburg, puck@puckipedia.nl
* All rights reserved. Distributed under the terms of the MIT License.
*/


#include "BlogPositivePostListView.h"

#include <stdio.h>

#include <Catalog.h>
#include <GroupLayout.h>
#include <ListView.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Message.h>
#include <OS.h>
#include <Rect.h>
#include <ScrollView.h>
#include <StringItem.h>
#include <TextControl.h>
#include <TextView.h>
#include <Window.h>

#include "BlogPositiveBlog.h"
#include "BlogPositivePlugin.h"
#include "BlogPositivePost.h"
#include "BlogPositivePostEditorWindow.h"


const uint32 kPostListViewOpenPost = 'PLOP';
const uint32 kPostListViewRemovePost = 'PLRP';
const uint32 kPostListViewNewPost = 'PLNP';


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Post List View"


class PostItem : public BStringItem {
public:
								PostItem(BlogPositivePost* post);
	BlogPositivePost*			Post();
private:
	BlogPositivePost*			fPost;
};


PostItem::PostItem(BlogPositivePost* post)
	:
	BStringItem(post->Name()),
	fPost(post)
{
}


BlogPositivePost*
PostItem::Post()
{
	return fPost;
}


struct PluginAndWindowThing {
	BlogPositivePostListView*	window;
	BlogPositivePlugin*			plugin;
	BlogPositiveBlog*			blog;
	BListView*					listView;
};


int32
BlogPositivePostListView::_loadList(void* pointer)
{
	PluginAndWindowThing* thing = static_cast<PluginAndWindowThing*>(pointer);
	if (thing->listView->LockLooper()) {
		thing->listView->MakeEmpty();
		thing->listView->UnlockLooper();
	}
	PostList* list = thing->plugin->GetBlogPosts(thing->blog);
	if (thing->listView->LockLooper()) {
		for (int i = 0; i < list->CountItems(); i++) {
			BlogPositivePost* post = list->ItemAt(i);
			thing->listView->AddItem(new PostItem(post));
		}
		thing->listView->UnlockLooper();
	}
}


BlogPositivePostListView::BlogPositivePostListView(const char* name,
	BlogPositiveBlog* blog)
	:
	BView(name, 0)
{
	fBlog = blog;

	fListView = new BListView("ListView");
	fListView->SetInvocationMessage(new BMessage(kPostListViewOpenPost));

	BMenuBar* menu = new BMenuBar("MenuView");
	fAddPost = new BMenuItem(B_TRANSLATE("New Post"),
		new BMessage(kPostListViewNewPost));
	fRemovePost = new BMenuItem(B_TRANSLATE("Remove Post"),
		new BMessage(kPostListViewRemovePost));
	menu->AddItem(fAddPost);
	menu->AddItem(fRemovePost);

	Reload();

	SetLayout(new BGroupLayout(B_VERTICAL, 0));
	AddChild(menu);
	AddChild(new BScrollView("scrollview", fListView, 0, false, true));
}


void
BlogPositivePostListView::Reload()
{
	PluginAndWindowThing* thing = new PluginAndWindowThing();
	thing->window = this;
	thing->blog = fBlog;
	thing->listView = fListView;
	thing->plugin = fBlog->Plugin();

	thread_id readThread = spawn_thread(BlogPositivePostListView::_loadList,
		"load_posts", B_NORMAL_PRIORITY, thing);
	resume_thread(readThread);

}


class NewPostWindow : public BWindow
{
public:
								NewPostWindow(BlogPositivePostListView* view);
	void						MessageReceived(BMessage* aMessage);
private:
	BlogPositivePostListView*	fView;
	BTextControl*				fTextView;
};


NewPostWindow::NewPostWindow(BlogPositivePostListView* view)
	:
	BWindow(BRect(100, 100, 500, 600), "New Post",
		B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	fTextView = new BTextControl("TextView", "Title: ",
		"", new BMessage('BPFN'));
	SetLayout(new BGroupLayout(B_VERTICAL));
	AddChild(BLayoutBuilder::Group<>(B_VERTICAL, 0).Add(fTextView));
	fView = view;
}


void
NewPostWindow::MessageReceived(BMessage* aMessage)
{
	switch (aMessage->what) {
		case 'BPFN':
			aMessage->SetString("postTitle", fTextView->Text());
			BMessenger(fView).SendMessage(aMessage);
			Close();
			break;
		default:
			BWindow::MessageReceived(aMessage);
	}
}


void
BlogPositivePostListView::MessageReceived(BMessage* aMessage)
{
	switch (aMessage->what) {
		case 'BPFN':
		{
			BlogPositivePost* p = fBlog->Plugin()->CreateNewPost(fBlog,
				aMessage->GetString("postTitle", ""));
			if (p != NULL)
			{
				BlogPositivePostEditorWindow* window
					= new BlogPositivePostEditorWindow(p,
						BRect(100, 100, 500, 600));
				window->Show();
			}
			break;
		}
		case kPostListViewOpenPost:
		{
			if (aMessage->GetInt32("index", -1) != -1) {
				BlogPositivePost* post = static_cast<PostItem*>(
					fListView->ItemAt(aMessage->GetInt32("index", -1)))->Post();
				BlogPositivePostEditorWindow* window
					= new BlogPositivePostEditorWindow(post,
						BRect(100, 100, 500, 600));
				window->Show();
			}
			break;
		}
		case kPostListViewNewPost:
		{
			(new NewPostWindow(this))->Show();
			break;
		}
		case kPostListViewRemovePost:
		{
			if (fListView->CurrentSelection() != -1) {
				BlogPositivePost* post = static_cast<PostItem*>(
					fListView->ItemAt(fListView->CurrentSelection()))->Post();
				post->Blog()->Plugin()->RemovePost(post);
				Reload();
			}
			break;
		}
		default:
			BView::MessageReceived(aMessage);
			break;
	}
}


void
BlogPositivePostListView::AttachedToWindow()
{
	fAddPost->SetTarget(this);
	fListView->SetTarget(this);
	fRemovePost->SetTarget(this);
}
