#include "BlogPositivePluginWindow.h"
#include "BlogPositivePluginLoader.h"
#include <stdio.h>

void
BlogPositivePluginPostListWindow::SetBlog(BlogPositiveBlog *blog) {
  fBlog = blog;
  printf("%d\n", blog);
}

BlogPositiveBlog *
BlogPositivePluginPostListWindow::Blog() {
  return fBlog;
}

void
BlogPositivePluginPostListWindow::MessageReceived(BMessage *message) {
  switch(message->what) {
  case kPostWindowGetSelection:
    BlogPositivePluginLoader::OpenPostWindowQueryingPlugins(this);
    break;
  default:
    BWindow::MessageReceived(message);
  }
}

void
BlogPositivePluginBlogPostWindow::SetPost(BlogPositivePost *post) {
  fPost = post;
}


BlogPositivePost *
BlogPositivePluginBlogPostWindow::Post() {
  return fPost;
}


void
BlogPositivePluginBlogPostWindow::MessageReceived(BMessage *message) {
  switch(message->what) {
  case kPostWindowSavePost:
    Post()->SetPage(BlogPositivePluginLoader::GetPageContent(this));
    Post()->Blog()->GetPlugin()->SavePost(fPost);
    break;
  default:
    BWindow::MessageReceived(message);
  }
}
