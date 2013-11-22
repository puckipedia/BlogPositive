// BlogPositiveSettings.h
//

#ifndef LZZ_BlogPositiveSettings_h
#define LZZ_BlogPositiveSettings_h
#include <Message.h>
#include <Path.h>
#include <File.h>
#include <FindDirectory.h>
#include <stdio.h>
#define LZZ_INLINE inline
class BlogPositiveSettings : public BMessage
{
public:
  BlogPositiveSettings (char const * settingsName);
  void Save ();
  static void SaveOther (BMessage * message, char const * settingsName);
private:
  BPath * settingsPath;
};
extern char const * kSettingsFolder;
#undef LZZ_INLINE
#endif
