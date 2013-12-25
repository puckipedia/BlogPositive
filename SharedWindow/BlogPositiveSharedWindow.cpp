/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "BlogPositiveSharedWindow.h"
#include "BlogPositiveMainView.h"

#include <Catalog.h>
#include <GroupLayout.h>
#include <Message.h>
#include <Rect.h>
#include <SplitView.h>

#include "BlogPositiveDelegate.h"
#include "BlogPositivePostListView.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Shared Window"


class BlogPositiveSharedDelegate : public BlogPositiveMainDelegate
{
	public:
				BlogPositiveSharedDelegate(BlogPositiveSharedWindow* window);
		void	OpenPostList(BlogPositiveBlog* blog);
	private:
		BlogPositiveSharedWindow*	fWindow;
};


BlogPositiveSharedDelegate::BlogPositiveSharedDelegate(
	BlogPositiveSharedWindow* window)
{
	fWindow = window;
}

void
BlogPositiveSharedDelegate::OpenPostList(BlogPositiveBlog* blog)
{
	BView* rightView = fWindow->SplitView()->FindView("rightView");
	fWindow->SplitView()->RemoveChild(rightView);
	
	BlogPositivePostListView* view
		= new BlogPositivePostListView("rightView", blog);
	fWindow->SplitView()->AddChild(view);
	fWindow->SplitView()->SetItemCollapsed(1, true);
}


BSplitView*
BlogPositiveSharedWindow::SplitView()
{
	return fSplitView;
}


BlogPositiveSharedWindow::BlogPositiveSharedWindow(BRect frame)
	:
	BWindow(frame, B_TRANSLATE("BlogPositive"),
		B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	BlogPositiveMainView* view = new BlogPositiveMainView("MainView",
		new BlogPositiveSharedDelegate(this));
	AddHandler(view);
	BView* rightView = new BView("rightView", 0);
	SetLayout(new BGroupLayout(B_HORIZONTAL, 0));
	fSplitView = new BSplitView(B_HORIZONTAL, 0);
	fSplitView->AddChild(view);
	fSplitView->AddChild(rightView);
	AddChild(fSplitView);
	fSplitView->SetItemCollapsed(1, false);
}
