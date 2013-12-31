/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_METADATA_H
#define BP_METADATA_H


class MetadataItem {
public:
	virtual					MetadataItem(BString name, BString key);
	virtual	BView*			LabelView();
	virtual	BView*			ContentView();
};


class Metadata {
public:
	MetadataItem*	GetItem(BString name);
	BString			GetItemType(BString name);
	void			SetItem(BString name, MetadataItem* item);
};

#endif
