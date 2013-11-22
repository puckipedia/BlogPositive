// BlogPositivePlugin.cpp
//

#include "BlogPositivePlugin.h"
#define LZZ_INLINE inline
BlogPositiveCreateBlog::BlogPositiveCreateBlog ()
  : BWindow (BRect(100, 100, 400, 190), "Create Blog", B_MODAL_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0)
                                                                                                    {
    fNameControl = new BTextControl("NameControl", "Name: ", "", new BMessage('CBFA'));
    fAuthControl = new BTextControl("AuthControl", "Auth: ", "", new BMessage('CBNB'));
    SetLayout(new BGroupLayout(B_VERTICAL));
    AddChild(BGroupLayoutBuilder(B_VERTICAL, 10).Add(fNameControl).Add(fAuthControl));

    fNameControl->MakeFocus();
    fBlogHandler = 'BACN';
  }
void BlogPositiveCreateBlog::SetBlogHandler (int32 bg)
                                {
    fBlogHandler = bg;
  }
void BlogPositiveCreateBlog::MessageReceived (BMessage * message)
                                          {
    switch(message->what) {
    case 'CBFA':
      fAuthControl->MakeFocus();
      break;
    case 'CBNB': {
      BlogPositiveSettings *settings = new BlogPositiveSettings("bloglist");
      BList *lis = BlogPositiveBlog::DeserializeList(settings, "blogs");
      BlogPositiveBlog *blog = new BlogPositiveBlog();
      blog->SetName(fNameControl->Text());
      blog->SetAuthentication(fAuthControl->Text());
      blog->SetBlogHandler(fBlogHandler);
      lis->AddItem(blog);
      
      BlogPositiveSettings::SaveOther(BlogPositiveBlog::SerializeList(lis, "blogs"), "bloglist");
      Hide();
    }
      break;
    default:
      BWindow::MessageReceived(message);
    }
  }
uint32 BlogPositivePlugin::Version ()
                           {return 0;}
char * BlogPositivePlugin::Name ()
                       {return "Unknown"; }
int32 BlogPositivePlugin::Type ()
                       {return kBlogPositiveBlogApi; }
bool BlogPositivePlugin::Supports (int32 Code)
                                    {return false;}
BlogPositivePost * BlogPositivePlugin::TryGetPost (BWindow * window)
                                                        {return 0; }
BWindow * BlogPositivePlugin::InitializeBlogWindow (BlogPositiveBlog * blog)
                                                                {return 0;}
BList * BlogPositivePlugin::GetBlogPosts (BlogPositiveBlog * blog)
                                                      {return new BList();}
BlogPositivePost * BlogPositivePlugin::CreateNewPost (BlogPositiveBlog * blog, char const * name)
                                                                                    {return NULL;}
void BlogPositivePlugin::HookBlogList (BlogPositivePluginPostListWindow * * window, BlogPositiveBlog * blog)
                                                                                               {}
void BlogPositivePlugin::HookEditor (BlogPositivePluginBlogPostWindow * * window, BlogPositivePost * post)
                                                                                             {}
void BlogPositivePlugin::SavePost (BlogPositivePost * post)
                                                {}
void BlogPositivePlugin::OpenNewBlogWindow ()
                                   {
    (new BlogPositiveCreateBlog())->Show();
  }
#undef LZZ_INLINE
