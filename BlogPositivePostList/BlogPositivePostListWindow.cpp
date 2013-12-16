/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePostListWindow.h"

#include <Catalog.h>
#include <GroupLayout.h>
#include <Rect.h>
#include <String.h>

#include "BlogPositiveBlog.h"
#include "BlogPositivePostListView.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Post List Window"


BlogPositivePostListWindow::BlogPositivePostListWindow(
	BlogPositiveBlog* blog, BRect rect)
	:
	BWindow(rect, "", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	BString title;
	title.SetToFormat(B_TRANSLATE("Blog: %s"), blog->Name());
	SetTitle(title.String());

	fListView = new BlogPositivePostListView("ListView", blog);

	SetLayout(new BGroupLayout(B_VERTICAL, 0));
	AddChild(fListView);
}
