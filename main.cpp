/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <stdio.h>
#include <Application.h>
#include <Button.h>
#include <Window.h>
#include <Message.h>
#include "API/BlogPositivePluginLoader.h"
#include "BlogPositiveSharedWindow.h"
#include "BlogPositiveMain/BlogPositiveMainWindow.h"

#define USE_SHARED_WINDOW 0

#if USE_SHARED_WINDOW
#define CLASS BlogPositiveSharedWindow
#else
#define CLASS BlogPositiveMainWindow
#endif

#include <curl/curl.h>

class HelloHaiku : public BApplication
{
    public:
    HelloHaiku() : BApplication("application/x-vnd.BlogPositive")
    {
    	BlogPositivePluginLoader::Initialize();
    }

    void ReadyToRun()
    {
		BWindow *win = new CLASS (BRect(100, 100, 600, 200));
		win->Show();
    }
};

int main(void)
{
    curl_global_init(CURL_GLOBAL_NOTHING);

    HelloHaiku app;

    app.Run();

    curl_global_cleanup();

    return 0;
}
