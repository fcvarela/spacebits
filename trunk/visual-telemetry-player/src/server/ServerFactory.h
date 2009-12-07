/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  ServerFactory.h
 *  TWS
 *
 *  Created by Filipe Varela on 09/06/05.
 *  Copyright 2009 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_SERVERFACTORY
#define _TWS_SERVERFACTORY

#include "Database.h"

namespace TWS {
    Database * getDatabase(void);
}

#endif
