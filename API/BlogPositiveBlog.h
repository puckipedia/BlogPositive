#ifndef BP_BLOG_H
#define BP_BLOG_H

#include <Message.h>
#include <List.h>

class BlogPositivePlugin;
class BlogPositiveBlog {
 public:
  static BList *DeserializeList(BMessage *message, const char *blogName);
  static BMessage *SerializeList(BList *blist, const char *blogName);
  
  BlogPositiveBlog();
  void SetName(const char *name);
  const char *Name();
  void SetAuthentication(const char *authentication);
  const char *Authentication();
  void SetBlogHandler(int32 blogHandler);
  int32 BlogHandler();
  BMessage *Serialize();
  void Unserialize(BMessage *message);
  void SetPlugin(BlogPositivePlugin *plugin);
  BlogPositivePlugin *Plugin();

 private:
  BString *fName;
  BString *fAuthentication;
  int32 fBlogHandler;
  BlogPositivePlugin *fPlugin;
};

#endif
