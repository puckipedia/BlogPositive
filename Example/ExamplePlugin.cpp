#include "ExamplePlugin.h"

#include <Path.h>
#include <Entry.h>
#include <Directory.h>
#include <List.h>
#include <malloc.h>
#include <File.h>
#include <stdio.h>

#include "../API/BlogPositiveBlog.h"
#include "../API/BlogPositivePost.h"

bool
ExamplePlugin::Supports(int32 Code) {
	return Code == 'BACN';
}

uint32
ExamplePlugin::MainHandler()
{
    return 'BACN';
}
BList *
ExamplePlugin::GetBlogPosts(BlogPositiveBlog *blog) {
  BList *list = new BList();
  BDirectory Dir(blog->Authentication());
  BEntry entry;
  
  while(Dir.GetNextEntry(&entry) == B_OK) {
    char name[B_FILE_NAME_LENGTH];
    entry.GetName(name);
    BFile file(&entry, B_READ_ONLY);
    off_t size;
    file.GetSize(&size);
    const char *pointer = (const char *)malloc(size);
    file.Read((void *)pointer, size);
    BlogPositivePost *post = new BlogPositivePost(blog);
    post->SetName((const char *)name);
    post->SetPage(pointer);
    list->AddItem(post);
  }
  return list;
}
BlogPositivePost *
ExamplePlugin::CreateNewPost(BlogPositiveBlog *blog, const char *name)
{
  BPath path(blog->Authentication());
  path.Append(name);
  BFile file(path.Path(), B_WRITE_ONLY | B_ERASE_FILE | B_CREATE_FILE);
  int32 a = '\0\0\0\0';
  printf("Creating new post at path %s, writing 0 in\n", path.Path());
  file.Write((void *)&a, sizeof(a));
  BlogPositivePost *p = new BlogPositivePost(blog);
  p->SetName(name);
  p->SetPage("");
  return p;
}

void
ExamplePlugin::RemovePost(BlogPositivePost *post)
{
    BPath path(post->Blog()->Authentication());
    path.Append(post->Name());
    BEntry entr(path.Path());
    printf("Remove post %s\n", path.Path());
    entr.Remove();
}

void
ExamplePlugin::SavePost(BlogPositivePost *post) {
  BPath path(post->Blog()->Authentication());
  path.Append(post->Name());
  BFile file(path.Path(), B_WRITE_ONLY | B_ERASE_FILE);
  file.Write((void *)post->Page(), post->PageSize()+2);
}
