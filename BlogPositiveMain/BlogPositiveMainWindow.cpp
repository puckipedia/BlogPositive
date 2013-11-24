#include "BlogPositiveMainWindow.h"
#include "BlogPositiveMainView.h"

#include <Rect.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <Message.h>
#include <stdio.h>

BlogPositiveMainWindow::BlogPositiveMainWindow(BRect frame)
    : BWindow(frame, "BlogPositive", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
    BlogPositiveMainView *view = new BlogPositiveMainView("MainView");
    AddHandler(view);
    SetLayout(new BGroupLayout(B_VERTICAL));
    AddChild(
	BGroupLayoutBuilder(B_VERTICAL, 0)
	.Add(view));
}

void
BlogPositiveMainWindow::MessageReceived(BMessage *message)
{
    BWindow::MessageReceived(message);
}
