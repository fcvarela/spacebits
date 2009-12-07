/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Factory.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 08/11/05.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include "Factory.h"

namespace TWS {
    static Camera *gCamera;
    float fps = 0.0;
    
    Camera * getCamera(void) {
        if (!gCamera)
            gCamera = new Camera();
        
        return gCamera;
    }
}

