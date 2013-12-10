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
	void		Reload(BlogList* BlogItemList)
	{
		for(int i = 0; i < BlogItemList->CountItems(); i++)
			AddItem(new BlogPositiveBlogListItem(BlogItemList->ItemAt(i)));
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
			PluginList* pluginList = BlogPositivePluginLoader::List();
			BlogPositivePlugin* plugin = pluginList->ItemAt(index);
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
	int32 sel = fListView->CurrentSelection();
	if (sel == -1)
		return;
	BlogPositiveSettings* settings = new BlogPositiveSettings("bloglist");
	BlogList* lis = BlogPositiveBlog::DeserializeList(settings, "blogs");
	BlogPositiveBlogListItem* listItem
		= static_cast<BlogPositiveBlogListItem*>(fListView->ItemAt(sel));
	BlogPositiveBlog* blog = listItem->Blog();
	for (int i = 0; i < lis->CountItems(); i++)
	{
		if (strcmp(blog->Name(), lis->ItemAt(i)->Name()) == 0)
		{
			lis->RemoveItemAt(i);
			break;
		}
	}
	BlogPositiveSettings::SaveOther(BlogPositiveBlog::SerializeList(lis,
		"blogs"), "bloglist");

	Reload(lis);
}


void
BlogPositiveMainView::Reload(BlogList* lis)
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

	PluginList* pluginList = BlogPositivePluginLoader::List();
	BlogPositiveSettings* settings = new BlogPositiveSettings("bloglist");

	BlogList* lis = BlogPositiveBlog::DeserializeList(settings, "blogs");
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

	SetLayout(new BGroupLayout(B_VERTICAL, 0));
	AddChild(fMenuBar);
	AddChild(new BScrollView("scroll_view", fListView, 0, false, true));
}
