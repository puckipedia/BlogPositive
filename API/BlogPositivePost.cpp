// BlogPositivePost.cpp
//

#include "BlogPositivePost.h"
#define LZZ_INLINE inline
BlogPositivePost::BlogPositivePost (BlogPositiveBlog * blog)
                                           {
    fName = new BString();
    fPage = new BString();
    fBlog = blog;
  }
BlogPositiveBlog * BlogPositivePost::Blog ()
                           {
    return fBlog;
  }
char const * BlogPositivePost::Name ()
                     {
    return fName->String();
  }
void BlogPositivePost::SetName (char const * name)
                                 {
    fName->SetTo(name);
  }
char const * BlogPositivePost::Page ()
                     {
    return fPage->String();
  }
void BlogPositivePost::SetPage (char const * page)
                                 {
    fPage->SetTo(page);
  }
uint32 BlogPositivePost::Pagesize ()
                    {
    return fPage->Length();
  }
#undef LZZ_INLINE
