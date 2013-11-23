// CreateWindowPlugin.h
//

#ifndef LZZ_CreateWindowPlugin_h
#define LZZ_CreateWindowPlugin_h
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <OS.h>
#include <ScrollView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ListView.h>
#include <TextView.h>
#include "../../API/BlogPositivePluginWindow.h"
#include "../../API/BlogPositivePlugin.h"
#include "../../API/BlogPositivePost.h"
#include "../../API/BlogPositivePluginWindow.h"
#define LZZ_INLINE inline
class PostItem : public BStringItem
{
public:
  PostItem (BlogPositivePost * post);
  BlogPositivePost * Post ();
private:
  BlogPositivePost * fPost;
};
class PluginAndWindowThing
{
public:
  BWindow * fWindow;
  BlogPositivePlugin * fPlugin;
  BlogPositiveBlog * fBlog;
  BListView * fView;
};
class CreateWindowPlugin : public BlogPositivePlugin
{
public:
  uint32 Version ();
  int32 Type ();
  bool Supports (int32 Code);
  void HookBlogList (BlogPositivePluginPostListWindow * * window, BlogPositiveBlog * blog);
  BlogPositivePost * TryGetPost (BWindow * window);
  static int32 loadList (void * t);
  void HookEditor (BlogPositivePluginBlogPostWindow * * window, BlogPositivePost * post);
  char const * GetPostContent (BlogPositivePluginBlogPostWindow * window);
};
LZZ_INLINE PostItem::PostItem (BlogPositivePost * post)
  : BStringItem (post->Name())
                                                                      {
    fPost = post;
  }
LZZ_INLINE BlogPositivePost * PostItem::Post ()
                                  {
    return fPost;
  }
LZZ_INLINE int32 CreateWindowPlugin::Type ()
                      {
    return kBlogPositiveBlogEditor;
  }
LZZ_INLINE bool CreateWindowPlugin::Supports (int32 Code)
                                   {
    return false;
  }
LZZ_INLINE void CreateWindowPlugin::HookBlogList (BlogPositivePluginPostListWindow * * window, BlogPositiveBlog * blog)
                                                                                              {
    BWindow *win = *window;
    BListView *aView = new BListView("ListView");
    BMessage *msg = new BMessage(kPostWindowGetSelection);
    aView->SetInvocationMessage(msg);
    win->SetLayout(new BGroupLayout(B_VERTICAL));
    win->AddChild(BGroupLayoutBuilder(B_VERTICAL, 10).Add(aView));
    PluginAndWindowThing *thing = new PluginAndWindowThing();
    thing->fWindow = *window;
    thing->fBlog = blog;
    thing->fView = aView;
    thing->fPlugin = blog->GetPlugin();
    thread_id readThread = spawn_thread(loadList, "load_posts", B_NORMAL_PRIORITY, thing);
    resume_thread(readThread);
  }
LZZ_INLINE BlogPositivePost * CreateWindowPlugin::TryGetPost (BWindow * window)
                                                       {
    BListView *aView = (BListView *)window->FindView("ListView");
    if(aView != NULL) {
      int32 ChosenOne = aView->CurrentSelection();
      if(ChosenOne == -1) {
	return NULL;
      }
      PostItem *item = (PostItem *)aView->ItemAt(ChosenOne);
      return item->Post();
    }

  }
LZZ_INLINE int32 CreateWindowPlugin::loadList (void * t)
                                        {
    PluginAndWindowThing *thing = (PluginAndWindowThing *)t;
    BList *list = thing->fPlugin->GetBlogPosts(thing->fBlog);
    if(thing->fView->LockLooper()) {
      for(int i = 0; i < list->CountItems(); i++) {
	BlogPositivePost *post = (BlogPositivePost *)list->ItemAt(i);
	thing->fView->AddItem(new PostItem(post));
      }
      thing->fView->UnlockLooper();
    }
  }
LZZ_INLINE void CreateWindowPlugin::HookEditor (BlogPositivePluginBlogPostWindow * * window, BlogPositivePost * post)
                                                                                            {
    BlogPositivePluginBlogPostWindow *win = *window;
    BTextView *aView = new BTextView("TextView");
    aView->SetText(post->Page());
    win->SetLayout(new BGroupLayout(B_VERTICAL));
    win->AddChild(BGroupLayoutBuilder(B_VERTICAL, 10).Add(aView));
    BRect r(win->Bounds());
    r.bottom = 20;
    BMenuBar *menuBar = new BMenuBar(r, "MenuBar");
    win->AddChild(menuBar);
    BMenu *menu = new BMenu("File");
    menu->AddItem(new BMenuItem("Save", new BMessage(kPostWindowSavePost), 'S'));
    menuBar->AddItem(menu);
  }
LZZ_INLINE char const * CreateWindowPlugin::GetPostContent (BlogPositivePluginBlogPostWindow * window)
                                                                              {
    BView *view = window->FindView("TextView");
    BTextView *textView = (BTextView *)view;
    return textView->Text();
  }
#undef LZZ_INLINE
#endif
