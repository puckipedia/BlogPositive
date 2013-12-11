/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositiveSettings.h"

#include <File.h>
#include <FindDirectory.h>
#include <Path.h>

const char* kSettingsFolder = "BlogPositive";

BlogPositiveSettings::BlogPositiveSettings(const char* name)
	: BMessage()
{
	fSettingsPath = new BPath();
	find_directory(B_USER_SETTINGS_DIRECTORY, fSettingsPath);
	fSettingsPath->Append(kSettingsFolder);
	fSettingsPath->Append(name);

	BFile file(settingsPath->Path(), B_READ_ONLY);
	if (file.InitCheck() == B_OK) {
		Unflatten(&file);
	}
}


void
BlogPositiveSettings::Save()
{
	BFile file(fSettingsPath->Path(),
		B_CREATE_FILE | B_ERASE_FILE | B_WRITE_ONLY);
	if (file.InitCheck() == B_OK) {
		Flatten(&file);
	}
}


void
BlogPositiveSettings::SaveOther(BMessage* message, const char* settingsName)
{
	BPath settingsPath;
	find_directory(B_USER_SETTINGS_DIRECTORY, &settingsPath);
	settingsPath.Append(kSettingsFolder);
	settingsPath.Append(settingsName);

	BFile file(settingsPath.Path(),
		B_CREATE_FILE | B_ERASE_FILE | B_WRITE_ONLY);
	if (file.InitCheck() == B_OK) {
		message->Flatten(&file);
	}
}

~BlogPositiveSettings::BlogPositiveSettings()
{
	delete fSettingsPath;
}
