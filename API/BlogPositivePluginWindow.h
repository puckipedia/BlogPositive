// BlogPositivePluginWindow.h
//

#ifndef LZZ_BlogPositivePluginWindow_h
#define LZZ_BlogPositivePluginWindow_h
#include <Window.h>
#include <stdio.h>
#include <Message.h>
#include "BlogPositiveBlog.h"
#include "BlogPositivePost.h"
#include "BlogPositivePluginLoader.h"
#define LZZ_INLINE inline
class BlogPositivePluginPostListWindow : public BWindow
{
public:
  BlogPositivePluginPostListWindow (BRect rect, char const * Name);
  void SetBlog (BlogPositiveBlog * blog);
  BlogPositiveBlog * Blog ();
  void MessageReceived (BMessage * msg);
private:
  BlogPositiveBlog * fBlog;
};
class BlogPositivePluginBlogPostWindow : public BWindow
{
public:
  BlogPositivePluginBlogPostWindow (BRect rect, char const * Name);
  void SetPost (BlogPositivePost * post);
  BlogPositivePost * Post ();
  void MessageReceived (BMessage * msg);
private:
  BlogPositivePost * fPost;
};
#undef LZZ_INLINE
#endif
