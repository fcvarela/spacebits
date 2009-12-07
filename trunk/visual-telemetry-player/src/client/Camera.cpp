/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Camera.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 08/11/05.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include <iostream>

#include "Camera.h"
#include "Vertex.h"

namespace TWS {
	// override node method to clamp altitude
	void Camera::incrementPosLLA(float inclat, float inclon, float incalt) {
		Node::incrementPosLLA(inclat, inclon, incalt);
		if (lla[2] > MAX_CAM_DIST) lla[2] = MAX_CAM_DIST;
		LLA2XYZ();
	}
}
