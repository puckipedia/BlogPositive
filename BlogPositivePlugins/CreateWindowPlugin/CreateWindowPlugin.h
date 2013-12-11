/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_CREATE_WINDOW_PLUGIN_H
#define BP_CREATE_WINDOW_PLUGIN_H


#include "../../API/BlogPositivePlugin.h"

class BlogPositivePluginBlogPostWindow;
class BlogPositivePluginPostListWindow;
class BlogPositivePost;
class BlogPositiveBlog;
class BWindow;
class CreateWindowPlugin : public BlogPositivePlugin {
public:
	uint32				Version();
	int32				Type();
	void				HookBlogList(BlogPositivePluginPostListWindow** window,
							BlogPositiveBlog* blog);
	BlogPositivePost*	TryGetPost(BWindow* window);
	static int32		loadList(void* t);
	void				HookEditor(BlogPositivePluginBlogPostWindow** window,
							BlogPositivePost* post);
	const char*			GetPostContent(BlogPositivePluginBlogPostWindow* window);
};

#endif
