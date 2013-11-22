#pragma once
#include <Message.h>
#include <Path.h>
#include <File.h>
#include <FindDirectory.h>
#include <stdio.h>

class BlogPositiveSettings : public BMessage {
public:
	BlogPositiveSettings(const char *settingsName);
	void Save();
	inline static void SaveOther(BMessage *message, const char *settingsName) {
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
private:
	BPath *settingsPath;	
};
