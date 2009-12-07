/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  TerrainPatch.cpp
 *  proj_cg
 *
 *  Created by Filipe Varela on 08/10/13.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include <iostream>
#include <math.h>
#include <cstdlib>

#include "TerrainPatch.h"
#include "FrustumTools.h"
#include "Factory.h"
#include "Camera.h"

namespace TWS {

    TerrainPatch::TerrainPatch()
    {
        m_lod = 0;
    }
    
    TerrainPatch::~TerrainPatch()
    {
        free(m_indices[0]);
        free(m_indices[1]);
        free(m_indices[2]);
        free(m_indices[3]);
    }
    
    void TerrainPatch::setVertices(Color4 *verts)
    {
        m_Vertices = verts;
    }
    
    int TerrainPatch::getLOD()
    {
        return m_lod;
    }
    
    // this index will be used to extract the correct vertices from the
    // shared vertex pool
    void TerrainPatch::setIndex(int myIndex) {
        // set my index
        m_index = myIndex;
        
        // our area depends on our index...
        m_vPos = m_index / 16;
        m_hPos = m_index % 16;
        
        // index vector
        m_indices[0] = (GLuint *)malloc(64*2*65*sizeof(GLuint));
        m_indices[1] = (GLuint *)malloc(32*2*33*sizeof(GLuint));
        m_indices[2] = (GLuint *)malloc(16*2*17*sizeof(GLuint));
        m_indices[3] = (GLuint *)malloc(8*2*9*sizeof(GLuint));
        
        int position[4] = {0,0,0,0};
        
        for (int lod=0; lod<4; lod++) {
            int factor = pow(2,lod);
            
            for (int i=0; i<64; i+=factor) {
                for (int j=0; j<65; j+=factor) {
                    int thisPoint = (m_vPos*64+i)*1025 + (m_hPos*64)+j;
                    m_indices[lod][position[lod]] = thisPoint;
                    m_indices[lod][position[lod]+1] = thisPoint + 1025*factor;
                    position[lod]+=2;
                }
            }
        }
    }
    
    void TerrainPatch::draw() {
        // are we in frustum?
        if (!sphereInFrustum(boundingSphere)) {
            rendered = false;
            return;
        }
        
        // how far are we from camera?
        Camera *cam = TWS::getCamera();
        float distanceFromCam = sqrt(
            pow(cam->getPosXYZ(0)-boundingSphere[0],2) +
            pow(cam->getPosXYZ(1)-boundingSphere[1],2) +
            pow(cam->getPosXYZ(2)-boundingSphere[2],2));
            
        distanceFromCam-=boundingSphere[3];
        
        if (distanceFromCam < 6000)
            m_lod = 0;
        else if (distanceFromCam < 12000)
            m_lod = 1;
        else if (distanceFromCam < 16000)
            m_lod = 2;
        else
            m_lod = 3;
        
        int scale = pow(2,m_lod);
        int strips = 64/scale;
        int verts = strips*2+2;
        
        for (int i=0; i<strips; i++)
            glDrawElements(GL_TRIANGLE_STRIP, verts, GL_UNSIGNED_INT, &m_indices[m_lod][i*verts]);
        
        rendered = true;
    }
    
    void TerrainPatch::calcBoundaries() {
        float maxX, maxY, maxZ;
        float minX, minY, minZ;
        float spanX, spanY, spanZ;
        
        maxX = minX = m_Vertices[m_indices[0][0]].r;
        maxY = minY = m_Vertices[m_indices[0][0]].g;
        maxZ = minZ = m_Vertices[m_indices[0][0]].b;
        
        // we only need to test the 4 corners of this patch
        int botleft = 64*64*2-1;
        int botright= 64*65*2-1;
        int topright= 65*2-2;
        int topleft= 0;
        
        // topleft
        maxX = m_Vertices[m_indices[0][topleft]].r > maxX ? m_Vertices[m_indices[0][topleft]].r : maxX;
        maxY = m_Vertices[m_indices[0][topleft]].g > maxY ? m_Vertices[m_indices[0][topleft]].g : maxY;
        maxZ = m_Vertices[m_indices[0][topleft]].b > maxZ ? m_Vertices[m_indices[0][topleft]].b : maxZ;
        minX = m_Vertices[m_indices[0][topleft]].r < minX ? m_Vertices[m_indices[0][topleft]].r : minX;
        minY = m_Vertices[m_indices[0][topleft]].g < minY ? m_Vertices[m_indices[0][topleft]].g : minY;
        minZ = m_Vertices[m_indices[0][topleft]].b < minZ ? m_Vertices[m_indices[0][topleft]].b : minZ;
        
        // topright
        maxX = m_Vertices[m_indices[0][topright]].r > maxX ? m_Vertices[m_indices[0][topright]].r : maxX;
        maxY = m_Vertices[m_indices[0][topright]].g > maxY ? m_Vertices[m_indices[0][topright]].g : maxY;
        maxZ = m_Vertices[m_indices[0][topright]].b > maxZ ? m_Vertices[m_indices[0][topright]].b : maxZ;
        minX = m_Vertices[m_indices[0][topright]].r < minX ? m_Vertices[m_indices[0][topright]].r : minX;
        minY = m_Vertices[m_indices[0][topright]].g < minY ? m_Vertices[m_indices[0][topright]].g : minY;
        minZ = m_Vertices[m_indices[0][topright]].b < minZ ? m_Vertices[m_indices[0][topright]].b : minZ;
        
        // bottomright
        maxX = m_Vertices[m_indices[0][botright]].r > maxX ? m_Vertices[m_indices[0][botright]].r : maxX;
        maxY = m_Vertices[m_indices[0][botright]].g > maxY ? m_Vertices[m_indices[0][botright]].g : maxY;
        maxZ = m_Vertices[m_indices[0][botright]].b > maxZ ? m_Vertices[m_indices[0][botright]].b : maxZ;
        minX = m_Vertices[m_indices[0][botright]].r < minX ? m_Vertices[m_indices[0][botright]].r : minX;
        minY = m_Vertices[m_indices[0][botright]].g < minY ? m_Vertices[m_indices[0][botright]].g : minY;
        minZ = m_Vertices[m_indices[0][botright]].b < minZ ? m_Vertices[m_indices[0][botright]].b : minZ;
        
        // bottomleft
        maxX = m_Vertices[m_indices[0][botleft]].r > maxX ? m_Vertices[m_indices[0][botleft]].r : maxX;
        maxY = m_Vertices[m_indices[0][botleft]].g > maxY ? m_Vertices[m_indices[0][botleft]].g : maxY;
        maxZ = m_Vertices[m_indices[0][botleft]].b > maxZ ? m_Vertices[m_indices[0][botleft]].b : maxZ;
        minX = m_Vertices[m_indices[0][botleft]].r < minX ? m_Vertices[m_indices[0][botleft]].r : minX;
        minY = m_Vertices[m_indices[0][botleft]].g < minY ? m_Vertices[m_indices[0][botleft]].g : minY;
        minZ = m_Vertices[m_indices[0][botleft]].b < minZ ? m_Vertices[m_indices[0][botleft]].b : minZ;
        
        spanX = maxX - minX;
        spanY = maxY - minY;
        spanZ = maxZ - minZ;
        
        if (spanX > spanY && spanX > spanZ)
            boundingSphere[3] = spanX;

        if (spanY > spanX && spanY > spanZ)
            boundingSphere[3] = spanY;

        if (spanZ > spanX && spanZ > spanY)
            boundingSphere[3] = spanZ;
        
        boundingSphere[0] = minX + spanX/2;
        boundingSphere[1] = minY + spanY/2;
        boundingSphere[2] = minZ + spanZ/2;
        
        // boundingSphere[3] contains max span. Max span is not, however, the sphere diameter
        // it is the diameter of a sphere that encompasses all 4 midpoints but not corners.
        // corrent radius is length of vector center->(maxspanX/2,maxSpanY/2,maxSpanZ/2)
        boundingSphere[3] = sqrt(spanX/2.0*spanX/2.0 + spanY/2.0*spanY/2.0 + spanZ/2.0*spanZ/2.0);
    }
};
