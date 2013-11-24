#include "BlogPositivePluginLoader.h"

#include <List.h>
#include <stdio.h>

#include "BlogPositiveBlog.h"
#include "BlogPositivePlugin.h"
#include "../BlogPositivePlugins/CreateWindowPlugin/CreateWindowPlugin.h"
#include "../Example/ExamplePlugin.h"
#include "BlogPositivePluginWindow.h"


BList *BlogPositivePluginLoader::fList;
CreateWindowPlugin *BlogPositivePluginLoader::fPlugin;

void
BlogPositivePluginLoader::Initialize()
{
    fList = new BList();
    fPlugin = new CreateWindowPlugin();
    fList->AddItem(fPlugin);
    fList->AddItem(new ExamplePlugin());
}

void
BlogPositivePluginLoader::FindPlugin(BlogPositiveBlog *aBlog)
{
    BlogPositivePlugin *aWorkingPlugin = 0;
    for(int i = 0; i < fList->CountItems(); i++)
    {
	BlogPositivePlugin *aPlugin = (BlogPositivePlugin *)fList->ItemAt(i);
	if(aPlugin->Type() == kBlogPositiveBlogApi &&
	   aPlugin->Supports(aBlog->BlogHandler()))
	{
	    aWorkingPlugin = aPlugin;
	    break;
	}
    }

    if(aWorkingPlugin == 0)
	return;

    aBlog->SetPlugin(aWorkingPlugin);
}

void
BlogPositivePluginLoader::OpenPostWindow(BlogPositivePost *aPost)
{
    BlogPositivePluginBlogPostWindow *aBlogWindow = 
	new BlogPositivePluginBlogPostWindow(BRect(100, 100, 600, 700), aPost->Name());
    
    aBlogWindow->SetPost(aPost);
    
    for(int i = 0; i < fList->CountItems(); i++) {
	BlogPositivePlugin *plugin = (BlogPositivePlugin *)fList->ItemAt(i);
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
BlogPositivePluginLoader::GetPageContent(BlogPositivePluginBlogPostWindow *aWindow)
{
    return fPlugin->GetPostContent(aWindow);
}
