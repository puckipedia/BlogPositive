#pragma once
#include "../API/BlogPositivePlugin.h"
#include "../API/BlogPositiveBlog.h"
#include <Window.h>
class ExamplePlugin : public BlogPositivePlugin {
public:
	inline uint32 Version() {return 0;}
	bool Supports(int32 Code);
	BList *GetBlogPosts(BlogPositiveBlog *blog);
	void SavePost(BlogPositivePost *post);
};
