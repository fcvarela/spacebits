/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  FrustumTools.h
 *  TWS
 *
 *  Created by Filipe Varela on 08/10/18.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifdef linux
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

#include "Vertex.h"

namespace TWS {
    struct plane_t
    {
        GLfloat A, B, C, D;
    };
    
    union frustum_t
    {
        struct
        {
            plane_t t, b, l, r, n, f;
        };
        plane_t planes[6];
    };
    
    void extractPlane(plane_t &plane, GLfloat *mat, int row);
    void calculateFrustum();
    bool sphereInFrustum(float *sphere);
}
