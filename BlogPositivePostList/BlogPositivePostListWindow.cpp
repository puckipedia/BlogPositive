/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePostListWindow.h"

#include <CatalogMacros.h>
#include <Rect.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <String.h>

#include "../API/BlogPositiveBlog.h"
#include "BlogPositivePostListView.h"

#define B_TRANSLATION_CONTEXT "Post List Window"

BlogPositivePostListWindow::BlogPositivePostListWindow(
	BlogPositiveBlog* blog, BRect rect)
	:
	BWindow(rect, "", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	BString title;
	title.SetToFormat(B_TRANSLATE("Blog: %s", "Title of blog window, %s is the blog name"), blog->Name());
	SetTitle(title.String());

	fListView = new BlogPositivePostListView("ListView", blog);

	SetLayout(new BGroupLayout(B_VERTICAL));
	AddChild(
		BLayoutBuilder::Group<>(B_VERTICAL, 0)
			.Add(fListView));
}
