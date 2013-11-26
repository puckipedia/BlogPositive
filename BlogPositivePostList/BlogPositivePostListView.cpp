/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "BlogPositivePostListView.h"

#include <OS.h>
#include <StringItem.h>
#include <ListView.h>

#include "../API/BlogPositiveBlog.h"
#include "../API/BlogPositivePost.h"
#include "../API/BlogPositivePlugin.h"
#include "../BlogPositivePostEditor/BlogPositivePostEditorWindow.h"

#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <ScrollView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <Window.h>
#include <Rect.h>
#include <MenuItem.h>
#include <TextView.h>
#include <Message.h>
#include <TextControl.h>
#include <stdio.h>

class PostItem : public BStringItem {
public:
  inline PostItem(BlogPositivePost *post) : BStringItem(post->Name()) {
    fPost = post;
  }
  inline BlogPositivePost *Post() {
    return fPost;
  }
 private:
  BlogPositivePost *fPost;
};

class PluginAndWindowThing {
 public:
  BlogPositivePostListView *fWindow;
  BlogPositivePlugin *fPlugin;
  BlogPositiveBlog *fBlog;
  BListView *fView;
};

const uint32 kPostListViewOpenPost = 'PLOP';
const uint32 kPostListViewRemovePost = 'PLRP';
const uint32 kPostListViewNewPost = 'PLNP';

int32
BlogPositivePostListView::loadList(void *t)
{
    PluginAndWindowThing *thing = (PluginAndWindowThing *)t;
    BList *list = thing->fPlugin->GetBlogPosts(thing->fBlog);
    if(thing->fView->LockLooper()) {
      for(int i = 0; i < list->CountItems(); i++) {
	BlogPositivePost *post = (BlogPositivePost *)list->ItemAt(i);
	thing->fView->AddItem(new PostItem(post));
      }
      thing->fView->UnlockLooper();
    }
}

BlogPositivePostListView::BlogPositivePostListView(const char *aName, BlogPositiveBlog *aBlog)
    : BView(aName, 0)
{
    fBlog = aBlog;
    
    fListView = new BListView("ListView");
    fListView->SetInvocationMessage(new BMessage(kPostListViewOpenPost));

    BMenuBar *menu = new BMenuBar("MenuView");
    fAddPost = new BMenuItem("New Post", new BMessage(kPostListViewNewPost));
    fRemovePost = new BMenuItem("Remove Post", new BMessage(kPostListViewRemovePost));
    menu->AddItem(fAddPost);
    menu->AddItem(fRemovePost);

    Reload();

    SetLayout(new BGroupLayout(B_VERTICAL));
    AddChild(BGroupLayoutBuilder(B_VERTICAL, 0).Add(menu).Add(fListView));

}
void
BlogPositivePostListView::Reload()
{
    if(fListView->LockLooper())
    {
	fListView->MakeEmpty();
	fListView->UnlockLooper();
    }
    PluginAndWindowThing *thing = new PluginAndWindowThing();
    thing->fWindow = this;
    thing->fBlog = fBlog;
    thing->fView = fListView;
    thing->fPlugin = fBlog->Plugin();

    thread_id readThread = spawn_thread(BlogPositivePostListView::loadList, "load_posts", B_NORMAL_PRIORITY, thing);
    resume_thread(readThread);

}
class NewPostWindow : public BWindow
{
public:
    NewPostWindow(BlogPositivePostListView *view)
	: BWindow(BRect(100, 100, 500, 600), "New Post", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
    {
	fTextView = new BTextControl("TextView", "Title: ", "", new BMessage('BPFN'));
	SetLayout(new BGroupLayout(B_VERTICAL));
	AddChild(BGroupLayoutBuilder(B_VERTICAL, 0).Add(fTextView));
	fView = view;
    }
    void MessageReceived(BMessage *aMessage)
    {
	if(aMessage->what == 'BPFN')
	{
	    aMessage->PrintToStream();
	    aMessage->SetString("postTitle", fTextView->Text());
	    fView->MessageReceived(aMessage);
	    Close();
	} else {
	    BWindow::MessageReceived(aMessage);
	}
    }
    BlogPositivePostListView *fView;
    BTextControl *fTextView;
};

void
BlogPositivePostListView::MessageReceived(BMessage *aMessage)
{
    switch(aMessage->what)
    {
    case 'BPFN': {
	BlogPositivePost *p = fBlog->Plugin()->CreateNewPost(fBlog, aMessage->GetString("postTitle", ""));
	printf("p = %d\n", p);
	if(p != NULL)
	{
	    Reload();
	    BlogPositivePostEditorWindow *window = 
		new BlogPositivePostEditorWindow(p, BRect(100, 100, 500, 600));
	}
	break;
    }
    case kPostListViewOpenPost:
    {
	if(aMessage->GetInt32("index", -1) != -1)
	{
	    BlogPositivePost *post = ((PostItem *)fListView->ItemAt(aMessage->GetInt32("index", -1)))->Post();
	    BlogPositivePostEditorWindow *window =
		new BlogPositivePostEditorWindow(post, BRect(100, 100, 500, 600));
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
	if(fListView->CurrentSelection() != -1)
	{
	    BlogPositivePost *post = ((PostItem *)fListView->ItemAt(fListView->CurrentSelection()))->Post();
	    post->Blog()->Plugin()->RemovePost(post);
	    Reload();
	}
    }
    default:
	BView::MessageReceived(aMessage);
    }
}

void
BlogPositivePostListView::AttachedToWindow()
{
    fAddPost->SetTarget(this);
    fListView->SetTarget(this);
    fRemovePost->SetTarget(this);
}
