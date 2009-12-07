/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Terrain.h
 *  
 *
 *  Created by Filipe Varela on 08/10/13.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_TERRAIN
#define _TWS_TERRAIN

#define TEXTURE_REPEAT 1

#include <vector>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#ifdef linux
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif

#include "TerrainPatch.h"

namespace TWS
{
    class TerrainTile
    {
        private:
            // holds patches per LOD
            std::vector<TerrainPatch *> m_Patches;
            Color4 *m_points2,*m_normals;
            GLuint VBO_vertexs;
            GLuint VBO_normals;
            int m_baseCoords[2];
            int m_position;
            
            // loads raw data from file.
            // creates patches for each LOD
            void calcNormals();
            void crossProduct(double *normalvec, double *firstvec, double *secondvec);
            void setNormalAtPos(int pos, double *color);
            void vectorFromTo(double *vec, Color4 &pt1, Color4 &pt2);
            
            short *pointBufferFromFile(char *fileName);
            
        public:
            bool readyToDraw,needToCopyVBOData;
            float baseLat();
            float baseLon();
            void draw();
            void loadFromFile();
            void setPosition(int position);
            TerrainTile(int baseLatitude, int baseLongitude);
            ~TerrainTile();
    };
}

#endif
