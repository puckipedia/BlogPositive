#ifndef BP_POST_EDITOR_WINDOW_H
#define BP_POST_EDITOR_WINDOW_H

#include <Window.h>

class BRect;
class BlogPositivePost;
class BlogPositivePostEditorView;
class BlogPositivePostEditorWindow : public BWindow
{
public:
    BlogPositivePostEditorWindow(BlogPositivePost *aPost, BRect aRect);
private:
    BlogPositivePostEditorView *fView;
};

#endif
