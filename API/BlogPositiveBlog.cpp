// BlogPositiveBlog.cpp
//

#include "BlogPositiveBlog.h"
#define LZZ_INLINE inline
uint32 const kBlogMessage = 'BPBL';
BlogPositiveBlog::BlogPositiveBlog ()
                           {
		fName = new BString();
		fAuthentication = new BString();
	}
BList * BlogPositiveBlog::DeserializeList (BMessage * message, char const * blogName)
                                                                               {
		BMessage msg;
		BList *Listy = new BList();
		int i = 0;
		printf("Deserializing list!\n");
		while(message->FindMessage(blogName, i++, &msg) == B_OK) {
			BlogPositiveBlog *blog = new BlogPositiveBlog();
			blog->Unserialize(&msg);
			printf("    Blog added! %s %s %d\n", blog->GetName(), blog->GetAuthentication(), blog->GetBlogHandler());
			Listy->AddItem(blog);
		}
		printf("Done!\n");
		return Listy;
	}
BMessage * BlogPositiveBlog::SerializeList (BList * blist, char const * blogName)
                                                                           {
		BMessage *bm = new BMessage();
		printf("Serializing! %s\n", blogName);
		bm->AddString("temp", blogName);
		printf("count: %d\n", blist->CountItems());
		blist->DoForEach((bool (*)(void *, void *))BlogPositiveBlog::SerializeOneItem, bm);
		bm->RemoveName("temp");
		printf("Done!\n");
		return bm;
	}
bool BlogPositiveBlog::SerializeOneItem (BlogPositiveBlog * dingA, BMessage * bmessage)
                                                                                  {
		const char *blogName = bmessage->GetString("temp", "");
		printf("Serializing one! %s %s %s %d\n", blogName, dingA->GetName(), dingA->GetAuthentication(), dingA->GetBlogHandler());
		bmessage->AddMessage(blogName, dingA->Serialize());
		return false;
	}
void BlogPositiveBlog::SetName (char const * name)
                                       {
		printf("Set name %s to %s\n", fName->String(), name);
		fName->SetTo(name);
	}
char const * BlogPositiveBlog::GetName ()
                              {
		return fName->String();
	}
void BlogPositiveBlog::SetAuthentication (char const * authentication)
                                                           {
		printf("Set auth %s to %s\n", fAuthentication->String(), authentication);
		fAuthentication->SetTo(authentication);
	}
char const * BlogPositiveBlog::GetAuthentication ()
                                        {
		return fAuthentication->String();
	}
void BlogPositiveBlog::SetBlogHandler (int32 blogHandler)
                                               {
		fBlogHandler = blogHandler;
	}
int32 BlogPositiveBlog::GetBlogHandler ()
                               {
		return fBlogHandler;
	}
BMessage * BlogPositiveBlog::Serialize ()
                              {
		BMessage *message = new BMessage();
		message->what = kBlogMessage;
		message->AddString("name", fName->String());
		message->AddString("auth", fAuthentication->String());
		message->AddInt32("handler", fBlogHandler);
		return message;
	}
void BlogPositiveBlog::Unserialize (BMessage * message)
                                            {
		fName->SetTo(message->GetString("name", ""));
		fAuthentication->SetTo(message->GetString("auth", ""));
		fBlogHandler = message->GetInt32("handler", 0);
	}
void BlogPositiveBlog::SetPlugin (BlogPositivePlugin * plugin)
                                                   {
		fPlugin = plugin;
	}
BlogPositivePlugin * BlogPositiveBlog::GetPlugin ()
                                        {
		return fPlugin;
	}
#undef LZZ_INLINE
