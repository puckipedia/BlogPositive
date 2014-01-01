/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_POST_H
#define BP_POST_H


#include <ObjectList.h>
#include <String.h>

#include "Metadata/Metadata.h"

class BlogPositiveBlog;

class BlogPositivePost {
public:
						BlogPositivePost() {}
						BlogPositivePost(BlogPositiveBlog* blog);

	BlogPositiveBlog*	Blog();
	void				SetBlog(BlogPositiveBlog* blog);

	const char*			Name();
	void				SetName(const char* name);

	const char*			Page();
	void				SetPage(const char* page);
	uint32				PageSize();
	
	Metadata*			Metadata();	

protected:
	BlogPositiveBlog*	fBlog;
	Metadata*			fMetadata;
	BString				fName;
	BString				fPage;
};

typedef BObjectList<BlogPositivePost> PostList;

#endif
