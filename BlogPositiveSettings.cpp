// BlogPositiveSettings.cpp
//

#include "BlogPositiveSettings.h"
#define LZZ_INLINE inline
void BlogPositiveSettings::SaveOther (BMessage * message, char const * settingsName)
                                                                           {
		BPath *settingsPath = new BPath();
			printf("a: %s\n", settingsPath->Path());
		find_directory(B_USER_SETTINGS_DIRECTORY, settingsPath);
			printf("a: %s\n", settingsPath->Path());
		settingsPath->Append("BlogPositive");
			printf("a: %s\n", settingsPath->Path());
		settingsPath->Append(settingsName);
			printf("a: %s\n", settingsPath->Path());
		
		BFile file(settingsPath->Path(), B_CREATE_FILE | B_ERASE_FILE | B_WRITE_ONLY);
		if(file.InitCheck() == B_OK) {
			message->Flatten(&file);
		}	
	}
char const * kSettingsFolder = "BlogPositive";
BlogPositiveSettings::BlogPositiveSettings (char const * name)
  : BMessage ()
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
void BlogPositiveSettings::Save ()
{
		BFile file(settingsPath->Path(), B_CREATE_FILE | B_ERASE_FILE | B_WRITE_ONLY);
			printf("a: %s\n", settingsPath->Path());
		if(file.InitCheck() == B_OK) {
			Flatten(&file);
		}	
}
#undef LZZ_INLINE
