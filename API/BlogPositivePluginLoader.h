/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_PLUGIN_LOADER_H
#define BP_PLUGIN_LOADER_H


class BWindow;
class BList;
class BlogPositiveBlog;
class BlogPositivePost;
class BlogPositivePluginBlogPostWindow;
class BlogPositivePluginLoader {
public:
	static void 	Initialize();
	static void 	FindPlugin(BlogPositiveBlog* blog);
	static BList*	List();
private:
	static BList*	fList;
};

#endif
