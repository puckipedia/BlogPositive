// BlogPositivePlugin.h
//

#ifndef LZZ_BlogPositivePlugin_h
#define LZZ_BlogPositivePlugin_h
#include <Window.h>
#include <List.h>
#include <TextControl.h> 
#include <GroupLayout.h> 
#include <GroupLayoutBuilder.h>
#include <Message.h>
#include "../BlogPositiveSettings.h" 
#include "BlogPositivePluginWindow.h"
#define LZZ_INLINE inline
class BlogPositiveCreateBlog : public BWindow
{
public:
  BlogPositiveCreateBlog ();
  void SetBlogHandler (int32 bg);
  void MessageReceived (BMessage * message);
private:
  int32 fBlogHandler;
  BTextControl * fNameControl;
  BTextControl * fAuthControl;
};
class BlogPositivePlugin
{
public:
  virtual uint32 Version ();
  virtual char * Name ();
  virtual int32 Type ();
  virtual bool Supports (int32 Code);
  virtual BlogPositivePost * TryGetPost (BWindow * window);
  virtual BWindow * InitializeBlogWindow (BlogPositiveBlog * blog);
  virtual BList * GetBlogPosts (BlogPositiveBlog * blog);
  virtual BlogPositivePost * CreateNewPost (BlogPositiveBlog * blog, char const * name);
  virtual void HookBlogList (BlogPositivePluginPostListWindow * * window, BlogPositiveBlog * blog);
  virtual void HookEditor (BlogPositivePluginBlogPostWindow * * window, BlogPositivePost * post);
  virtual void SavePost (BlogPositivePost * post);
  virtual void OpenNewBlogWindow ();
};
#undef LZZ_INLINE
#endif
