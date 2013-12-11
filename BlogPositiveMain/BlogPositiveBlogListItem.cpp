/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositiveBlogListItem.h"

#include "../API/BlogPositiveBlog.h"


BlogPositiveBlogListItem::BlogPositiveBlogListItem(BlogPositiveBlog* blog)
	:
	BStringItem(blog->Name())
{
	fBlog = blog;
}
