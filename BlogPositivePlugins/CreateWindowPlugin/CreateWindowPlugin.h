#ifndef BP_CREATE_WINDOW_PLUGIN_H
#define BP_CREATE_WINDOW_PLUGIN_H

#include "../../API/BlogPositivePlugin.h"


class BlogPositivePluginPostListWindow;
class BlogPositivePluginBlogPostWindow;
class BlogPositivePost;
class BlogPostiveBlog;
class BWindow;
class CreateWindowPlugin : public BlogPositivePlugin {
public:
    uint32 Version();
    int32 Type();
    void HookBlogList(BlogPositivePluginPostListWindow **window, BlogPositiveBlog *blog);
    BlogPositivePost *TryGetPost(BWindow *window);
    static int32 loadList(void *t);
    void HookEditor(BlogPositivePluginBlogPostWindow **window, BlogPositivePost *post);
    const char *GetPostContent(BlogPositivePluginBlogPostWindow *window);
};

#endif
