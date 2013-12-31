/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_WP_PLUGIN_H
#define BP_WP_PLUGIN_H


#include "BlogPositiveBlogPlugin.h"
#include "BlogPositivePost.h"

class BlogPositiveBlog;
class BlogPositiveBlogListDelegate;
class BString;
class XmlNode;
class XmlRpcRequest;

class WordpressPlugin : public BlogPositiveBlogPlugin
{
public:
	uint32				Version();
	const char*			Name();

	uint32				MainHandler();
	bool				Supports(uint32 Code);

	PostList*			GetBlogPosts(BlogPositiveBlog* aBlog);
	void				SavePost(BlogPositivePost* aPost);
	BlogPositivePost*	CreateNewPost(BlogPositiveBlog* aBlog,
							const char* aName);
	void				RemovePost(BlogPositivePost* aPost);
	void				OpenNewBlogWindow(BlogPositiveBlogListDelegate* dele);
private:
	XmlNode*			Request(XmlRpcRequest* r, BString* responseString,
							BlogPositiveBlog* blog);
};

#endif
