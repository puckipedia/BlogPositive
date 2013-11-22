// BlogPositivePluginWindow.cpp
//

#include "BlogPositivePluginWindow.h"
#define LZZ_INLINE inline
uint32 const kPostWindowGetSelection = 'PWgs';
uint32 const kPostWindowSavePost = 'PWsp';
uint32 const kPostWindowNewPost = 'PWnp';
BlogPositivePluginPostListWindow::BlogPositivePluginPostListWindow (BRect rect, char const * Name)
  : BWindow (rect, Name, B_DOCUMENT_WINDOW, 0)
                                                                                                             {}
BlogPositivePluginBlogPostWindow::BlogPositivePluginBlogPostWindow (BRect rect, char const * Name)
  : BWindow (rect, Name, B_DOCUMENT_WINDOW, 0)
                                                                                                             {}
void BlogPositivePluginPostListWindow::SetBlog (BlogPositiveBlog * blog)
                                                                  {
  fBlog = blog;
  printf("%d\n", blog);
}
BlogPositiveBlog * BlogPositivePluginPostListWindow::Blog ()
                                         {
  return fBlog;
}
void BlogPositivePluginPostListWindow::MessageReceived (BMessage * message)
                                                                     {
  switch(message->what) {
  case kPostWindowGetSelection:
    BlogPositivePluginLoader::OpenPostWindowQueryingPlugins(this);
    break;
  default:
    BWindow::MessageReceived(message);
  }
}
void BlogPositivePluginBlogPostWindow::SetPost (BlogPositivePost * post)
                                                                  {
  fPost = post;
}
BlogPositivePost * BlogPositivePluginBlogPostWindow::Post ()
                                         {
  return fPost;
}
void BlogPositivePluginBlogPostWindow::MessageReceived (BMessage * message)
                                                                     {
  switch(message->what) {
  case kPostWindowSavePost:
    Post()->SetPage(BlogPositivePluginLoader::GetPageContent(this));
    Post()->Blog()->GetPlugin()->SavePost(fPost);
    break;
  default:
    BWindow::MessageReceived(message);
  }
}
#undef LZZ_INLINE
