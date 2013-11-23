// BlogPositivePluginLoader.h
//

#ifndef LZZ_BlogPositivePluginLoader_h
#define LZZ_BlogPositivePluginLoader_h
#include <List.h>
#include <stdio.h>
#include "../BlogPositivePlugins/CreateWindowPlugin/CreateWindowPlugin.h"
#include "BlogPositivePost.h"
#include "BlogPositiveBlog.h"
#include "BlogPositivePluginWindow.h"
#include "../Example/ExamplePlugin.h"
#define LZZ_INLINE inline
class BlogPositivePluginLoader
{
public:
  static BList * List;
  static CreateWindowPlugin * Plugin;
public:
  static void Initialize ();
  static void LoadWindow (BlogPositiveBlog * blog);
  static void OpenPostWindow (BlogPositivePost * post);
  static void OpenPostWindowQueryingPlugins (BWindow * window);
  static char const * GetPageContent (BlogPositivePluginBlogPostWindow * window);
};
#undef LZZ_INLINE
#endif
