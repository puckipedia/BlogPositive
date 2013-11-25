#include "BlogPositivePostEditorView.h"

#include <TextView.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Message.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>

#include "BlogPositivePostEditorView.h"

#include <TextView.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Message.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>

#include "../API/BlogPositivePost.h"
#include "../API/BlogPositiveBlog.h"
#include "../API/BlogPositivePlugin.h"

const uint32 kPostEditorSavePost = 'PESP';

BlogPositivePostEditorView::BlogPositivePostEditorView(const char *aName, BlogPositivePost *aPost)
    : BView(aName, 0)
{
    
    fTextView = new BTextView("TextView");
    fTextView->SetText(aPost->Page());
    
    BMenuBar *menuBar = new BMenuBar("MenuBar");

    fMenuItem = new BMenuItem("Save", new BMessage(kPostEditorSavePost));

    menuBar->AddItem(fMenuItem);

    SetLayout(new BGroupLayout(B_VERTICAL));
    AddChild(BGroupLayoutBuilder(B_VERTICAL, 0).Add(menuBar).Add(fTextView));
    
    fPost = aPost;
}

void
BlogPositivePostEditorView::AttachedToWindow()
{
    fMenuItem->SetTarget(this);
}

void
BlogPositivePostEditorView::Save()
{
    fPost->SetPage(fTextView->Text());
    fPost->Blog()->Plugin()->SavePost(fPost);
}

void
BlogPositivePostEditorView::MessageReceived(BMessage *aMessage)
{
    aMessage->PrintToStream();
    switch(aMessage->what)
    {
    case kPostEditorSavePost:
	Save();
	break;
    default:
	BView::MessageReceived(aMessage);
    }
}
