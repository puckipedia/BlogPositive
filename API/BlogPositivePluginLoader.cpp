/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePluginLoader.h"

#include <List.h>
#include <stdio.h>

#include "BlogPositiveBlog.h"
#include "BlogPositivePluginWindow.h"
#include "../Example/ExamplePlugin.h"
#include "../BlogPositivePlugins/WordpressPlugin/WordpressPlugin.h"

PluginList* BlogPositivePluginLoader::fList;

void
BlogPositivePluginLoader::Initialize()
{
	fList = new PluginList();
	fList->AddItem(new ExamplePlugin());
	fList->AddItem(new WordpressPlugin());
}


void
BlogPositivePluginLoader::FindPlugin(BlogPositiveBlog* aBlog)
{
	BlogPositivePlugin* aWorkingPlugin = NULL;
	for (int i = 0; i < fList->CountItems(); i++)
	{
		BlogPositivePlugin* aPlugin = fList->ItemAt(i);
		if (aPlugin->Type() == kBlogPositiveBlogApi
			&& aPlugin->Supports(aBlog->BlogHandler()))
		{
			aWorkingPlugin = aPlugin;
			break;
		}
	}

	if (aWorkingPlugin == 0)
		return;

	aBlog->SetPlugin(aWorkingPlugin);
}


PluginList*
BlogPositivePluginLoader::List()
{
	return fList;
}
