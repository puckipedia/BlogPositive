// BlogPositiveBlog.h
//

#ifndef LZZ_BlogPositiveBlog_h
#define LZZ_BlogPositiveBlog_h
#include <Message.h>
#include <List.h>
#include <stdio.h>
#include <String.h>
#define LZZ_INLINE inline
class BlogPositiveBlog
{
public:
  BlogPositiveBlog ();
  static BList * DeserializeList (BMessage * message, char const * blogName);
  static BMessage * SerializeList (BList * blist, char const * blogName);
  static bool SerializeOneItem (BlogPositiveBlog * dingA, BMessage * bmessage);
  void SetName (char const * name);
  char const * GetName ();
  void SetAuthentication (char const * authentication);
  char const * GetAuthentication ();
  void SetBlogHandler (int32 blogHandler);
  int32 GetBlogHandler ();
  BMessage * Serialize ();
  void Unserialize (BMessage * message);
  void SetPlugin (BlogPositivePlugin * plugin);
  BlogPositivePlugin * GetPlugin ();
private:
  BString * fName;
  BString * fAuthentication;
  int32 fBlogHandler;
  BlogPositivePlugin * fPlugin;
};
#undef LZZ_INLINE
#endif
