/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  TerrainPatch.h
 *  
 *
 *  Created by Filipe Varela on 08/10/13.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_TERRAINPATCH
#define _TWS_TERRAINPATCH

#include <vector>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

#ifdef linux
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Vertex.h"

namespace TWS
{           
    class TerrainPatch
    {
        private:
            Color4 *m_Vertices;
            float m_minAlt;
            
            float boundingSphere[4];
            GLuint *m_indices[4];
            GLuint m_VBOindices[4];
            int m_index;
            int m_lod;
            bool m_VOBready;

            // my position in the patch grid
            int m_hPos, m_vPos;
            
            void initVOB();
        public:
            bool rendered;
            TerrainPatch();
            ~TerrainPatch();
            
            void setIndex(int myIndex);
            void setVertices(Color4 *verts);
            int getLOD();
            void draw();
            void calcBoundaries();
    };
}

#endif
