#include "WordpressPlugin.h"

#include <String.h>
#include <curl/curl.h>
#include <stdio.h>
#include "../../API/BlogPositiveBlog.h"
#include "xmlnode.h"

uint32
WordpressPlugin::Version()
{
    return 0;
}

char *
WordpressPlugin::Name()
{
    return "Wordpress.com or self-hosted";
}

int32
WordpressPlugin::Type()
{
    return kBlogPositiveBlogApi;
}

uint32
WordpressPlugin::MainHandler()
{
    return 'WoPr';
}

bool
WordpressPlugin::Supports(int32 Code)
{
    return Code == 'WoPr';
}
void WriteTobString(void *bloc, size_t size, size_t nmemb, void *userp)
{
    *((BString *)userp) << (const char *)bloc;
}

BList *
WordpressPlugin::GetBlogPosts(BlogPositiveBlog *aBlog)
{
    BString Auth = BString(aBlog->Authentication());

    int32 SeperatorLocation = Auth.FindFirst("|||");
    int32 LastLocation = Auth.FindLast("|||");
    
    BString username;
    Auth.CopyInto(username, 0, SeperatorLocation);

    BString password;
    Auth.CopyInto(password, SeperatorLocation+3, LastLocation-SeperatorLocation-3);
    
    BString xmlrpcurl;
    Auth.CopyInto(xmlrpcurl, LastLocation+3, Auth.Length()-LastLocation-3);

    /*    tinyxml2::XMLPrinter p;
    p.PushHeader(false, false);

    p.OpenElement("methodCall");
     p.OpenElement("methodName");
      p.PushText("wp.getUsersBlogs");
     p.CloseElement();
     p.OpenElement("params"); 
      p.OpenElement("param");
       p.PushText(username.String());
      p.CloseElement();
      p.OpenElement("param");
       p.PushText(password.String());
      p.CloseElement();
     p.CloseElement();
    p.CloseElement();
    */
    
#define CreateStringValue(name, value) \
    XmlNode * name = new XmlNode(params, "param"); {	\
	XmlNode *val = new XmlNode(name, "value"); \
	XmlNode *string = new XmlNode(val, "string"); \
	string->SetValue(value); \
	val->AddChild(string); \
	name->AddChild(val); \
    }

    XmlNode *root = new XmlNode(NULL, "root");

    XmlNode *mainnode = new XmlNode(root, "methodCall");
    
    XmlNode *methodName = new XmlNode(mainnode, "methodName");
    methodName->SetValue(BString("wp.getUsersBlogs"));
    
    XmlNode *params = new XmlNode(mainnode, "params");

    CreateStringValue(userparam, username);
    CreateStringValue(passparam, password);
    

    root->AddChild(mainnode);
    mainnode->AddChild(methodName);
    mainnode->AddChild(params);
    params->AddChild(userparam);
    params->AddChild(passparam);


    CURL *curl = curl_easy_init();

    BString str;

    curl_easy_setopt(curl, CURLOPT_URL, xmlrpcurl.String());
    BString *stttr = root->SavetoString();
    printf("LOADING POSTS? %s %s %s %s\n", username.String(), password.String(), xmlrpcurl.String(), stttr->String());

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, stttr->String());
    delete stttr;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteTobString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&str);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_perform(curl);

    printf("-%s-\n", str.String());
}

void
WordpressPlugin::SavePost(BlogPositivePost *aPost)
{

}

BlogPositivePost *
WordpressPlugin::CreateNewPost(BlogPositiveBlog *aBlog, const char *aName)
{
    return NULL;
}
