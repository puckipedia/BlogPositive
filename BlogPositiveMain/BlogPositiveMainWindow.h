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
