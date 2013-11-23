#include "BlogPositiveMainWindow.h"
#include "BlogPositiveMainView.h"

#include <Rect.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>

BlogPositiveMainWindow::BlogPositiveMainWindow(BRect frame)
    : BWindow(frame, "BlogPositive", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
    SetLayout(new BGroupLayout(B_VERTICAL));
    AddChild(
	BGroupLayoutBuilder(B_VERTICAL, 0)
	.Add(new BlogPositiveMainView("MainView")));
}
