/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_SETTINGS_H
#define BP_SETTINGS_H


#include <Message.h>

class BPath;
class BlogPositiveSettings : public BMessage {
public:
				BlogPositiveSettings(const char* settingsName);
				~BlogPositiveSettings();
	void		Save();
	static void	SaveOther(BMessage* message, const char* settingsName);
private:
	BPath*		fSettingsPath;
};

#endif BP_SETTINGS_H
