// ExamplePlugin.h
//

#ifndef LZZ_ExamplePlugin_h
#define LZZ_ExamplePlugin_h
#include <Window.h>
#include <List.h>
#include <TextControl.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <Message.h>
#include <Message.h>
#include <Path.h>
#include <File.h>
#include <FindDirectory.h>
#include <stdio.h>
#include <Message.h>
#include <List.h>
#include <stdio.h>
#include <String.h>
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
#include <String.h>
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
