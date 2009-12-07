/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  IniTools.h
 *  TWS
 *
 *  Created by Filipe Varela on 08/11/05.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_INITOOLS
#define _TWS_INITOOLS

namespace TWS {
	void setIniFile(const char *filename);
	void locateHeadingKey(const char *headingName, const char *keyName);
	const char *stringForHeadingKey(const char *headingName, const char *keyName, char *value, size_t limit);
	const int intForHeadingKey(const char *headingName, const char *keyName, int *value);
	const long longForHeadingKey(const char *headingName, const char *keyName, long *value);
	const double doubleForHeadingKey(const char *headingName, const char *keyName, double *value);
	const bool boolForHeadingKey(const char *headingName, const char *keyName, bool *value);
}

#endif
