#include "CreateWindowPlugin.h"

uint32
CreateWindowPlugin::Version()
{
    return 0;
}

int32
CreateWindowPlugin::Type()
{
    return kBlogPositiveBlogEditor;
}

void
CreateWindowPlugin::HookBlogList(BlogPositivePluginPostListWindow **aWindow, BlogPositiveBlog *aBlog)
{
    BWindow *win = *aWindow;
    BListView *aView = new BListView("ListView");
    BMessage *msg = new BMessage(kPostWindowGetSelection);
    aView->SetInvocationMessage(msg);
    win->SetLayout(new BGroupLayout(B_VERTICAL));
    win->AddChild(BGroupLayoutBuilder(B_VERTICAL, 10).Add(aView));
    PluginAndWindowThing *thing = new PluginAndWindowThing();
    thing->fWindow = *window;
    thing->fBlog = blog;	
    thing->fView = aView;
    thing->fPlugin = aBlog->GetPlugin();
    thread_id readThread = spawn_thread(loadList, "load_posts", B_NORMAL_PRIORITY, thing);
    resume_thread(readThread);
}

BlogPositivePost *
CreateWindowPlugin::TryGetPost(BWindow *aWindow)
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

int32
CreateWindowPlugin::loadList(void *t)
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

void
CreateWindowPlugin::HookEditor(BlogPositivePluginBlogPostWindow **aWindow, BlogPositivePost *aPost)
{
    BlogPositivePluginBlogPostWindow *win = *aWindow;
    BTextView *aView = new BTextView("TextView");
    aView->SetText(aPost->Page());
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

const char *
CreateWindowPlugin::GetPostContent(BlogPositivePluginBlogPostWindow *aWindow)
{
    BView *aView = aWindow->FindView("TextView");
    BTextView *aTextView = (BTextView *)aView;
    return aTextView->Text();
}
