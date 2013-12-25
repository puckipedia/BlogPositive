/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositiveSettings.h"

#include <stdio.h>

#include <Directory.h>
#include <File.h>
#include <FindDirectory.h>
#include <Path.h>
#include <String.h>


const char* kSettingsFolder = "BlogPositive";

BlogPositiveSettings::_Init BlogPositiveSettings::fStatic;
BlogList* gBlogList;

BlogPositiveSettings::BlogPositiveSettings(const char* name)
	: BMessage()
{
	fSettingsPath = new BPath();
	find_directory(B_USER_SETTINGS_DIRECTORY, fSettingsPath);
	fSettingsPath->Append(kSettingsFolder);

	BString dirs(fSettingsPath->Path());
	dirs << "/";

	create_directory(dirs.String(), 0755);
	
	fSettingsPath->Append(name);

	BFile file(fSettingsPath->Path(), B_READ_ONLY);
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


BlogPositiveSettings::~BlogPositiveSettings()
{
	delete fSettingsPath;
}


BlogPositiveSettings::_Init::_Init()
{
	BlogPositiveSettings* settings = new BlogPositiveSettings("bloglist");
	gBlogList = BlogPositiveBlog::DeserializeList(settings, "blogs");
}


void
BlogPositiveSettings::_Init::Destruct()
{
	BlogPositiveSettings::SaveOther(
		BlogPositiveBlog::SerializeList(gBlogList, "blogs"), "bloglist");
}
