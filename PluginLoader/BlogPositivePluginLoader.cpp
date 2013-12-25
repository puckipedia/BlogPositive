/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePluginLoader.h"

#include <List.h>

#include "BloggerPlugin.h"
#include "BlogPositiveBlog.h"
#include "LiveJournalPlugin.h"
#include "WordpressPlugin.h"


PluginList* BlogPositivePluginLoader::fList;


void
BlogPositivePluginLoader::Initialize()
{
	fList = new PluginList();
	// fList->AddItem(new BloggerPlugin());
		// Disabled temporarily (haikuporter release)
	fList->AddItem(new LiveJournalPlugin());
	fList->AddItem(new WordpressPlugin());
}


void
BlogPositivePluginLoader::FindPlugin(BlogPositiveBlog* blog)
{
	BlogPositivePlugin* workingPlugin = NULL;
	for (int i = 0; i < fList->CountItems(); i++) {
		BlogPositivePlugin* plugin = fList->ItemAt(i);
		if (plugin->Type() == kBlogPositiveBlogApi
			&& plugin->Supports(blog->BlogHandler())) {
			workingPlugin = plugin;
			break;
		}
	}

	if (workingPlugin == NULL)
		return;

	blog->SetPlugin(workingPlugin);
}


PluginList*
BlogPositivePluginLoader::List()
{
	return fList;
}
