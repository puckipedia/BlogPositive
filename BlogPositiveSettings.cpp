#include "BlogPositiveSettings.h"

const char *kSettingsFolder = "BlogPositive";

BlogPositiveSettings::BlogPositiveSettings(const char *name)
												: BMessage()
{
		settingsPath = new BPath();
			printf("a: %s\n", settingsPath->Path());
		find_directory(B_USER_SETTINGS_DIRECTORY, settingsPath);
			printf("a: %s\n", settingsPath->Path());
		settingsPath->Append(kSettingsFolder);
			printf("a: %s\n", settingsPath->Path());
		settingsPath->Append(name);
			printf("a: %s\n", settingsPath->Path());
		
		BFile file(settingsPath->Path(), B_READ_ONLY);
		if(file.InitCheck() == B_OK) {
			Unflatten(&file);
		}
}

void
BlogPositiveSettings::Save()
{
		BFile file(settingsPath->Path(), B_CREATE_FILE | B_ERASE_FILE | B_WRITE_ONLY);
			printf("a: %s\n", settingsPath->Path());
		if(file.InitCheck() == B_OK) {
			Flatten(&file);
		}	
}
