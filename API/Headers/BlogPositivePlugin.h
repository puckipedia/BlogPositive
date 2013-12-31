/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_PLUGIN_H
#define BP_PLUGIN_H


#include <ObjectList.h>

class BlogPositivePlugin {
public:
	virtual	uint32				Version();
	virtual	const char*			Name();
};

typedef BObjectList<BlogPositivePlugin> PluginList;

#endif
