#include "ExamplePlugin.h"

#include <Path.h>
#include <Entry.h>
#include <Directory.h>
#include <List.h>
#include <malloc.h>
#include <File.h>

#include "../API/BlogPositiveBlog.h"
#include "../API/BlogPositivePost.h"

bool
ExamplePlugin::Supports(int32 Code) {
	return Code == 'BACN';
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

void
ExamplePlugin::SavePost(BlogPositivePost *post) {
  BPath path(post->Blog()->Authentication());
  path.Append(post->Name());
  BFile file(path.Path(), B_WRITE_ONLY | B_ERASE_FILE);
  file.Write((void *)post->Page(), post->PageSize()+2);
}
