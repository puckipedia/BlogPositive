/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_PLUGIN_H
#define BP_PLUGIN_H


#include <ObjectList.h>

#include "BlogPositivePost.h"

#define kBlogPositiveBlogApi 1

class BlogPositiveMainView;
class BlogPositiveBlog;
class BlogPositivePluginWindow;

class BlogPositivePlugin {
public:
	virtual	uint32				Version();
	virtual	uint32				MainHandler();
	virtual	const char*			Name();
	virtual	int32				Type();

	virtual	bool				Supports(int32 Code);
	virtual	PostList*			GetBlogPosts(BlogPositiveBlog* blog);
	virtual	void				SavePost(BlogPositivePost* post);
	virtual	BlogPositivePost*	CreateNewPost(BlogPositiveBlog* blog,
									const char* name);
	virtual	void				RemovePost(BlogPositivePost* post);
	virtual	void				OpenNewBlogWindow(BlogPositiveMainView* main);
};

typedef BObjectList<BlogPositivePlugin> PluginList;

#endif
