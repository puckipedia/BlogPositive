#include "BlogPositivePluginWindow.h"

#include <Window.h>
#include <Message.h>

#include "BlogPositiveBlog.h"
#include "BlogPositivePluginLoader.h"
#include "BlogPositivePost.h"
#include "BlogPositivePlugin.h"

BlogPositivePluginPostListWindow::BlogPositivePluginPostListWindow(BRect rect, const char *Name)
    : BWindow(rect, Name, B_DOCUMENT_WINDOW, 0)
{
    
}

void
BlogPositivePluginPostListWindow::SetBlog(BlogPositiveBlog *blog)
{
    fBlog = blog;
}

BlogPositiveBlog *
BlogPositivePluginPostListWindow::Blog()
{
    return fBlog;
}

void
BlogPositivePluginPostListWindow::MessageReceived(BMessage *message)
{
    switch(message->what)
    {
    case kPostWindowGetSelection:
	BlogPositivePluginLoader::OpenPostWindowQueryingPlugins(this);
	break;
    default:
	BWindow::MessageReceived(message);
    }
}

BlogPositivePluginBlogPostWindow::BlogPositivePluginBlogPostWindow(BRect rect, const char *Name)
 : BWindow(rect, Name, B_DOCUMENT_WINDOW, 0)
{
    
}

void
BlogPositivePluginBlogPostWindow::SetPost(BlogPositivePost *post)
{
    fPost = post;
}

BlogPositivePost *
BlogPositivePluginBlogPostWindow::Post()
{
    return fPost;
}

void
BlogPositivePluginBlogPostWindow::MessageReceived(BMessage *message)
{
    switch(message->what)
    {
    case kPostWindowSavePost:
	Post()->SetPage(BlogPositivePluginLoader::GetPageContent(this));
	Post()->Blog()->Plugin()->SavePost(fPost);
	break;
    default:
	BWindow::MessageReceived(message);
    }
}
