/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_EXAMPLE_PLUGIN_H
#define BP_EXAMPLE_PLUGIN_H


#include "BlogPositivePlugin.h"
#include "BlogPositivePost.h"

class BList;
class BlogPositiveBlog;

class ExamplePlugin : public BlogPositivePlugin {
public:
	uint32				Version() {return 0;}

	uint32				MainHandler();
	bool				Supports(int32 Code);

	PostList*			GetBlogPosts(BlogPositiveBlog* blog);
	void				SavePost(BlogPositivePost* post);
	BlogPositivePost*	CreateNewPost(BlogPositiveBlog* blog, const char* name);
	void				RemovePost(BlogPositivePost* post);
};

#endif
