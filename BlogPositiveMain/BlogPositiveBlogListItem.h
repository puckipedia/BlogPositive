#pragma once
#include <ListItem.h>
#include <Message.h>
#include "../API/BlogPositiveBlog.h"

class BlogPositiveBlogListItem : public BStringItem {
public:
	BlogPositiveBlogListItem(BlogPositiveBlog *blog);
	inline BlogPositiveBlog *Blog() {return fBlog;}
private:
	BlogPositiveBlog *fBlog;
};
