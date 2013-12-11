/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_MAIN_VIEW_H
#define BP_MAIN_VIEW_H


#include <View.h>
#include "../API/BlogPositiveBlog.h"

class BlogPositiveBlogListView;
class BlogPositiveDelegate;
class BMenuBar;
class BMenuItem;
class BMenu;
class BMessage;

class BlogPositiveMainView : public BView {
public:
								BlogPositiveMainView(const char* name,
									BlogPositiveDelegate* del);
	void						MessageReceived(BMessage* message);
	void						AttachedToWindow();
	void						Reload(BlogList* list);
	void						RemoveBlog();
private:
	BMenuBar*					fMenuBar;
	BMenu*						fNewMenu;
	BMenuItem*					fRemoveMenuItem;
	BlogPositiveBlogListView*	fListView;
	BlogPositiveDelegate*		fDelegate;
};

#endif
