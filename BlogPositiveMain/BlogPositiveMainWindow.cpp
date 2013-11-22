// BlogPositiveMainWindow.cpp
//

#include "BlogPositiveMainWindow.h"
#define LZZ_INLINE inline
int32 const kBlogSelected = 'BPBS';
BlogPositiveMainWindow * BlogPositiveMainWindow::First;
BlogPositiveBlogListView::BlogPositiveBlogListView ()
  : BListView ("BlogPositiveBlogListView", B_SINGLE_SELECTION_LIST)
                                                                                                    {
	}
void BlogPositiveBlogListView::Reload (BList * BlogItemList)
                                   {
	  BlogItemList->DoForEach(&BlogPositiveBlogListView::createItem, this);

  }
bool BlogPositiveBlogListView::createItem (void * item, void * p)
                                                    {
		BlogPositiveBlogListItem *i = new BlogPositiveBlogListItem((BlogPositiveBlog *)item);
		((BListView *)p)->AddItem((BListItem *)i);
		return false;
	}
void BlogPositiveMainWindow::MessageReceived (BMessage * message)
                                                           {
	if(message->what == kBlogSelected) {
		printf("%d -> %d\n",
			fListView->CurrentSelection(),
				((BlogPositiveBlogListItem *)fListView->ItemAt(fListView->CurrentSelection())));
		BlogPositivePluginLoader::LoadWindow(((BlogPositiveBlogListItem *)fListView->ItemAt(fListView->CurrentSelection()))->Blog());
		return;
	}
	if(message->what == 'ABCD') {
	  int32 m = message->GetInt32("ding", 0);
	  BList *pluginList = BlogPositivePluginLoader::List;
	  BlogPositivePlugin *pl = (BlogPositivePlugin *)pluginList->ItemAt(m);
	  pl->OpenNewBlogWindow();
	  return;
	}
	BWindow::MessageReceived(message);
}
BlogPositiveMainWindow::BlogPositiveMainWindow (BRect frame)
  : BWindow (frame, "BlogPositive", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	
	BMessage *invocationMessage = new BMessage(kBlogSelected);
        BlogPositiveBlogListView *aView;
	BScrollView *sView;
	BRect aRect( Bounds() );
	aView = new BlogPositiveBlogListView();
	aView->SetInvocationMessage(invocationMessage);
	fListView = aView;

	aRect.bottom = 20;
	BMenuBar *menuBar = new BMenuBar("MenuBar");
	BList *pluginList = BlogPositivePluginLoader::List;
	BlogPositiveSettings *settings = new BlogPositiveSettings("bloglist");
	BList *lis = BlogPositiveBlog::DeserializeList(settings, "blogs");
	aView->Reload(lis);
	for(int i = 0; i < pluginList->CountItems(); i++) {
	  BlogPositivePlugin *pl = (BlogPositivePlugin *)pluginList->ItemAt(i);
	  if(pl->Type() == kBlogPositiveBlogApi) {
	    BMessage *msg = new BMessage('ABCD');
	    msg->SetInt32("ding", i);
	    BMenuItem *mi = new BMenuItem(pl->Name(), msg);
	    menuBar->AddItem(mi);
	  }
	}
	SetLayout(new BGroupLayout(B_VERTICAL));
	AddChild(BGroupLayoutBuilder(B_VERTICAL, 0).Add(menuBar).Add(new BScrollView("scroll_view", aView, 0, false, true)));
	
}
bool BlogPositiveMainWindow::QuitRequested ()
{
	return BWindow::QuitRequested();
}
#undef LZZ_INLINE
