#pragma once
#include <Window.h>
#include <Message.h>
#include <ListView.h>

class BlogPositiveMainWindow : public BWindow
{
public:
  static BlogPositiveMainWindow *First;
					BlogPositiveMainWindow(BRect frame);
	virtual bool	QuitRequested();
			void	MessageReceived(BMessage *message);
private:
			BListView *fListView;
};
