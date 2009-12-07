/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Node.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 08/05/21.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include <iostream>

#include "Node.h"
#include "Vertex.h"

namespace TWS {
    Node::Node() {
        lla[0] = lla[1] = lla[2] = 0.0;
        xyz[0] = xyz[1] = xyz[2] = 0.0;
        rot[0] = rot[1] = rot[2] = 0.0;
    }
    
    Node::~Node() {
    }
    
    void Node::LLA2XYZ() {
        Color4 temp = Color4(0,0,0,0);
        geocentricToCarthesian(&temp, lla[0], lla[1], lla[2]);
        setPosXYZ(temp.r, temp.g, temp.b);
    }
    
    void Node::XYZ2LLA() {
        carthesianToGeocentric(lla, xyz);
        setPosLLA(lla[0], lla[1], lla[2]);
    }
    
    void Node::incrementPosLLA(float inclat, float inclon, float incalt) {
        lla[0] += inclat;
        lla[1] += inclon;
        lla[2] += incalt;
        
        // lock latitude
        if (lla[0] > 90.0)
            lla[0] = 90.0;
        if (lla[0] < -90.0)
            lla[0] = -90.0;
            
        if (lla[1] > 180.0)
            lla[1] -= 360.0;
        if (lla[1] < -180.0)
            lla[1] += 360.0;
            
        LLA2XYZ();
    }
    
    void Node::incrementPosXYZ(float incx, float incy, float incz) {
        xyz[0] += incx;
        xyz[1] += incy;
        xyz[2] += incz;
        
        XYZ2LLA();
    }
    
    void Node::incrementRotXYZ(float incx, float incy, float incz) {
        // set
        rot[0] += incx;
        rot[1] += incy;
        rot[2] += incz;
        
        // scale
        if (rot[0] > 90.0)
            rot[0] = 90.0;
            
        if (rot[0] < -90.0)
            rot[0] = -90.0;
            
        if (rot[1] > 180.0)
            rot[1] -= 360.0;
        if (rot[1] < -180.0)
            rot[1] += 360.0;
    }
            
    void Node::setPosLLA(float lat, float lon, float alt) {
        lla[0] = lat; lla[1] = lon; lla[2] = alt;
    }
    
    void Node::setPosXYZ(float x, float y, float z) {
        xyz[0] = x; xyz[1] = y; xyz[2] = z;
    }
    
    void Node::setRotXYZ(float x, float y, float z) {
        rot[0] = x; rot[1] = y; rot[2] = z;
    }
    
    #ifndef TWS_SERVER_BUILD
    void Node::draw(){}
    #endif
}
