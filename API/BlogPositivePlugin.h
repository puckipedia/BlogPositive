#ifndef BP_PLUGIN_H
#define BP_PLUGIN_H

#include "BlogPositiveBlog.h"
#include "BlogPositivePost.h"
#include "BlogPositivePluginWindow.h"
#include "../BlogPositiveSettings.h"
#define kBlogPositiveBlogApi 1
#define kBlogPositiveBlogEditor 2

class BlogPositivePlugin {
public:
  virtual uint32 Version();
  virtual char *Name();
  virtual int32 Type();

  //Implement if kBlogPositiveBlogApi
  virtual bool Supports(int32 Code);
  virtual BList *GetBlogPosts(BlogPositiveBlog *blog); 
  virtual void SavePost(BlogPositivePost *post);
  virtual BlogPositivePost *CreateNewPost(BlogPositiveBlog *blog, const char *name);
  
  //Implement if kBlogPositiveBlogEditor
  virtual BlogPositivePost *TryGetPost(BWindow *window);
  virtual BWindow *InitializeBlogWindow(BlogPositiveBlog *blog);
  virtual void HookBlogList(BlogPositivePluginPostListWindow **window, BlogPositiveBlog *blog);
  virtual void HookEditor(BlogPositivePluginBlogPostWindow **window, BlogPositivePost *post);
  virtual void OpenNewBlogWindow();
};

#endif
