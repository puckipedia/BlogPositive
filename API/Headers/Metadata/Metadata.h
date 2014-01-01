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
			MetadataItem(BString name, BString value) {fName = name; fValue = value;};

	BString	Name() {return fName;};
	void	SetName(BString name) {fName = name;};

	BString	Value() {return fValue;};
	void	SetValue(BString value) {fValue = value;};
	
	BString	Key() {return fKey;};
	void	SetKey(BString key) {fKey = key;};

private:
	BString	fName;
	BString	fValue;
	BString	fKey;
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
