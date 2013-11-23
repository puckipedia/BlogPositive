#include "BlogPositivePlugin.h"

#include <TextControl.h>
#include <Message.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <Window.h>
#include <List.h>

class BlogPositiveCreateBlog : public BWindow {
public:
    BlogPositiveCreateBlog();
    void SetBlogHandler(int32 blogHandler);
    void MessageReceived(BMessage *message);
    int32 BlogHandler();
private:
    int32 fBlogHandler;
    BTextControl *fNameControl;
    BTextControl *fAuthControl;
};

BlogPositiveCreateBlog::BlogPositiveCreateBlog()
    : BWindow(BRect(100, 100, 400, 190), "Create Blog", B_MODAL_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0)
{
    fNameControl = new BTextControl("NameControl", "Name: ", "", new BMessage('CBFA'));
    fAuthControl = new BTextControl("AuthControl", "Auth: ", "", new BMessage('CBNB'));
    SetLayout(new BGroupLayout(B_VERTICAL));
    AddChild(BGroupLayoutBuilder(B_VERTICAL, 10).Add(fNameControl).Add(fAuthControl));
  
    fNameControl->MakeFocus();
    fBlogHandler = 'BACN';
}

void
BlogPositiveCreateBlog::SetBlogHandler(int32 blogHandler)
{
    fBlogHandler = bg;
}

void MessageReceived(BMessage *message)
{
    switch(message->what)
    {
    case 'CBFA':
	fAuthControl->MakeFocus();
	break;
    case 'CBNB':
    {
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

uint32 
BlogPositivePlugin::Version() 
{
    return 0;
}

char *
BlogPositivePlugin::Name() 
{
    return "Unknown"; 
}

int32 
BlogPositivePlugin::Type() 
{
    return kBlogPositiveBlogApi; 
}

bool 
BlogPositivePlugin::Supports(int32 Code) 
{
    return false;
}

BlogPositivePost *
BlogPositivePlugin::TryGetPost(BWindow *window) 
{
    return 0; 
}

BWindow *
BlogPositivePlugin::InitializeBlogWindow(BlogPositiveBlog *blog) 
{
    return 0;
}

BList *
BlogPositivePlugin::GetBlogPosts(BlogPositiveBlog *blog) 
{
    return new BList();
}

BlogPositivePost *
BlogPositivePlugin::CreateNewPost(BlogPositiveBlog *blog, const char *name) 
{
    return NULL;
}

void 
BlogPositivePlugin::HookBlogList(BlogPositivePluginPostListWindow **window, BlogPositiveBlog *blog) 
{
}

void 
BlogPositivePlugin::HookEditor(BlogPositivePluginBlogPostWindow **window, BlogPositivePost *post) 
{
}

void 
BlogPositivePlugin::SavePost(BlogPositivePost *post) 
{
}

void 
OpenNewBlogWindow()
{
    (new BlogPositiveCreateBlog())->Show();
}
