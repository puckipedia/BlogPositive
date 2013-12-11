/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_BLOG_H
#define BP_BLOG_H


#include <Archivable.h>
#include <List.h>
#include <Message.h>
#include <ObjectList.h>
#include <String.h>

class BlogPositivePlugin;
class BlogPositiveBlog : public BArchivable {
public:

	static BlogList*		DeserializeList(BMessage* message,
								const char* blogName);
	static BMessage*		SerializeList(BlogList* blist,
								const char* blogName);

							BlogPositiveBlog();
							BlogPositiveBlog(BMessage* from);

	void					SetName(const char* name);
	const char*				Name();

	void					SetAuthentication(const char* authentication);
	const char*				Authentication();

	void					SetBlogHandler(int32 blogHandler);
	int32					BlogHandler();

	void					Unserialize(BMessage* message);

	void					SetPlugin(BlogPositivePlugin* plugin);
	BlogPositivePlugin*		Plugin();

	BString					NameString();

	status_t				Archive(BMessage* into, bool deep = true);
private:
	BString					fName;
	BString					fAuthentication;
	int32					fBlogHandler;
	BlogPositivePlugin*		fPlugin;
};

typedef BObjectList<BlogPositiveBlog> BlogList;

#endif
