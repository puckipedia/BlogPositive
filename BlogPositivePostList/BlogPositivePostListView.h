#ifndef BP_POST_LIST_VIEW_H
#define BP_POST_LIST_VIEW_H

#include <View.h>

class BMessage;
class BlogPositiveBlog;
class BListView;
class BlogPositivePostListView : public BView
{
public:
    BlogPositivePostListView(const char *aName, BlogPositiveBlog *aBlog);
    void AttachedToWindow();
    void MessageReceived(BMessage *aMessage);
    static int32 loadList(void *t);
private:
    BlogPositiveBlog *fBlog;
    BListView *fListView;
};

#endif
