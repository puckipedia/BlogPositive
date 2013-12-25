/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_DELEGATE_H
#define BP_DELEGATE_H

class BlogPositiveBlog;

class BlogPositiveMainDelegate
{
public:
	virtual	void	OpenPostList(BlogPositiveBlog* blog) {};
};

class BlogPositiveBlogListDelegate
{
public:
	virtual	void	ReloadBlogs() {};
};

class BlogPositivePostListDelegate
{
public:
	virtual	void	ReloadPosts() {};
};

#endif
