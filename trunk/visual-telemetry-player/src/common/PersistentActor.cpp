/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  PersistentActor.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 08/11/05.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include <iostream>

#include "PersistentActor.h"

namespace TWS {
    PersistentActor::PersistentActor() {
#ifndef TWS_SERVER_BUILD
        m_sphere = gluNewQuadric();
#endif
    }
#ifndef TWS_SERVER_BUILD
    void PersistentActor::draw() {
        glTranslatef(xyz[0],xyz[1],xyz[2]);
        gluQuadricNormals(m_sphere, GL_SMOOTH);
        gluSphere(m_sphere, 100, 30, 30);
    }
#endif
    PersistentActor::~PersistentActor() {
#ifndef TWS_SERVER_BUILD
        gluDeleteQuadric(m_sphere);
#endif
    }
}
