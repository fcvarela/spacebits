/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Vertex.h
 *  TWS
 *
 *  Created by Filipe Varela on 08/10/17.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_VERTEX
#define _TWS_VERTEX

#define GEO_EARTH_RADIUS 6371008.7714f
#define PI 3.14159265f
#define DEG2RAD 0.0174532925

#include <math.h>

struct Color4 {
    float r,g,b,a;
    Color4(float r, float g, float b, float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
};

void geocentricToCarthesian(Color4 *pt, float latitude, float longitude, float altitude);
void geocentricToCarthesianReal(Color4 *pt, float latitude, float longitude, float altitude);

void carthesianToGeocentric(float *lla, float *xyz);
void carthesianToGeocentricReal(float *lla, Color4 *pt);

#endif
