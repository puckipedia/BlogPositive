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

	void				Reload();
private:
	static	int32		_loadList(void* t);
	BlogPositiveBlog*	fBlog;
	BMenuItem*			fAddPost;
	BMenuItem*			fRemovePost;
	BListView*			fListView;
};

#endif
