#ifndef BP_MAIN_VIEW_H
#define BP_MAIN_VIEW_H

#include <View.h>

class BListView;
class BMessage;
class BlogPositiveMainView : public BView
{
public:
    void MessageReceived(BMessage *message);
private:
    BListView *fListView;
};

#endif
