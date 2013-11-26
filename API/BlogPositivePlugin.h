/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef BP_PLUGIN_H
#define BP_PLUGIN_H

#include "BlogPositiveBlog.h"
#include "BlogPositivePost.h"
#include "BlogPositivePluginWindow.h"
#include "../BlogPositiveSettings.h"

#define kBlogPositiveBlogApi 1

//#define kBlogPositiveBlogEditor 2

class BlogPositiveMainView;
class BlogPositivePlugin {
public:
    virtual uint32 Version();
    virtual uint32 MainHandler();
    virtual char *Name();
    virtual int32 Type();
    
    //Implement if kBlogPositiveBlogApi
    virtual bool Supports(int32 Code);
    virtual BList *GetBlogPosts(BlogPositiveBlog *blog); 
    virtual void SavePost(BlogPositivePost *post);
    virtual BlogPositivePost *CreateNewPost(BlogPositiveBlog *blog, const char *name); 
    virtual void RemovePost(BlogPositivePost *post);
    virtual void OpenNewBlogWindow(BlogPositiveMainView *aMainView);
};

#endif
