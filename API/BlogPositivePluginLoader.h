#ifndef BP_PLUGIN_LOADER_H
#define BP_PLUGIN_LOADER_H

class BWindow;
class BList;
class BlogPositiveBlog;
class BlogPositivePost;
class BlogPositivePluginBlogPostWindow;
class BlogPositivePluginLoader {
public:
    static void Initialize();
    static void FindPlugin(BlogPositiveBlog *blog);
    static BList *fList;
private:
};

#endif
