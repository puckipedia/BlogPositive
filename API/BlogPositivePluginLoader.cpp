#include "BlogPositivePluginLoader.h"

#include <List.h>
#include <stdio.h>

#include "BlogPositiveBlog.h"
#include "BlogPositivePlugin.h"
#include "../Example/ExamplePlugin.h"
#include "BlogPositivePluginWindow.h"
#include "../BlogPositivePlugins/WordpressPlugin/WordpressPlugin.h"

BList *BlogPositivePluginLoader::fList;

void
BlogPositivePluginLoader::Initialize()
{
    fList = new BList();
    fList->AddItem(new ExamplePlugin());
    fList->AddItem(new WordpressPlugin());
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
