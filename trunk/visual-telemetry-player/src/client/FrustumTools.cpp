/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  FrustumTools.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 08/10/18.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include <iostream>
#include <cstdlib>
#include <math.h>i

#include "FrustumTools.h"

namespace TWS
{
    frustum_t globalFrustum;
    
    void extractPlane(plane_t &plane, GLfloat *mat, int row) {
        int scale = (row < 0) ? -1 : 1;
        row = abs(row) - 1;
        
        // calculate plane coefficients from the matrix
        plane.A = mat[3] + scale * mat[row];
        plane.B = mat[7] + scale * mat[row + 4];
        plane.C = mat[11] + scale * mat[row + 8];
        plane.D = mat[15] + scale * mat[row + 12];
        
        // normalize the plane
        float length = sqrtf(plane.A * plane.A + plane.B * plane.B + plane.C * plane.C);
        plane.A /= length;
        plane.B /= length;
        plane.C /= length;
        plane.D /= length;
    }
    
    // determines the current view frustum
    void calculateFrustum() {
        // get the projection and modelview matrices
        GLfloat projection[16];
        GLfloat modelview[16];
        
        glGetFloatv(GL_PROJECTION_MATRIX, projection);
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
        
        // use OpenGL to multiply them
        glPushMatrix();
        glLoadMatrixf(projection);
        glMultMatrixf(modelview);
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
        glPopMatrix();
        
        // extract each plane
        extractPlane(globalFrustum.l, modelview, 1);
        extractPlane(globalFrustum.r, modelview, -1);
        extractPlane(globalFrustum.b, modelview, 2);
        extractPlane(globalFrustum.t, modelview, -2);
        extractPlane(globalFrustum.n, modelview, 3);
        extractPlane(globalFrustum.f, modelview, -3);
    }
    
    bool sphereInFrustum(float *sphere) {
        GLfloat dist;
        for (int i = 0; i < 6; i++)
        {
            dist = globalFrustum.planes[i].A * sphere[0] +
            	   globalFrustum.planes[i].B * sphere[1] +
            	   globalFrustum.planes[i].C * sphere[2] +
            	   globalFrustum.planes[i].D;
            //std::cout << "Distance " << dist << std::endl;
            if (dist <= -sphere[3])
                return false;
        }
        
        return true;
    }
}
