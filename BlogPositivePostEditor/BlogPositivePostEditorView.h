#ifndef BP_POST_EDITOR_VIEW_H
#define BP_POST_EDITOR_VIEW_H

#include <View.h>

class BlogPositivePost;
class BMenuItem;
class BMessage;
class BTextView;
class BlogPositivePostEditorView : public BView
{
public:
    BlogPositivePostEditorView(const char *aName, BlogPositivePost *aPost);
    void AttachedToWindow();
    void MessageReceived(BMessage *aMessage);
    void Save();
private:
    BMenuItem *fMenuItem;
    BTextView *fTextView;
    BlogPositivePost *fPost;
};

#endif
