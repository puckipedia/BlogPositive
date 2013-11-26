/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once
#include <ListItem.h>
#include <Message.h>
#include "../API/BlogPositiveBlog.h"

class BlogPositiveBlogListItem : public BStringItem {
public:
	BlogPositiveBlogListItem(BlogPositiveBlog *blog);
	inline BlogPositiveBlog *Blog() {return fBlog;}
private:
	BlogPositiveBlog *fBlog;
};
