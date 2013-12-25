/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_LJ_PLUGIN_H
#define BP_LJ_PLUGIN_H


#include "BlogPositivePlugin.h"
#include "BlogPositivePost.h"

class BlogPositiveBlog;
class BlogPositiveBlogListDelegate;
class BString;
class XmlNode;
class XmlRpcRequest;

class LiveJournalPlugin : public BlogPositivePlugin
{
public:
	uint32				Version();
	const char*			Name();
	int32				Type();

	uint32				MainHandler();
	bool				Supports(int32 Code);

	PostList*			GetBlogPosts(BlogPositiveBlog* aBlog);
	void				SavePost(BlogPositivePost* post);
	void				RemovePost(BlogPositivePost* post);
	BlogPositivePost*	CreateNewPost(BlogPositiveBlog* aBlog,
							const char* aName);
	void				OpenNewBlogWindow(BlogPositiveBlogListDelegate* mainView);
private:
	XmlNode*			Request(XmlRpcRequest* r, BString* responseString,
							BString auth);
	void				GetAuthentication(BString Auth,
							BString* Username, BString* pass,
							BString* journal);
};

#endif