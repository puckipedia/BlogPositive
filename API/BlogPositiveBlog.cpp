/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositiveBlog.h"

#include <List.h>
#include <ObjectList.h>
#include <String.h>

#include "BlogPositivePlugin.h"

BlogPositiveBlog::BlogPositiveBlog()
{
}


BlogList*
BlogPositiveBlog::DeserializeList(BMessage* message, const char* blogName)
{
	BMessage msg;
	BlogList* Listy = new BlogList();
	int i = 0;
	while (message->FindMessage(blogName, i++, &msg) == B_OK)
	{
		BlogPositiveBlog* blog = new BlogPositiveBlog(&msg);
		Listy->AddItem(blog);
	}
	return Listy;
}


BMessage*
BlogPositiveBlog::SerializeList(BlogList* blist,
	const char* blogName)
{
	BMessage* bm = new BMessage();
	for (int i = 0; i < blist->CountItems(); i++)
	{
		BMessage resultMessage;
		blist->ItemAt(i)->Archive(&resultMessage);
		bm->AddMessage(blogName, &resultMessage);
	}
	return bm;
}


void
BlogPositiveBlog::SetName(const char* name)
{
	fName = name;
}


const char*
BlogPositiveBlog::Name()
{
	return fName.String();
}


void
BlogPositiveBlog::SetAuthentication(const char* auth)
{
	fAuthentication = auth;
}


const char*
BlogPositiveBlog::Authentication()
{
	return fAuthentication.String();
}


void
BlogPositiveBlog::SetBlogHandler(int32 blogHandler)
{
	fBlogHandler = blogHandler;
}


int32
BlogPositiveBlog::BlogHandler()
{
	return fBlogHandler;
}


status_t
BlogPositiveBlog::Archive(BMessage* into, bool deep = true)
{
	into->AddString("name", fName);
	into->AddString("auth", fAuthentication);
	into->AddInt32("handler", fBlogHandler);
	return B_OK;
}


BlogPositiveBlog::BlogPositiveBlog(BMessage* message)
{
	fName = message->GetString("name", "");
	fAuthentication = message->GetString("auth", "");
	fBlogHandler = message->GetInt32("handler", 0);
}


void
BlogPositiveBlog::SetPlugin(BlogPositivePlugin* plugin)
{
	fPlugin = plugin;
}


BlogPositivePlugin*
BlogPositiveBlog::Plugin()
{
	return fPlugin;
}


BString
BlogPositiveBlog::NameString()
{
	return fName;
}
