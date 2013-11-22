#pragma once
#include "BlogPositiveBlog.h"
#include "BlogPositivePlugin.h"
#include <List.h>
#include "../BlogPositivePlugins/CreateWindowPlugin/CreateWindowPlugin.h"
#include "../Example/ExamplePlugin.h"
#include "BlogPositivePluginWindow.h"
#include <stdio.h>
class BlogPositivePluginLoader {
public:
  static BList *List;
  static CreateWindowPlugin *Plugin;
 public:
  inline static void Initialize() {
    List = new BList();
    Plugin = new CreateWindowPlugin();
    List->AddItem(Plugin);
    List->AddItem(new ExamplePlugin());
  }	
  inline static void LoadWindow(BlogPositiveBlog *blog) {
    BlogPositivePlugin *working = 0;
    int i = 0;
    for(; i < List->CountItems(); i++) {
      printf("Trying plugin %d:",i);
      BlogPositivePlugin *plugin = (BlogPositivePlugin *)List->ItemAt(i);
      if(plugin->Type() == kBlogPositiveBlogApi && plugin->Supports(blog->GetBlogHandler())) {
	working = plugin;
	printf(" :D\n");
	break;
      } else {
	printf(" D:\n");
      }
    }
    if(working == 0)
      return;
    BlogPositivePluginPostListWindow *blogWindow = new BlogPositivePluginPostListWindow(BRect(100, 100, 600, 700), blog->GetName());
    blog->SetPlugin(working);
    blogWindow->SetBlog(blog);
    for(i = 0; i < List->CountItems(); i++) {
      BlogPositivePlugin *plugin = (BlogPositivePlugin *)List->ItemAt(i);
      if(plugin->Type() == kBlogPositiveBlogEditor) {
	plugin->HookBlogList(&blogWindow, blog);
      }
    }
    if(blogWindow == 0) {
      return;
    }
    printf("Running %d now\n", blogWindow);
    blogWindow->Run();
    blogWindow->Show();
  }
  inline static void OpenPostWindow(BlogPositivePost *post) {
    BlogPositivePluginBlogPostWindow *blogWindow = new BlogPositivePluginBlogPostWindow(BRect(100, 100, 600, 700), post->Name());
    blogWindow->SetPost(post);
    for(int i = 0; i < List->CountItems(); i++) {
      BlogPositivePlugin *plugin = (BlogPositivePlugin *)List->ItemAt(i);
      if(plugin->Type() == kBlogPositiveBlogEditor) {
	plugin->HookEditor(&blogWindow, post);
      }
    }
    blogWindow->Run();
    blogWindow->Show();
  }
  inline static void OpenPostWindowQueryingPlugins(BWindow *window) {
    for(int i = 0; i < List->CountItems(); i++) {
      BlogPositivePlugin *plugin = (BlogPositivePlugin *)List->ItemAt(i);
      if(plugin->Type() == kBlogPositiveBlogEditor) {
	BlogPositivePost *post = plugin->TryGetPost(window);
	if(post != NULL) {
	  OpenPostWindow(post);
	  return;
	}
      }
    }
  }
  inline static const char *GetPageContent(BlogPositivePluginBlogPostWindow *window) {
    return Plugin->GetPostContent(window);
  }
};
