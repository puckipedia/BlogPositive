/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_POST_LIST_VIEW_H
#define BP_POST_LIST_VIEW_H


#include <View.h>

class BListView;
class BlogPositiveBlog;
class BMenuItem;
class BMessage;
class BlogPositivePostListView : public BView
{
public:
						BlogPositivePostListView(const char* name,
							BlogPositiveBlog* blog);
			void		AttachedToWindow();
			void		MessageReceived(BMessage* aMessage);
	static	int32		loadList(void* t);
	void				Reload();
private:
	BlogPositiveBlog*	fBlog;
	BMenuItem*			fAddPost;
	BMenuItem*			fRemovePost;
	BListView*			fListView;
};

#endif
