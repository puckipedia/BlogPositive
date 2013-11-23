#ifndef BP_PLUGIN_WINDOW_H
#define BP_PLUGIN_WINDOW_H

#include <Window.h>
#include "BlogPositiveBlog.h"
#include <Message.h>
#include "BlogPositivePost.h"

const uint32 kPostWindowGetSelection = 'PWgs';
const uint32 kPostWindowSavePost = 'PWsp';
const uint32 kPostWindowNewPost = 'PWnp';

class BlogPositivePluginPostListWindow : public BWindow {
 public:
    BlogPositivePluginPostListWindow(BRect rect, const char *Name) : BWindow(rect, Name, B_DOCUMENT_WINDOW, 0) {}
    void SetBlog(BlogPositiveBlog *blog);
    BlogPositiveBlog *Blog();
    void MessageReceived(BMessage *msg);
private:
    BlogPositiveBlog *fBlog;
};
class BlogPositivePluginBlogPostWindow : public BWindow {
public:
    BlogPositivePluginBlogPostWindow(BRect rect, const char *Name) : BWindow(rect, Name, B_DOCUMENT_WINDOW, 0) {}
    void SetPost(BlogPositivePost *post);
    BlogPositivePost *Post();
    void MessageReceived(BMessage *msg);
private:
    BlogPositivePost *fPost;
};

#endif
