#ifndef BP_POST_H
#define BP_POST_H

class BString;
class BlogPositiveBlog;
class BlogPositivePost {
public:
    BlogPositivePost(BlogPositiveBlog *blog);
    BlogPositiveBlog *Blog();
    const char *Name();
    void SetName(const char *name);
    const char *Page();
    void SetPage(const char *page);
    uint32 Pagesize();
 private:
    BString *fName;
    BString *fPage;
    BlogPositiveBlog *fBlog;
};

#endif
