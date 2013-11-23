#pragma once
#include "BlogPositiveBlog.h"
#include "BlogPositivePost.h"
#include "BlogPositivePluginWindow.h"
#include "../BlogPositiveSettings.h"
#define kBlogPositiveBlogApi 1
#define kBlogPositiveBlogEditor 2

class BlogPositivePlugin {
public:
  uint32 Version();
  char *Name();
  int32 Type();

  //Implement if kBlogPositiveBlogApi
  bool Supports(int32 Code);
  BList *GetBlogPosts(BlogPositiveBlog *blog); 
  void SavePost(BlogPositivePost *post);
  BlogPositivePost *CreateNewPost(BlogPositiveBlog *blog, const char *name);
  
  //Implement if kBlogPositiveBlogEditor
  BlogPositivePost *TryGetPost(BWindow *window);
  BWindow *InitializeBlogWindow(BlogPositiveBlog *blog);
  void HookBlogList(BlogPositivePluginPostListWindow **window, BlogPositiveBlog *blog);
  void HookEditor(BlogPositivePluginBlogPostWindow **window, BlogPositivePost *post);
  void OpenNewBlogWindow();
};
