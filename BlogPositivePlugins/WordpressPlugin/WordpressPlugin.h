/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef BP_WP_PLUGIN_H
#define BP_WP_PLUGIN_H

#include "../../API/BlogPositivePlugin.h"

class BlogPositiveBlog;
class BlogPositivePost;
class WordpressPlugin : public BlogPositivePlugin
{
public:
	uint32				Version();
	char*				Name();
	int32				Type();

	uint32				MainHandler();
	bool				Supports(int32 Code);

	BList*				GetBlogPosts(BlogPositiveBlog* aBlog);
	void				SavePost(BlogPositivePost* aPost);
	BlogPositivePost*	CreateNewPost(BlogPositiveBlog* aBlog,
							const char* aName);
	void				RemovePost(BlogPositivePost* aPost);
private:
	void				GetAuthentication(BString Auth,
							BString* Username, BString* pass,
							BString* XmlRPCUrl);
};

#endif
