/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_POST_METADATA_WINDOW_H
#define BP_POST_METADATA_WINDOW_H

#include <Message.h>
#include <Window.h>

#include "Metadata/Metadata.h"

class BRect;

class BlogPositivePostMetadataWindow : public BWindow {
public:
				BlogPositivePostMetadataWindow(BRect rect,
					Metadata* metadata);
	void		MessageReceived(BMessage* message);
private:
	Metadata*	fMetadata;
};

#endif
