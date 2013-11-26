/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef BP_MAIN_VIEW_H
#define BP_MAIN_VIEW_H

#include <View.h>

class BListView;
class BMessage;
class BMenuBar;
class BMenu;
class BlogPositiveBlogListView;
class BlogPositiveBlog;
class BPoint;
class BMenuItem;
class BlogPositiveDelegate;
class BlogPositiveMainView : public BView
{
public:
    BlogPositiveMainView(const char *name, BlogPositiveDelegate *del);
    void MessageReceived(BMessage *message);
    void AttachedToWindow();
    void Reload(BList *list);
    void RemoveBlog();
private:
    BMenuBar *fMenuBar;
    BMenu *fNewMenu;
    BMenuItem *fRemoveMenuItem;
    BlogPositiveBlogListView *fListView;
    BlogPositiveDelegate *fDelegate;
};

#endif
