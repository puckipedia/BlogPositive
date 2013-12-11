/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_PLUGIN_WINDOW_H
#define BP_PLUGIN_WINDOW_H


#include <Window.h>

const uint32 kPostWindowGetSelection = 'PWgs';
const uint32 kPostWindowSavePost = 'PWsp';
const uint32 kPostWindowNewPost = 'PWnp';

class BlogPositiveBlog;
class BlogPositivePost;
class BMessage;
class BRect;

class BlogPositivePluginPostListWindow : public BWindow {
public:
						BlogPositivePluginPostListWindow(BRect rect,
							const char* Name);

	void				MessageReceived(BMessage* msg);

	void				SetBlog(BlogPositiveBlog* blog);
	BlogPositiveBlog*	Blog();

private:
	BlogPositiveBlog*	fBlog;
};

class BlogPositivePluginBlogPostWindow : public BWindow {
public:
						BlogPositivePluginBlogPostWindow(BRect rect,
							const char* Name);
	void				SetPost(BlogPositivePost* post);
	BlogPositivePost*	Post();
	void				MessageReceived(BMessage* msg);
private:
	BlogPositivePost*	fPost;
};

#endif
