// BlogPositivePluginLoader.cpp
//

#include "BlogPositivePluginLoader.h"
#define LZZ_INLINE inline
BList * BlogPositivePluginLoader::List;
CreateWindowPlugin * BlogPositivePluginLoader::Plugin;
BList * BlogPositivePluginLoader::List;
CreateWindowPlugin * BlogPositivePluginLoader::Plugin;
void BlogPositivePluginLoader::Initialize ()
                           {
    List = new BList();
    Plugin = new CreateWindowPlugin();
    List->AddItem(Plugin);
    List->AddItem(new ExamplePlugin());
  }
void BlogPositivePluginLoader::LoadWindow (BlogPositiveBlog * blog)
                                                 {
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
void BlogPositivePluginLoader::OpenPostWindow (BlogPositivePost * post)
                                                     {
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
void BlogPositivePluginLoader::OpenPostWindowQueryingPlugins (BWindow * window)
                                                             {
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
char const * BlogPositivePluginLoader::GetPageContent (BlogPositivePluginBlogPostWindow * window)
                                                                              {
    return Plugin->GetPostContent(window);
  }
#undef LZZ_INLINE
