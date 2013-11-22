// BlogPositiveBlogListItem.cpp
//

#include "BlogPositiveBlogListItem.h"
#define LZZ_INLINE inline
BlogPositiveBlog * BlogPositiveBlogListItem::Blog ()
                                 {return fBlog;}
BlogPositiveBlogListItem::BlogPositiveBlogListItem (BlogPositiveBlog * blog)
  : BStringItem (blog->GetName())
{fBlog = blog;}
#undef LZZ_INLINE
