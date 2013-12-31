/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_METADATA_TYPES_H
#define BP_METADATA_TYPES_H


#include <ObjectList.h>

#include "Metadata.h"

/*class MetadataItem {
public:
	BView*			LabelView();
	BView*			ContentView();
};*/

template<class T>
class MetadataItem {
public:
	virtual	T		Value() { return fValue; };
	virtual	void	SetValue(T value) {fValue = value;};
private:
	T				fValue;
};

class MetadataIntSlider : public MetadataItem<int32> {
public:
					MetadataIntSlider(BString name, BString key,
						int32 max, int32 min);
private:
	int32			fMax;
	int32			fMin;
};

class MetadataFloatSlider : public MetadataItem<float> {
public:
					MetadataFloatSlider(BString name, BString key);
};

typedef StringList BObjectList<const char>*
class MetadataOptionList : public MetadataItem<const char*> {
public:
					MetadataOptionList(BString name, BString key);
					MetadataOptionList(StringList list);
private:
	StringList		fObjectList;
};

#endif
