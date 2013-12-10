/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include <Application.h>
#include <Button.h>
#include <Message.h>
#include <Window.h>

#include <stdio.h>

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
			HelloHaiku();
	void	ReadyToRun();
};


HelloHaiku::HelloHaiku()
	:
	BApplication("application/x-vnd.BlogPositive")
{
	BlogPositivePluginLoader::Initialize();
}


void
HelloHaiku::ReadyToRun()
{
	BWindow* window = new CLASS (BRect(100, 100, 600, 200));
	window->Show();
}


int
main()
{
	curl_global_init(CURL_GLOBAL_NOTHING);

	HelloHaiku app;
	app.Run();

	curl_global_cleanup();
	return 0;
}
