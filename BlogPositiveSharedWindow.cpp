/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "BlogPositiveSharedWindow.h"

#include <Rect.h>
#include <View.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <stdio.h>

#include "BlogPositiveMain/BlogPositiveMainView.h"
#include "BlogPositivePostList/BlogPositivePostListView.h"
#include "BlogPositiveDelegate.h"

class BlogPositiveSharedDelegate : public BlogPositiveDelegate
{
public:
    void OpenPostList(BlogPositiveBlog *blog)
	{
	    if(fWindow->LockLooper()) {	
		BView *v = fWindow->FindView("SecondView");
		printf("Opening postlist: %d\n", v->CountChildren());
		for(int i = 0; i < v->CountChildren(); i++)
		{
		    BView *subView = v->ChildAt(i);
		    subView->RemoveSelf();
		    printf("Removing %d", subView);
		    delete subView;
		}
		v->SetLayout(new BGroupLayout(B_VERTICAL));
		v->AddChild(BGroupLayoutBuilder(B_VERTICAL, 0).Add(new BlogPositivePostListView("PostList", blog)));
		fWindow->UnlockLooper();
	    }
	}
    BlogPositiveSharedWindow *fWindow;
};

BlogPositiveSharedWindow::BlogPositiveSharedWindow(BRect aRect)
    : BWindow(aRect, "BlogPositive", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
    BView *secondView = new BView("SecondView", B_AUTO_UPDATE_SIZE_LIMITS);
    BlogPositiveSharedDelegate *derp = new BlogPositiveSharedDelegate();
    derp->fWindow = this;
    BlogPositiveMainView *mainView = new BlogPositiveMainView("MainView", derp);
    SetLayout(new BGroupLayout(B_HORIZONTAL));
    AddChild(
        BGroupLayoutBuilder(B_HORIZONTAL, 0)
        .Add(mainView)
        .Add(secondView));
}
