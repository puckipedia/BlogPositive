/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "BlogPositivePostMetadataWindow.h"

#include <LayoutBuilder.h>
#include <Rect.h>

BlogPositivePostMetadataWindow::BlogPositivePostMetadataWindow(BRect rect,
	Metadata* metadata)
	:
	BWindow(rect, "Metadata", B_FLOATING_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS),
	fMetadata(metadata)
{
	BLayoutBuilder::Grid<> builder(this);
	builder.SetInsets(10, 10, 10, 10);
	for (int32 i = 0; i < metadata->CountItems(); i++) {
		MetadataItem* item = metadata->ItemAt(i);
		BMessage* message = new BMessage('Puck');
		message->AddInt32("index", i);
		BTextControl* control = new BTextControl(item->Key(), item->Name(), item->Value(), NULL);
		control->SetModificationMessage(message);
		builder.AddTextControl(control, 0, i, B_ALIGN_RIGHT);
	}
}


void
BlogPositivePostMetadataWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case 'Puck': {
			MetadataItem* item = fMetadata->ItemAt(message->GetInt32("index", -1));
			BTextControl* control;
			message->FindPointer("source", (void**)&control);
			item->SetValue(control->Text());
			break;
		}
		default:
			BWindow::MessageReceived(message);
	}
};
