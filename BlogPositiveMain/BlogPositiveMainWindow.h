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
#include "../API/BlogPositiveBlog.h"
#include "../BlogPositiveSettings.h"

#include "../API/BlogPositivePlugin.h"
#include "../API/BlogPositivePluginLoader.h"
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
