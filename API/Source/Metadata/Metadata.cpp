/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "Metadata/Metadata.h"

Metadata::Metadata()
{
	fPairList = new BObjectList<MetadataPair>();
}


MetadataItem*
Metadata::GetItem(BString name)
{
	return fPairList->ItemAt(_Index(name))->item;
}


void
Metadata::SetItem(BString name, MetadataItem* item)
{
	int32 index = _Index(name);
	if (index < 0) {
		MetadataPair* pair = new MetadataPair;
		pair->name = name;
		pair->item = item;
		fPairList->AddItem(pair);
	} else {
		MetadataItem* it = fPairList->ItemAt(index)->item;
		fPairList->ItemAt(index)->item = item;
		delete it; // trash it, remove it, etc.
	}
}


int32
Metadata::CountItems()
{
	return fPairList->CountItems();
}


MetadataItem*
Metadata::ItemAt(int32 index)
{
	return fPairList->ItemAt(index)->item;
}


int32
Metadata::_Index(BString name)
{
	for (int32 i = 0; i < fPairList->CountItems(); i++) {
		if (fPairList->ItemAt(i)->name == name)
			return i;
	}
}
