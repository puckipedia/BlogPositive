#include "BlogPositivePost.h"

#include <String.h>

#include "BlogPositiveBlog.h"

BlogPositivePost::BlogPositivePost(BlogPositiveBlog *aBlog)
{
    fName = new BString();
    fPage = new BString();
    fBlog = aBlog;
}

BlogPositiveBlog *
BlogPositivePost::Blog()
{
    return fBlog;
}

void
BlogPositivePost::SetBlog(BlogPositiveBlog *aBlog)
{
    fBlog = aBlog;
}

const char *
BlogPositivePost::Name()
{
    return fName->String();
}

void
BlogPositivePost::SetName(const char *aName)
{
    fName->SetTo(aName);
}

const char *
BlogPositivePost::Page()
{
    return fPage->String();
}

void
BlogPositivePost::SetPage(const char *page)
{
    fPage->SetTo(page);
}

uint32
BlogPositivePost::PageSize()
{
    return fPage->Length();
}
