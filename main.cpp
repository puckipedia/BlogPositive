/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <curl/curl.h>

#include <Application.h>
#include <Button.h>
#include <Message.h>
#include <Window.h>

#include "API/BlogPositivePluginLoader.h"
#include "BlogPositiveMain/BlogPositiveMainWindow.h"


class BlogPositiveApp : public BApplication
{
public:
			BlogPositiveApp();
	void	ReadyToRun();
};


BlogPositiveApp::BlogPositiveApp()
	:
	BApplication("application/x-vnd.BlogPositive")
{
	BlogPositivePluginLoader::Initialize();
}


void
BlogPositiveApp::ReadyToRun()
{
	BWindow* window = new BlogPositiveMainWindow(BRect(100, 100, 600, 200));
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
