/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_METADATA_H
#define BP_METADATA_H


#include <ObjectList.h>
#include <String.h>
#include <View.h>

class MetadataItem {
public:
	BString	Name();
	void	SetName(BString name);

	BString	Value();
	void	SetValue(BString value);
	
	BString	Key();
	void	SetKey(BString key);
};

class MetadataPair {
public:
	BString key;
	MetadataItem* item;
};

class Metadata {
public:
					Metadata();
	MetadataItem*	GetItem(BString name);
	void			SetItem(BString name, MetadataItem* item);
	
	int32			CountItems();
	MetadataItem*	ItemAt(int32 index);

private:
	int32			_Index(BString name);
	BObjectList<MetadataPair>*	fPairList;
};

#endif
