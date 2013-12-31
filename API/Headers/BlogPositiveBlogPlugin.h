/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_BLOG_PLUGIN_H
#define BP_BLOG_PLUGIN_H


#include <ObjectList.h>

#include "BlogPositiveDelegate.h"
#include "BlogPositivePlugin.h"
#include "BlogPositivePost.h"

const uint32 kCreateBlog = 'BPCB';
const uint32 kCancelBlog = 'BPKB';

class BlogPositiveMainView;
class BlogPositiveBlog;
class BlogPositivePluginWindow;

class BlogPositiveBlogPlugin : public BlogPositivePlugin {
public:
	virtual	uint32				MainHandler();
	virtual	bool				Supports(uint32 Code);

	virtual	PostList*			GetBlogPosts(BlogPositiveBlog* blog);

	virtual	void				SavePost(BlogPositivePost* post);
	virtual	BlogPositivePost*	CreateNewPost(BlogPositiveBlog* blog,
									const char* name);
	virtual	void				RemovePost(BlogPositivePost* post);

	virtual	void				OpenNewBlogWindow(BlogPositiveBlogListDelegate* main);
};

#endif
