/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef BP_POST_LIST_WINDOW_H
#define BP_POST_LIST_WINDOW_H

#include <Window.h>

class BlogPositiveBlog;
class BRect;
class BlogPositivePostListView;
class BlogPositivePostListWindow : public BWindow
{
public:
    BlogPositivePostListWindow(BlogPositiveBlog *blog, BRect rect);
private:
    BlogPositivePostListView *fListView;
};

#endif