/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePostEditorWindow.h"

#include <CatalogMacros.h>
#include <Rect.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <String.h>

#include "../API/BlogPositivePost.h"
#include "BlogPositivePostEditorView.h"

#define B_TRANSLATION_CONTEXT "Post Editor Window"

BlogPositivePostEditorWindow::BlogPositivePostEditorWindow(
	BlogPositivePost* aPost, BRect aRect)
	:
	BWindow(aRect, "", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	fView = new BlogPositivePostEditorView("EditorView", aPost);

	BString titleString;
	titleString.SetToFormat(B_TRANSLATE("Post: %s", "The %s par);
	titleString << aPost->Name();
	SetTitle(titleString.String());

	SetLayout(new BGroupLayout(B_VERTICAL));
	AddChild(
		BGroupLayoutBuilder(B_VERTICAL, 0)
		.Add(fView));
}
