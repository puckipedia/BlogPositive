/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "BlogPositiveMainWindow.h"
#include "BlogPositiveMainView.h"

#include <Rect.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <Message.h>
#include <stdio.h>

#include "../BlogPositiveDelegate.h"
#include "../BlogPositivePostList/BlogPositivePostListWindow.h"

#define B_TRANSLATION_CONTEXT "Main Window"

class BlogPositiveMainDelegate : public BlogPositiveDelegate
{
	public:
		void	OpenPostList(BlogPositiveBlog* aBlog)
	{
		BlogPositivePostListWindow* aWindow
			= new BlogPositivePostListWindow(aBlog, BRect(100, 100, 400, 600));
		aWindow->Show();
	}
};

BlogPositiveMainWindow::BlogPositiveMainWindow(BRect frame)
	:
	BWindow(frame, "BlogPositive", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	BlogPositiveMainView* view
		= new BlogPositiveMainView("MainView", new BlogPositiveMainDelegate());
	AddHandler(view);
	SetLayout(new BGroupLayout(B_VERTICAL), 0);
	AddChild(view);
}


void
BlogPositiveMainWindow::MessageReceived(BMessage* message)
{
	BWindow::MessageReceived(message);
}
