// BlogPositiveMainWindow.h
//

#ifndef LZZ_BlogPositiveMainWindow_h
#define LZZ_BlogPositiveMainWindow_h
#include <Window.h>
#include <Message.h>
#include <ListView.h>
#include <TextView.h>
#include <GroupLayoutBuilder.h>
#include <ScrollView.h>
#include <List.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <stdio.h>
#include <Message.h>
#include <List.h>
#include <stdio.h>
#include <String.h>
#include <Message.h>
#include <Path.h>
#include <File.h>
#include <FindDirectory.h>
#include <stdio.h>
#include <Window.h>
#include <List.h>
#include <TextControl.h> 
#include <GroupLayout.h> 
#include <GroupLayoutBuilder.h>
#include <Message.h>
#include "../BlogPositiveSettings.h" 
#include <List.h>
#include <stdio.h>
#include "../BlogPositivePlugins/CreateWindowPlugin/CreateWindowPlugin.h"
#include "BlogPositivePost.h"
#include "BlogPositiveBlog.h"
#include "BlogPositivePluginWindow.h"
#include "../Example/ExamplePlugin.h"
#define LZZ_INLINE inline
class BlogPositiveMainWindow : public BWindow
{
public:
  static BlogPositiveMainWindow * First;
  BlogPositiveMainWindow (BRect frame);
  virtual bool QuitRequested ();
  void MessageReceived (BMessage * message);
private:
  BListView * fListView;
};
class BlogPositiveBlogListView : public BListView
{
public:
  BlogPositiveBlogListView ();
  void Reload (BList * BlogItemList);
  static bool createItem (void * item, void * p);
};
#undef LZZ_INLINE
#endif
