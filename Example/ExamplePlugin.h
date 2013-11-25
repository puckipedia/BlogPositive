#ifndef BP_EXAMPLE_PLUGIN_H
#define BP_EXAMPLE_PLUGIN_H

#include "../API/BlogPositivePlugin.h"

class BlogPositiveBlog;
class BlogPositivePost;
class BList;
class ExamplePlugin : public BlogPositivePlugin {
public:
    uint32 Version() {return 0;}
    uint32 MainHandler();
    bool Supports(int32 Code);
    BList *GetBlogPosts(BlogPositiveBlog *blog);
    void SavePost(BlogPositivePost *post);
    BlogPositivePost *CreateNewPost(BlogPositiveBlog *blog, const char *name); 
    void RemovePost(BlogPositivePost *post);
};

#endif
