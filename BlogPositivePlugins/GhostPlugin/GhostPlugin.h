/* 
 * Copyright 2014 Josef Gajdusek, atx@atx.name
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef BP_GHOST_PLUGIN_H
#define BP_GHOST_PLUGIN_H

#include "BlogPositiveBlogPlugin.h"
#include "BlogPositivePost.h"

class JsonObject;
class JsonArray;
class BStringList;

class GhostPlugin : public BlogPositiveBlogPlugin {
public:
	uint32				Version();
	const char*			Name();

	uint32				MainHandler();
	bool				Supports(uint32 code);

	PostList*			GetBlogPosts(BlogPositiveBlog* blog);
	void				SavePost(BlogPositivePost* post);
	BlogPositivePost*	CreateNewPost(BlogPositiveBlog* blog,
							const char* name);
	void				RemovePost(BlogPositivePost* post);

	void				OpenNewBlogWindow(BlogPositiveBlogListDelegate* dele);

private:
	JsonObject*			_Request(BlogPositiveBlog* blog, BString target,
							BString method="GET", BString data="",
							bool authenticate=true);
	int32				_Authorize(BlogPositiveBlog* blog);
	void				_DisplayErrors(JsonArray* arr);
};

#endif
