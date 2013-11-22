// BlogPositivePost.h
//

#ifndef LZZ_BlogPositivePost_h
#define LZZ_BlogPositivePost_h
#include <String.h>
#define LZZ_INLINE inline
class BlogPositivePost
{
public:
  BlogPositivePost (BlogPositiveBlog * blog);
  BlogPositiveBlog * Blog ();
  char const * Name ();
  void SetName (char const * name);
  char const * Page ();
  void SetPage (char const * page);
  uint32 Pagesize ();
private:
  BString * fName;
  BString * fPage;
  BlogPositiveBlog * fBlog;
};
#undef LZZ_INLINE
#endif
