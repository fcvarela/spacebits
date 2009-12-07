/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  PersistentActor.h
 *  TWS
 *
 *  Created by Filipe Varela on 08/11/05.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_PERSISTENTACTOR
#define _TWS_PERSISTENTACTOR

#include "BaseActor.h"

#ifndef TWS_SERVER_BUILD
    #ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #endif

    #ifdef linux
    #include <GL/gl.h>
    #include <GL/glu.h>
    #endif
#endif

// no persistent logic here. that is handled by the server
// class naming is just a helper

namespace TWS {
    class PersistentActor : public BaseActor {
    public:
        PersistentActor();
        ~PersistentActor();
#ifndef TWS_SERVER_BUILD
        void draw();
#endif
    private:
#ifndef TWS_SERVER_BUILD
        GLUquadric *m_sphere;
#endif
    };
}

#endif
