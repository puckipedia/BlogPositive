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
class BlogPositiveMainView : public BView
{
public:
    BlogPositiveMainView(const char *name);
    void MessageReceived(BMessage *message);
    void AttachedToWindow();
    void Reload(BList *list);
    void RemoveBlog();
private:
    BMenuBar *fMenuBar;
    BMenu *fNewMenu;
    BMenuItem *fRemoveMenuItem;
    BlogPositiveBlogListView *fListView;
};

#endif
