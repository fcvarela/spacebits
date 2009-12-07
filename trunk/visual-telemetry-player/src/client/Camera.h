/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Camera.h
 *  TWS
 *
 *  Created by Filipe Varela on 08/11/05.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_CAMERA
#define _TWS_CAMERA

#define MAX_CAM_DIST 11000000.0

#include "Node.h"

namespace TWS {
	class Camera : public Node {
	public:
		// override increments
		void incrementPosLLA(float inclat, float inclon, float incalt);
	};
}

#endif
