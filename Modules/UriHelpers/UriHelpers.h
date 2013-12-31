/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef BP_URI_HELPERS_H
#define BP_URI_HELPERS_H

#include <ObjectList.h>
#include <String.h>

class QueryParameterPart;
class QueryParameters {
public:
	static	QueryParameters ParseParameters(BString parameters);
							QueryParameters();

			BString			String();
			BString			GetParameter(BString name);
			void			SetParameter(BString name, BString value);
private:
			int32			_IndexItem(BString name);
			BString			_Escape(BString string);
			BString			_Unescape(BString string);
			BObjectList<QueryParameterPart>	fParameters;
};

#endif
