#ifndef BP_PLUGIN_LOADER_H
#define BP_PLUGIN_LOADER_H

class BWindow;
class BList;
class BlogPositiveBlog;
class BlogPositivePost;
class BlogPositivePluginBlogPostWindow;
class CreateWindowPlugin;
class BlogPositivePluginLoader {
public:
    static void Initialize();
    static void LoadWindow(BlogPositiveBlog *blog);
    static void OpenPostWindow(BlogPositivePost *post);
    static void OpenPostWindowQueryingPlugins(BWindow *window);
    static const char *GetPageContent(BlogPositivePluginBlogPostWindow *window);
private:
    static BList *fList;
    static CreateWindowPlugin *fPlugin;
};

#endif
