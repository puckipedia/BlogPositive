#pragma once
#include <Message.h>
class BPath;
class BlogPositiveSettings : public BMessage {
 public:
  BlogPositiveSettings(const char *settingsName);
  void Save();
  static void SaveOther(BMessage *message, const char *settingsName);
 private:
  BPath *settingsPath;	
};
