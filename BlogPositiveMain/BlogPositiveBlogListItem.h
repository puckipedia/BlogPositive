/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_BLOGLISTITEM_H
#define BP_BLOGLISTITEM_H


#include <ListItem.h>

class BlogPositiveBlog;

class BlogPositiveBlogListItem : public BStringItem {
public:
						BlogPositiveBlogListItem(BlogPositiveBlog* blog);
	BlogPositiveBlog*	Blog()
	{
		return fBlog;
	}

private:
	BlogPositiveBlog*	fBlog;
};

#endif
