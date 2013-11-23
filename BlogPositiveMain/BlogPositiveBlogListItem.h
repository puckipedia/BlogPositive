// BlogPositiveBlogListItem.h
//

#ifndef LZZ_BlogPositiveBlogListItem_h
#define LZZ_BlogPositiveBlogListItem_h
#include <ListItem.h>
#include <Message.h>
#include <View.h>
#include "../API/BlogPositiveBlog.h"
#define LZZ_INLINE inline
class BlogPositiveBlogListItem : public BStringItem
{
public:
  BlogPositiveBlogListItem (BlogPositiveBlog * blog);
  BlogPositiveBlog * Blog ();
private:
  BlogPositiveBlog * fBlog;
};
#undef LZZ_INLINE
#endif
