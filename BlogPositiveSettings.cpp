#include "BlogPositiveSettings.h"
#include <Path.h>
#include <File.h>
#include <FindDirectory.h>

const char *kSettingsFolder = "BlogPositive";

BlogPositiveSettings::BlogPositiveSettings(const char *name)
  : BMessage()
{
  settingsPath = new BPath();
  find_directory(B_USER_SETTINGS_DIRECTORY, settingsPath);
  settingsPath->Append(kSettingsFolder);
  settingsPath->Append(name);
  
  BFile file(settingsPath->Path(), B_READ_ONLY);
  if(file.InitCheck() == B_OK) {
    Unflatten(&file);
  }
}

void
BlogPositiveSettings::Save()
{
  BFile file(settingsPath->Path(), B_CREATE_FILE | B_ERASE_FILE | B_WRITE_ONLY);
  if(file.InitCheck() == B_OK) {
    Flatten(&file);
  }	
}

void
BlogPositiveSettings::SaveOther(BMessage *message, const char *settingsName)
{
  BPath *settingsPath = new BPath();
  find_directory(B_USER_SETTINGS_DIRECTORY, settingsPath);
  settingsPath->Append(kSettingsFolder);
  settingsPath->Append(settingsName);
  
  BFile file(settingsPath->Path(), B_CREATE_FILE | B_ERASE_FILE | B_WRITE_ONLY);
  if(file.InitCheck() == B_OK) {
    message->Flatten(&file);
  }
  delete settingsPath;
}
