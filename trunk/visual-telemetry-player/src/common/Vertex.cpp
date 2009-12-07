/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Vertex.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 08/10/23.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include "Vertex.h"

void geocentricToCarthesian(Color4 *pt, float latitude, float longitude, float altitude)
{
    // meters per degree lat
    // x:longitude
    // z:latitude
    // y:altitude
    pt->r = 111321 * longitude;// * cosf(latitude * DEG2RAD);
    pt->g = pt->a = altitude;
    pt->b = -(110870 * latitude);
}

void carthesianToGeocentric(float *lla, float *xyz)
{
    // altitude = y
    lla[2] = xyz[1];
    
    // latitude = -(z / 110870);
    lla[0] = - (xyz[2] / 110870);
    
    // longitude = x / 111321 / acosf(lla[0]);
    lla[1] = xyz[0] / 111321;// / cosf(lla[0] * DEG2RAD);
}

void geocentricToCarthesianReal(Color4 *pt, float latitude, float longitude, float altitude)
{
    pt->r = cos(longitude * DEG2RAD) * cos(latitude * DEG2RAD) * (GEO_EARTH_RADIUS + altitude);
    pt->g = sin(longitude * DEG2RAD) * cos(latitude * DEG2RAD) * (GEO_EARTH_RADIUS + altitude);
    pt->b = sin(latitude * DEG2RAD) * (GEO_EARTH_RADIUS + altitude);
    pt->a = altitude;
}

/* Support for future spherical coordinate system
void carthesianToGeocentricReal(float *lla, Color4 *pt)
{
    // longitude
    lla[1] = atan2(pt->g, pt->r);
    lla[0] = PI/2.0 - atan2(sqrt(pt->r*pt->r + pt->g*pt->g), pt->b);
    lla[2] = sqrt(pt->r*pt->r + pt->g*pt->g + pt->b*pt->b) - GEO_EARTH_RADIUS;
    
    lla[0] /= DEG2RAD;
    lla[1] /= DEG2RAD;
}*/
