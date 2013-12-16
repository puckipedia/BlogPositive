/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "ExamplePlugin.h"

#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <List.h>
#include <Path.h>

#include "BlogPositiveBlog.h"
#include "BlogPositivePost.h"


bool
ExamplePlugin::Supports(int32 Code)
{
	return Code == 'BACN';
}


uint32
ExamplePlugin::MainHandler()
{
	return 'BACN';
}


PostList*
ExamplePlugin::GetBlogPosts(BlogPositiveBlog* blog) {
	PostList* list = new PostList();
	BDirectory Dir(blog->Authentication());
	BEntry entry;

	while (Dir.GetNextEntry(&entry) == B_OK) {
		char name[B_FILE_NAME_LENGTH];
		entry.GetName(name);
		BFile file(&entry, B_READ_ONLY);
		off_t size;
		file.GetSize(&size);
		char* pointer = new char[size];
		file.Read(reinterpret_cast<void*>(pointer), size);
		BlogPositivePost* post = new BlogPositivePost(blog);
		post->SetName(name);
		post->SetPage(reinterpret_cast<const char*>(pointer));
		list->AddItem(post);
	}
	return list;
}


BlogPositivePost*
ExamplePlugin::CreateNewPost(BlogPositiveBlog* blog, const char* name)
{
	BPath path(blog->Authentication());
	path.Append(name);
	BFile file(path.Path(), B_WRITE_ONLY | B_ERASE_FILE | B_CREATE_FILE);
	char a = '\0';
	file.Write(static_cast<void*>(&a), sizeof(a));
	BlogPositivePost* p = new BlogPositivePost(blog);
	p->SetName(name);
	p->SetPage("");
	return p;
}


void
ExamplePlugin::RemovePost(BlogPositivePost* post)
{
	BPath path(post->Blog()->Authentication());
	path.Append(post->Name());
	BEntry entr(path.Path());
	entr.Remove();
}


void
ExamplePlugin::SavePost(BlogPositivePost* post) {
	BPath path(post->Blog()->Authentication());
	path.Append(post->Name());
	BFile file(path.Path(), B_WRITE_ONLY | B_ERASE_FILE);
	char* page = const_cast<char*>(post->Page());
	file.Write(reinterpret_cast<void*>(page), post->PageSize()+1);
}
