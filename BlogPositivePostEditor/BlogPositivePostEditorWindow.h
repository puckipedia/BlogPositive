/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_POST_EDITOR_WINDOW_H
#define BP_POST_EDITOR_WINDOW_H


#include <Window.h>

class BlogPositivePost;
class BlogPositivePostEditorView;
class BRect;

class BlogPositivePostEditorWindow : public BWindow
{
public:
								BlogPositivePostEditorWindow(
									BlogPositivePost* aPost, BRect aRect);
private:
	BlogPositivePostEditorView*	fView;
};

#endif
