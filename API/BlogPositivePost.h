#ifndef BP_POST_H
#define BP_POST_H

#include <SupportDefs.h>

class BString;
class BlogPositiveBlog;
class BlogPositivePost {
public:
    BlogPositivePost() {}
    BlogPositivePost(BlogPositiveBlog *blog);
    BlogPositiveBlog *Blog();
    void SetBlog(BlogPositiveBlog *blog);
    const char *Name();
    void SetName(const char *name);
    const char *Page();
    void SetPage(const char *page);
    uint32 PageSize();
protected:
    BString *fName;
    BString *fPage;
    BlogPositiveBlog *fBlog;
};

#endif
