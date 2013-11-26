/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef BP_MAIN_WINDOW_H
#define BP_MAIN_WINDOW_H

#include <Window.h>

class BMessage;
class BlogPositiveMainWindow : public BWindow
{
public:
    BlogPositiveMainWindow(BRect frame);
    void MessageReceived(BMessage *msg);
};

#endif
