/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePostEditorView.h"

#include <Catalog.h>
#include <GroupLayout.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Message.h>
#include <TextView.h>

#include "../API/BlogPositiveBlog.h"
#include "../API/BlogPositivePlugin.h"
#include "../API/BlogPositivePost.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Post Editor View"

const uint32 kPostEditorSavePost = 'PESP';

BlogPositivePostEditorView::BlogPositivePostEditorView(const char* name,
	BlogPositivePost* post)
	:
	BView(name, 0),
	fPost(post)
{

	fTextView = new BTextView("TextView");
	fTextView->SetText(post->Page());

	BMenuBar* menuBar = new BMenuBar("MenuBar");

	fMenuItem = new BMenuItem(B_TRANSLATE("Save"),
		new BMessage(kPostEditorSavePost));

	menuBar->AddItem(fMenuItem);

	SetLayout(new BGroupLayout(B_VERTICAL, 0));

	AddChild(menuBar);
	AddChild(new BScrollView("scroll_view", fTextView, 0, false, true));
}


void
BlogPositivePostEditorView::AttachedToWindow()
{
	fMenuItem->SetTarget(this);
}


void
BlogPositivePostEditorView::Save()
{
	fPost->SetPage(fTextView->Text());
	fPost->Blog()->Plugin()->SavePost(fPost);
}


void
BlogPositivePostEditorView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case kPostEditorSavePost:
			Save();
			break;
		default:
			BView::MessageReceived(message);
	}
}
