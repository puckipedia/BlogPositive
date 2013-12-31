/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_BLOGGER_PLUGIN_H
#define BP_BLOGGER_PLUGIN_H

#include "BlogPositiveBlogPlugin.h"
#include "BlogPositivePost.h"

class BloggerPlugin : public BlogPositiveBlogPlugin
{
public:
	uint32				Version();
	const char*			Name();

	uint32				MainHandler();
	bool				Supports(uint32 Code);

	PostList*			GetBlogPosts(BlogPositiveBlog* aBlog);

};

#endif
