#include "BlogPositivePostListView.h"

#include <OS.h>
#include <StringItem.h>
#include <ListView.h>

#include "../API/BlogPositiveBlog.h"
#include "../API/BlogPositivePost.h"
#include "../API/BlogPositivePlugin.h"

#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <OS.h>
#include <ScrollView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ListView.h>
#include <TextView.h>

class PostItem : public BStringItem {
public:
  inline PostItem(BlogPositivePost *post) : BStringItem(post->Name()) {
    fPost = post;
  }
  inline BlogPositivePost *Post() {
    return fPost;
  }
 private:
  BlogPositivePost *fPost;
};

class PluginAndWindowThing {
 public:
  BlogPositivePostListView *fWindow;
  BlogPositivePlugin *fPlugin;
  BlogPositiveBlog *fBlog;
  BListView *fView;
};

/*void
CreateWindowPlugin::HookEditor(BlogPositivePluginBlogPostWindow **aWindow, BlogPositivePost *aPost)
{
    BlogPositivePluginBlogPostWindow *win = *aWindow;

    BTextView *aView = new BTextView("TextView");
    aView->SetText(aPost->Page());

    BMenuBar *menuBar = new BMenuBar("MenuBar");

    win->SetLayout(new BGroupLayout(B_VERTICAL));
    win->AddChild(BGroupLayoutBuilder(B_VERTICAL, 0).Add(menuBar).Add(aView));

    BMenu *menu = new BMenu("File");

    menu->AddItem(new BMenuItem("Save", new BMessage(kPostWindowSavePost), 'S'));

    menuBar->AddItem(menu);
}
*/

const uint32 kPostListViewOpenPost = 'PLOP';
const uint32 kPostListViewRemovePost = 'PLRP';
const uint32 kPostListViewNewPost = 'PLNP';

int32
BlogPositivePostListView::loadList(void *t)
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

BlogPositivePostListView::BlogPositivePostListView(const char *aName, BlogPositiveBlog *aBlog)
    : BView(aName, 0)
{
    fBlog = aBlog;
    
    fListView = new BListView("ListView");
    fListView->SetInvocationMessage(new BMessage(kPostListViewOpenPost));

    PluginAndWindowThing *thing = new PluginAndWindowThing();
    thing->fWindow = this;
    thing->fBlog = aBlog;	
    thing->fView = fListView;
    thing->fPlugin = aBlog->Plugin();

    thread_id readThread = spawn_thread(BlogPositivePostListView::loadList, "load_posts", B_NORMAL_PRIORITY, thing);
    resume_thread(readThread);

    SetLayout(new BGroupLayout(B_VERTICAL));
    AddChild(BGroupLayoutBuilder(B_VERTICAL, 10).Add(fListView));

}

void
BlogPositivePostListView::MessageReceived(BMessage *aMessage)
{
    BView::MessageReceived(aMessage);
}

void
BlogPositivePostListView::AttachedToWindow()
{

}
