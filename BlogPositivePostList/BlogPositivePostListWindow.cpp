/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePostListWindow.h"

#include <Rect.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <String.h>

#include "../API/BlogPositiveBlog.h"
#include "BlogPositivePostListView.h"

BlogPositivePostListWindow::BlogPositivePostListWindow(
	BlogPositiveBlog* blog, BRect rect)
	:
	BWindow(rect, "Blog", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	fListView = new BlogPositivePostListView("ListView", blog);

	SetLayout(new BGroupLayout(B_VERTICAL));
	AddChild(
		BLayoutBuilder::Group<>(B_VERTICAL, 0)
			.Add(fListView));
}
