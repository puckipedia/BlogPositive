/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef BP_SHARED_WINDOW_H
#define BP_SHARED_WINDOW_H

#include <Window.h>

class BRect;
class BlogPositiveSharedWindow : public BWindow
{
public:
    BlogPositiveSharedWindow(BRect aRect);
};

#endif
