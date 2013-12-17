/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_BLOGGER_PLUGIN_H
#define BP_BLOGGER_PLUGIN_H

#include "BlogPositivePlugin.h"
#include "BlogPositivePost.h"

class WordpressPlugin : public BlogPositivePlugin
{
public:
	uint32				Version();
	const char*			Name();
	int32				Type();

	uint32				MainHandler();
	bool				Supports(int32 Code);

	PostList*			GetBlogPosts(BlogPositiveBlog* aBlog);

};

#endif
