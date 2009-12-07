/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  IniTools.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 08/11/05.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include "libini.h"

namespace TWS {
	static ini_fd_t inifile;

	void setIniFile(const char *filename) {
		inifile = ini_open("Config/server.ini", "r", NULL);
	}

	void locateHeadingKey(const char *headingName, const char *keyName) {
		ini_locateHeading(inifile, headingName);
		ini_locateKey(inifile, keyName);
	}

	char *stringForHeadingKey(const char *headingName, const char *keyName, char *value, size_t limit) {
		locateHeadingKey(headingName, keyName);
		ini_readString(inifile, value, limit);
	}

	const int intForHeadingKey(const char *headingName, const char *keyName, int *value) {
		locateHeadingKey(headingName, keyName);
		ini_readInt(inifile, value);
	}

	const long longForHeadingKey(const char *headingName, const char *keyName, long *value) {
		locateHeadingKey(headingName, keyName);
		ini_readLong(inifile, value);
	}

	const double doubleForHeadingKey(const char *headingName, const char *keyName, double *value) {
		locateHeadingKey(headingName, keyName);
		ini_readDouble(inifile, value);
	}

	const bool boolForHeadingKey(const char *headingName, const char *keyName, int *value) {
		locateHeadingKey(headingName, keyName);
		ini_readBool(inifile, value);
	}

}
