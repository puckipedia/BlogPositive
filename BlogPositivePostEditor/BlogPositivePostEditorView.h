/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_POST_EDITOR_VIEW_H
#define BP_POST_EDITOR_VIEW_H


#include <View.h>

class BlogPositivePost;
class BMenuItem;
class BMessage;
class BTextView;

class BlogPositivePostEditorView : public BView
{
public:
						BlogPositivePostEditorView(const char* name,
							BlogPositivePost* post);

	void 				AttachedToWindow();
	void 				MessageReceived(BMessage* aMessage);

	void 				Save();
private:
	BMenuItem*			fMenuItem;
	BTextView*			fTextView;
	BlogPositivePost*	fPost;
};

#endif
