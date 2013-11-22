#include <stdio.h>
#include <Application.h>
#include <Button.h>
#include <Window.h>
#include <Message.h>
#include "API/BlogPositivePluginLoader.h"
#include "BlogPositiveMain/BlogPositiveMainWindow.h"

class HelloHaiku : public BApplication
{
    public:
    HelloHaiku() : BApplication("application/x-vnd.BlogPositive")
    {
    	BlogPositivePluginLoader::Initialize();
    }

    void ReadyToRun()
    {
		BWindow *win = new BlogPositiveMainWindow(BRect(100, 100, 300, 200));
		win->Show();
    }
};

int main(void)
{
    HelloHaiku app;

    app.Run();

    return 0;
}
