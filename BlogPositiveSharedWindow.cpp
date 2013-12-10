/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "BlogPositiveSharedWindow.h"

#include <CatalogMacros.h>
#include <Rect.h>
#include <View.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <stdio.h>

#include "BlogPositiveMain/BlogPositiveMainView.h"
#include "BlogPositivePostList/BlogPositivePostListView.h"
#include "BlogPositiveDelegate.h"

#define B_TRANSLATION_CONTEXT "Shared Window"

class BlogPositiveSharedDelegate : public BlogPositiveDelegate
{
public:
	void						OpenPostList(BlogPositiveBlog* blog);
	void						SetWindow(BlogPositiveSharedWindow* window);
	BlogPositiveSharedWindow*	Window();

private:
	BlogPositiveSharedWindow*	fWindow;
};


BlogPositiveSharedDelegate::OpenPostList(BlogPositiveBlog* blog)
{
	if (fWindow->LockLooper()) {
		BView* v = fWindow->FindView("SecondView");
		for (int i = 0; i < v->CountChildren(); i++)
		{
			BView* subView = v->ChildAt(i);
			subView->RemoveSelf();
			delete subView;
		}
		BGroupLayout* layout = new BGroupLayout(B_VERTICAL);
		v->SetLayout(layout);
		v->AddChild(new BlogPositivePostListView("PostList", blog)));
		fWindow->UnlockLooper();
	}
}


void
BlogPositiveSharedDelegate::SetWindow(BlogPositiveSharedWindow* window)
{
	fWindow = window;
}


BlogPositiveSharedWindow
BlogPositiveSharedDelegate::Window()
{
	return fWindow;
}


BlogPositiveSharedWindow::BlogPositiveSharedWindow(BRect aRect)
	:
	BWindow(aRect, B_TRANSLATE("BlogPositive", "Window title"),
		B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	BlogPositiveSharedDelegate* delegate = new BlogPositiveSharedDelegate();
	delegate->fWindow = this;
	BlogPositiveMainView* mainView
		= new BlogPositiveMainView("MainView", delegate);

	BView* secondView = new BView("SecondView", B_AUTO_UPDATE_SIZE_LIMITS);
	SetLayout(new BGroupLayout(B_HORIZONTAL));

	AddChild(mainView);
	AddChild(secondView);
}
