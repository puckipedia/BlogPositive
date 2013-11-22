// BlogPositivePluginLoader.h
//

#ifndef LZZ_BlogPositivePluginLoader_h
#define LZZ_BlogPositivePluginLoader_h
#include <List.h>
#include <stdio.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <OS.h>
#include <ScrollView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ListView.h>
#include <TextView.h>
#include <Window.h>
#include <stdio.h>
#include <Message.h>
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
class BlogPositivePluginLoader
{
public:
  static BList * List;
  static CreateWindowPlugin * Plugin;
public:
  static void Initialize ();
  static void LoadWindow (BlogPositiveBlog * blog);
  static void OpenPostWindow (BlogPositivePost * post);
  static void OpenPostWindowQueryingPlugins (BWindow * window);
  static char const * GetPageContent (BlogPositivePluginBlogPostWindow * window);
};
#undef LZZ_INLINE
#endif
