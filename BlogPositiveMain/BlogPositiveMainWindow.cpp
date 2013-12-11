/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "BlogPositiveMainWindow.h"
#include "BlogPositiveMainView.h"

#include <Catalog.h>
#include <GroupLayout.h>
#include <Message.h>
#include <Rect.h>

#include "../BlogPositiveDelegate.h"
#include "../BlogPositivePostList/BlogPositivePostListWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Main Window"

class BlogPositiveMainDelegate : public BlogPositiveDelegate
{
	public:
		void	OpenPostList(BlogPositiveBlog* blog);
};


BlogPositiveMainDelegate::OpenPostList(BlogPositiveBlog* blog)
{
	BlogPositivePostListWindow* window
		= new BlogPositivePostListWindow(blog, BRect(100, 100, 400, 600));
	window->Show();
}


BlogPositiveMainWindow::BlogPositiveMainWindow(BRect frame)
	:
	BWindow(frame, B_TRANSLATE("BlogPositive"),
		B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	BlogPositiveMainView* view
		= new BlogPositiveMainView("MainView", new BlogPositiveMainDelegate());
	AddHandler(view);

	SetLayout(new BGroupLayout(B_VERTICAL, 0));
	AddChild(view);
}
