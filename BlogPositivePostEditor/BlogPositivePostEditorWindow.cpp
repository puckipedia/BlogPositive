#include "BlogPositivePostEditorWindow.h"

#include <Rect.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>

#include "../API/BlogPositivePost.h"
#include "BlogPositivePostEditorView.h"

BlogPositivePostEditorWindow::BlogPositivePostEditorWindow(
    BlogPositivePost *aPost, BRect aRect)
    : BWindow(aRect, aPost->Name(), B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
    fView = new BlogPositivePostEditorView("EditorView", aPost);

    SetLayout(new BGroupLayout(B_VERTICAL));
    AddChild(
	BGroupLayoutBuilder(B_VERTICAL, 0)
	.Add(fView));
}
