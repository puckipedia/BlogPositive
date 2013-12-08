/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePluginLoader.h"

#include <List.h>
#include <stdio.h>

#include "BlogPositiveBlog.h"
#include "BlogPositivePlugin.h"
#include "BlogPositivePluginWindow.h"
#include "../Example/ExamplePlugin.h"
#include "../BlogPositivePlugins/WordpressPlugin/WordpressPlugin.h"

BList* BlogPositivePluginLoader::fList;

void
BlogPositivePluginLoader::Initialize()
{
	fList = new BList();
	fList->AddItem(new ExamplePlugin());
	fList->AddItem(new WordpressPlugin());
}


void
BlogPositivePluginLoader::FindPlugin(BlogPositiveBlog* aBlog)
{
	BlogPositivePlugin* aWorkingPlugin = NULL;
	for (int i = 0; i < fList->CountItems(); i++)
	{
		BlogPositivePlugin* aPlugin = (BlogPositivePlugin*)fList->ItemAt(i);
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
