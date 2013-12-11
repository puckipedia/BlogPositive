/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "CreateWindowPlugin.h"

#include <GroupLayout.h>
#include <ListView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <OS.h>
#include <ScrollView.h>
#include <TextView.h>

#include "../../API/BlogPositivePluginWindow.h"
#include "../../API/BlogPositivePost.h"

class PostItem : public BStringItem {
public:
						PostItem(BlogPositivePost* post);
	BlogPositivePost*	Post();
private:
	BlogPositivePost*	fPost;
};


PostItem::PostItem(BlogPositivePost* post)
	:
	BStringItem(post->Name())
{
	fPost = post;
}


BlogPositivePost*
PostItem::Post()
{
	return fPost;
}


class PluginAndWindowThing {
public:
	BWindow* fWindow;
	BlogPositivePlugin* fPlugin;
	BlogPositiveBlog* fBlog;
	BListView* fView;
};


uint32
CreateWindowPlugin::Version()
{
	return 0;
}


int32
CreateWindowPlugin::Type()
{
	return kBlogPositiveBlogEditor;
}


void
CreateWindowPlugin::HookBlogList(BlogPositivePluginPostListWindow** aWindow,
	BlogPositiveBlog* aBlog)
{
	BWindow* win = *aWindow;
	BListView* aView = new BListView("ListView");
	BMessage* msg = new BMessage(kPostWindowGetSelection);
	aView->SetInvocationMessage(msg);
	win->SetLayout(new BGroupLayout(B_VERTICAL, 10));
	win->AddChild(aView);
	PluginAndWindowThing* thing = new PluginAndWindowThing();
	thing->fWindow = *aWindow;
	thing->fBlog = aBlog;
	thing->fView = aView;
	thing->fPlugin = aBlog->Plugin();
	thread_id readThread = spawn_thread(loadList, "load_posts",
		B_NORMAL_PRIORITY, thing);
	resume_thread(readThread);
}


BlogPositivePost*
CreateWindowPlugin::TryGetPost(BWindow* window)
{
	BListView* listView = dynamic_cast<BListView*>(window->FindView("ListView"));
	if (aView != NULL) {
		int32 chosenOne = listView->CurrentSelection();
		if (chosenOne == -1) {
			return NULL;
		}
		PostItem* item = static_cast<PostItem*>(listView->ItemAt(chosenOne));
		return item->Post();
	}
	return NULL;
}


int32
CreateWindowPlugin::loadList(void* t)
{
	PluginAndWindowThing* thing = (PluginAndWindowThing*)t;
	PostList* list = thing->fPlugin->GetBlogPosts(thing->fBlog);
	if (thing->fView->LockLooper()) {
		for (int i = 0; i < list->CountItems(); i++) {
			BlogPositivePost* post = list->ItemAt(i);
			thing->fView->AddItem(new PostItem(post));
		}
		thing->fView->UnlockLooper();
	}
}


void
CreateWindowPlugin::HookEditor(BlogPositivePluginBlogPostWindow**aWindow,
	BlogPositivePost* aPost)
{
	BlogPositivePluginBlogPostWindow* win = *aWindow;

	BTextView* aView = new BTextView("TextView");
	aView->SetText(aPost->Page());

	BMenuBar* menuBar = new BMenuBar("MenuBar");

	win->SetLayout(new BGroupLayout(B_VERTICAL));
	win->AddChild(menuBar);
	win->AddChild(aView);

	BMenu* menu = new BMenu("File");

	menu->AddItem(new BMenuItem("Save",
		new BMessage(kPostWindowSavePost), 'S'));

	menuBar->AddItem(menu);
}


const char*
CreateWindowPlugin::GetPostContent(BlogPositivePluginBlogPostWindow* aWindow)
{
BView* aView = aWindow->FindView("TextView");
BTextView* aTextView = (BTextView*)aView;
return aTextView->Text();
}
