/*
 *  Factory.h
 *  TWS
 *
 *  Created by Filipe Varela on 08/11/05.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_FACTORY
#define _TWS_FACTORY

#include "Camera.h"

namespace TWS {
    Camera * getCamera(void);
    extern float fps;
}

#endif
