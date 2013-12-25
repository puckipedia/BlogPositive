/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePluginLoader.h"

#include <kernel/image.h>
#include <stdio.h>

#include <Directory.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <List.h>
#include <Path.h>

#include "BlogPositiveBlog.h"

PluginList* BlogPositivePluginLoader::fList;

void LoadPaths(BPath dirPath, PluginList* list)
{
	printf("Loading dir %s:\n", dirPath.Path());
	BDirectory addonsDir(dirPath.Path());
	BEntry entry;
	BPath path;
	void (*plugin)(PluginList*);
	while (addonsDir.GetNextEntry(&entry) == B_OK) {
		entry.GetPath(&path);
		printf("\tLoading addon %s: ", path.Path());
		image_id addon = load_add_on(path.Path());
		if (addon > 0) {
			if (get_image_symbol(addon,
				"loadModules",
				B_SYMBOL_TYPE_TEXT,
				reinterpret_cast<void**>(&plugin)) == B_OK) {
				plugin(list);
				printf("Win :D\n");
			} else {
				printf("Fail :( (missing symbol)\n");
			}
		} else {
			printf("Fail :( (not an addon?)\n");
		}
	}
	printf("Done.\n");
}

void
BlogPositivePluginLoader::Initialize()
{
	fList = new PluginList();
	BPath path;
	find_directory(B_SYSTEM_ADDONS_DIRECTORY, &path);
	path.Append("BlogPositive");
	LoadPaths(path, fList);
	find_directory(B_USER_ADDONS_DIRECTORY, &path);
	path.Append("BlogPositive");
	LoadPaths(path, fList);
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
