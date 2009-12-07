/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  ServerFactory.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 09/06/05.
 *  Copyright 2009 Filipe Varela. All rights reserved.
 *
 */

#include "ServerFactory.h"

namespace TWS {
	static Database *gDatabase;

	Database * getDatabase(void) {
	// lock here
	if (!gDatabase)
		gDatabase = new Database();
	// unlock here
	return gDatabase;
	}
}
