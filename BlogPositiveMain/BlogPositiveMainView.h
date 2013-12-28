/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_MAIN_VIEW_H
#define BP_MAIN_VIEW_H


#include <View.h>

#include "BlogPositiveBlog.h"
#include "BlogPositiveDelegate.h"

class BlogPositiveBlogListView;
class BlogPositiveMainDelegate;
class BMenuBar;
class BMenuItem;
class BMenu;
class BMessage;

class BlogPositiveMainView : public BView, public BlogPositiveBlogListDelegate
	{
public:
								BlogPositiveMainView(const char* name,
									BlogPositiveMainDelegate* del);

	void						MessageReceived(BMessage* message);
	void						AttachedToWindow();

	void						Reload(BlogList* list);
	void						RemoveBlog();

	// BlogListDelegate
	void						ReloadBlogs();


private:
	BMenuBar*					fMenuBar;
	BMenu*						fNewMenu;
	BMenuItem*					fRemoveMenuItem;
	BlogPositiveBlogListView*	fListView;
	BlogPositiveMainDelegate*	fDelegate;
};

#endif
