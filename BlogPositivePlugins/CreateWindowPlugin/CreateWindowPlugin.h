// CreateWindowPlugin.h
//

#ifndef LZZ_CreateWindowPlugin_h
#define LZZ_CreateWindowPlugin_h
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <OS.h>
#include <ScrollView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ListView.h>
#include <TextView.h>
#include "../../API/BlogPositivePluginWindow.h"
#include "../../API/BlogPositivePlugin.h"
#include "../../API/BlogPositivePost.h"
#include "../../API/BlogPositivePluginWindow.h"
#define LZZ_INLINE inline
class PostItem : public BStringItem
{
public:
  PostItem (BlogPositivePost * post);
  BlogPositivePost * Post ();
private:
  BlogPositivePost * fPost;
};
class PluginAndWindowThing
{
public:
  BWindow * fWindow;
  BlogPositivePlugin * fPlugin;
  BlogPositiveBlog * fBlog;
  BListView * fView;
};
class CreateWindowPlugin : public BlogPositivePlugin
{
public:
  uint32 Version ();
  int32 Type ();
  bool Supports (int32 Code);
  void HookBlogList (BlogPositivePluginPostListWindow * * window, BlogPositiveBlog * blog);
  BlogPositivePost * TryGetPost (BWindow * window);
  static int32 loadList (void * t);
  void HookEditor (BlogPositivePluginBlogPostWindow * * window, BlogPositivePost * post);
  char const * GetPostContent (BlogPositivePluginBlogPostWindow * window);
};
#undef LZZ_INLINE
#endif
