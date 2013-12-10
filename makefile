NAME = BlogPositive
TYPE = APP
APP_MIME_SIG = x.vnd.blogpositive
SRCS =	API/BlogPositiveBlog.cpp API/BlogPositivePlugin.cpp \
	API/BlogPositivePluginLoader.cpp \
	API/BlogPositivePost.cpp \
	BlogPositiveMain/BlogPositiveBlogListItem.cpp BlogPositiveMain/BlogPositiveMainView.cpp \
	BlogPositiveMain/BlogPositiveMainWindow.cpp \
	BlogPositivePlugins/WordpressPlugin/WordpressPlugin.cpp \
	BlogPositivePlugins/WordpressPlugin/xmlnode.cpp \
	BlogPositivePlugins/WordpressPlugin/XmlRpcWrapper.cpp \
	BlogPositivePostEditor/BlogPositivePostEditorView.cpp \
	BlogPositivePostEditor/BlogPositivePostEditorWindow.cpp \
	BlogPositivePostList/BlogPositivePostListView.cpp \
	BlogPositivePostList/BlogPositivePostListWindow.cpp \
	BlogPositiveSettings.cpp Example/ExamplePlugin.cpp \
	main.cpp

LIBS = be root curl network z textencoding localestub
RSRCS = haiku.rsrc
LOCALES = en

SYSTEM_INCLUDE_PATHS = $(BUILDHOME)/headers/curl

include $(BUILDHOME)/etc/makefile-engine