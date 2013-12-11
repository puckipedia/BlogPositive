/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePost.h"

#include <String.h>

#include "BlogPositiveBlog.h"


BlogPositivePost::BlogPositivePost(BlogPositiveBlog* blog)
{
	SetBlog(blog);
}


BlogPositiveBlog*
BlogPositivePost::Blog()
{
	return fBlog;
}


void
BlogPositivePost::SetBlog(BlogPositiveBlog* blog)
{
	fBlog = blog;
}


const char*
BlogPositivePost::Name()
{
	return fName.String();
}


void
BlogPositivePost::SetName(const char* name)
{
	fName = name;
}


const char*
BlogPositivePost::Page()
{
	return fPage.String();
}


void
BlogPositivePost::SetPage(const char* page)
{
	fPage = page;
}


uint32
BlogPositivePost::PageSize()
{
	return fPage.Length();
}
