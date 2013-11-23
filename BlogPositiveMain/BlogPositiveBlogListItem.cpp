#include "BlogPositiveBlogListItem.h"

#include <View.h>

BlogPositiveBlogListItem::BlogPositiveBlogListItem(BlogPositiveBlog *blog)
    : BStringItem(blog->Name())
{
    fBlog = blog;
}
