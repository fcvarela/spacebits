/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Node.h
 *  TWS
 *
 *  Created by Filipe Varela on 08/05/21.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_Node
#define _TWS_Node

#ifndef TWS_SERVER_BUILD
    #ifdef __APPLE__
    #include <OpenGL/OpenGL.h>
    #endif

    #ifdef linux
    #include <GL/gl.h>
    #endif
#endif

namespace TWS {
    class Node {
        protected:
            // global coords
            float lla[3];
            float xyz[3];
            float rot[3];
            
        public:
            Node();
            ~Node();
            
            // increments
            virtual void incrementPosLLA(float inclat, float inclon, float incalt);
            virtual void incrementPosXYZ(float incx, float incy, float incz);
            virtual void incrementRotXYZ(float incx, float incy, float incz);
            
            // absolute
            virtual void setPosLLA(float lat, float lon, float alt);
            virtual void setPosXYZ(float x, float y, float z);
            virtual void setRotXYZ(float x, float y, float z);
            
            // accessor
            virtual float getPosLLA(int index) { return lla[index]; }
            virtual float getPosXYZ(int index) { return xyz[index]; }
            virtual float getRotXYZ(int index) { return rot[index]; }
            
            // coordinate system change
            virtual void LLA2XYZ();
            virtual void XYZ2LLA();
            
            // draw
            #ifndef TWS_SERVER_BUILD
            virtual void draw();
            #endif
    };
}

#endif
