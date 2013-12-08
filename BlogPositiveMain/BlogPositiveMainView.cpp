/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositiveMainView.h"

#include <TextView.h>
#include <ListView.h>
#include <GroupLayoutBuilder.h>
#include <ScrollView.h>
#include <List.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <stdio.h>
#include <Point.h>
#include <PopUpMenu.h>
#include <String.h>
#include <Rect.h>

#include "../API/BlogPositiveBlog.h"
#include "../BlogPositiveSettings.h"
#include "../BlogPositivePostList/BlogPositivePostListWindow.h"
#include "BlogPositiveBlogListItem.h"
#include "../API/BlogPositivePlugin.h"
#include "../API/BlogPositivePluginLoader.h"
#include "../BlogPositiveDelegate.h"

const int32 kBlogSelected = 'BPBS';
const int32 kCreateNewBlog = 'BPCB';
const int32 kRemoveCurrentBlog = 'BPRC';
class BlogPositiveBlogListView : public BListView {
public:
				BlogPositiveBlogListView()
				:
				BListView("BlogPositiveBlogListView", B_SINGLE_SELECTION_LIST)
	{
	}
	void		Reload(BList* BlogItemList)
	{
		BlogItemList->DoForEach(&BlogPositiveBlogListView::createItem, this);
	}

	static bool	createItem(void* item, void* p)
	{
		BlogPositiveBlogListItem* item
			= new BlogPositiveBlogListItem((BlogPositiveBlog* )item);
		((BListView* )p)->AddItem(item);
		return false;
	}
};

void
BlogPositiveMainView::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		case kRemoveCurrentBlog:
			RemoveBlog();
			break;
		case kBlogSelected:
		{
			if (fListView->CurrentSelection() == -1)
				break;
			BlogPositiveBlog* aBlog
				= ((BlogPositiveBlogListItem*)fListView->ItemAt(
						fListView->CurrentSelection()))->Blog();
			BlogPositivePluginLoader::FindPlugin(aBlog);
			fDelegate->OpenPostList(aBlog);
			break;
		}
		case kCreateNewBlog:
		{
			int32 index = message->GetInt32("ding", 0);
			BList* pluginList = BlogPositivePluginLoader::List();
			BlogPositivePlugin* plugin
				= (BlogPositivePlugin* )pluginList->ItemAt(index);
			plugin->OpenNewBlogWindow(this);
			break;
		}
		default:
			BView::MessageReceived(message);
	}
}


void
BlogPositiveMainView::RemoveBlog()
{
	if (fListView->CurrentSelection() == -1)
		return;
	BlogPositiveSettings* settings = new BlogPositiveSettings("bloglist");
	BList* lis = BlogPositiveBlog::DeserializeList(settings, "blogs");
	BlogPositiveBlog* blog = ((BlogPositiveBlogListItem* )fListView->ItemAt(
		fListView->CurrentSelection()))->Blog();
	for (int i = 0; i < lis->CountItems(); i++)
	{
		if (strcmp(blog->Name(),
			((BlogPositiveBlog*)lis->ItemAt(i))->Name()) == 0)
		{
			lis->RemoveItem(i);
			break;
		}
	}
	BlogPositiveSettings::SaveOther(BlogPositiveBlog::SerializeList(lis, "blogs")
		, "bloglist");

	Reload(lis);
}


void
BlogPositiveMainView::Reload(BList* lis)
{
	fListView->MakeEmpty();
	fListView->Reload(lis);
}


void
BlogPositiveMainView::AttachedToWindow()
{
	fListView->SetTarget(this);
	for (int i = 0; i < fNewMenu->CountItems(); i++)
	{
		fNewMenu->ItemAt(i)->SetTarget(this);
	}
	fRemoveMenuItem->SetTarget(this);
}


BlogPositiveMainView::BlogPositiveMainView(const char* name,
	BlogPositiveDelegate* del)
	:
	BView(name, 0)
{
	fDelegate = del;

	fListView = new BlogPositiveBlogListView();
	fListView->SetInvocationMessage(new BMessage(kBlogSelected));

	fMenuBar = new BMenuBar("MenuBar");

	fNewMenu = new BMenu("Add Blog");
	fMenuBar->AddItem(fNewMenu);

	BList* pluginList = BlogPositivePluginLoader::fList;
	BlogPositiveSettings* settings = new BlogPositiveSettings("bloglist");

	BList* lis = BlogPositiveBlog::DeserializeList(settings, "blogs");
	fListView->Reload(lis);

	for (int i = 0; i < pluginList->CountItems(); i++)
	{
		BlogPositivePlugin* pl = (BlogPositivePlugin* )pluginList->ItemAt(i);
		if (pl->Type() == kBlogPositiveBlogApi)
		{
			BMessage* msg = new BMessage(kCreateNewBlog);
			msg->SetInt32("ding", i);
			msg->SetString("sendToView", Name());
			BMenuItem* mi = new BMenuItem(pl->Name(), msg);
			fNewMenu->AddItem(mi);
		}
	}

	BMessage* aMenuItemMessage = new BMessage(kRemoveCurrentBlog);
	fRemoveMenuItem = new BMenuItem("Remove blog", aMenuItemMessage);
	fMenuBar->AddItem(fRemoveMenuItem);

	SetLayout(new BGroupLayout(B_VERTICAL));
	AddChild(fMenuBar);
	AddChild(new BScrollView("scroll_view", fListView, 0, false, true)));
}
