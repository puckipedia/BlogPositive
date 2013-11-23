#include "BlogPositivePluginLoader.h"

#include "BlogPositiveBlog.h"
#include "BlogPositivePlugin.h"
#include "../BlogPositivePlugins/CreateWindowPlugin/CreateWindowPlugin.h"
#include "../Example/ExamplePlugin.h"
#include "BlogPositivePluginWindow.h"
#include <List.h>


BList *BlogPositivePluginLoader::fList;
CreateWindowPlugin *BlogPositivePluginLoader::fPlugin;

void
BlogPositivePluginLoader::Initialize()
{
    List = new BList();
    Plugin = new CreateWindowPlugin();
    List->AddItem(Plugin);
    List->AddItem(new ExamplePlugin());
}

void
BlogPositivePluginLoader::LoadWindow(BlogPositiveBlog *aBlog)
{
    BlogPositivePlugin *aWorkingPlugin = 0;
    for(int i = 0; i < fList->CountItems(); i++) {
	BlogPositivePlugin *aPlugin = (BlogPositivePlugin *)fList->ItemAt(i);
	if(aPlugin->Type() == kBlogPositiveBlogApi &&
	   aPlugin->Supports(aBlog->GetBlogHandler())) {
	    aWorkingPlugin = aPlugin;
	    break;
	}
    }
    if(aWorkingPlugin == 0)
	return;

    BlogPositivePluginPostListWindow *aBlogWindow = 
	new BlogPositivePluginPostListWindow(BRect(100, 100, 600, 700), aBlog->GetName());

    blog->SetPlugin(aWorkingPlugin);
    aBlogWindow->SetBlog(aBlog);

    for(int i = 0; i < fList->CountItems(); i++) {
	BlogPositivePlugin *plugin = (BlogPositivePlugin *)fList->ItemAt(i);
	if(plugin->Type() == kBlogPositiveBlogEditor) {
	    plugin->HookBlogList(&aBlogWindow, aBlog);
	}
    }

    aBlogWindow->Show();
}

void
BlogPositivePluginLoader::OpenPostWindow(BlogPositivePost *aPost)
{
    BlogPositivePluginBlogPostWindow *aBlogWindow = 
	new BlogPositivePluginBlogPostWindow(BRect(100, 100, 600, 700), post->Name());
    
    aBlogWindow->SetPost(post);
    
    for(int i = 0; i < fList->CountItems(); i++) {
	BlogPositivePlugin *plugin = (BlogPositivePlugin *)List->ItemAt(i);
	if(plugin->Type() == kBlogPositiveBlogEditor) {
	    plugin->HookEditor(&aBlogWindow, aPost);
	}
    }

    aBlogWindow->Show();
}

void
BlogPositivePluginLoader::OpenPostWindowQueryingPlugins(BWindow *aWindow)
{
    for(int i = 0; i < fList->CountItems(); i++) {
	BlogPositivePlugin *aPlugin = (BlogPositivePlugin *)fList->ItemAt(i);
	if(aPlugin->Type() == kBlogPositiveBlogEditor) {
	    BlogPositivePost *aPost = aPlugin->TryGetPost(aWindow);
	    if(aPost != NULL) {
		OpenPostWindow(aPost);
		return;
	    }
	}
    }
}
const char *
BlogPositivePluginLoader::GetPageContent(BlogPositivePluginBlogPostWindow *aWindow) {
    return fPlugin->GetPostContent(aWindow);
}
