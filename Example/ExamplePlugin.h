// ExamplePlugin.h
//

#ifndef LZZ_ExamplePlugin_h
#define LZZ_ExamplePlugin_h
#include "../API/BlogPositivePlugin.h"
#include "../API/BlogPositiveBlog.h"
#include <Window.h>
#include <Directory.h>
#include <TextView.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <ListView.h>
#include <Entry.h>
#include <Path.h>
#include <ScrollView.h>
#include <malloc.h>
#include <File.h>
#include "../API/BlogPositivePost.h"
#define LZZ_INLINE inline
class ExamplePlugin : public BlogPositivePlugin
{
public:
  uint32 Version ();
  bool Supports (int32 Code);
  BList * GetBlogPosts (BlogPositiveBlog * blog);
  void SavePost (BlogPositivePost * post);
};
#undef LZZ_INLINE
#endif
