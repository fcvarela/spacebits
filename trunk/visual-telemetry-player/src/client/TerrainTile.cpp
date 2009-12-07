/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Terrain.cpp
 *  
 *
 *  Created by Filipe Varela on 08/10/13.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#pragma mark IMPORTANT-ALL TILES SHARE INDICES

#include <iostream>
#include <netinet/in.h>
#include <math.h>
#include <cstdlib>
#include "TerrainTile.h"

namespace TWS
{
    float TerrainTile::baseLat() {
        return m_baseCoords[0];
    }
    
    float TerrainTile::baseLon() {
        return m_baseCoords[1];
    }
    
    void TerrainTile::draw() {
        if (needToCopyVBOData == true) {
            std::cout << "TWS::TerrainTile " << m_baseCoords[0] << "," << m_baseCoords[1] << " copying data into VBOs" << std::endl;
            // bind VBOs and copy data
            glBindBuffer(GL_ARRAY_BUFFER, VBO_vertexs);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Color4) * 1025 * 1025, m_points2, GL_DYNAMIC_DRAW);
        
            glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Color4) * 1025 * 1025, m_normals, GL_DYNAMIC_DRAW);
            
            free(m_points2);
            free(m_normals);
            
            readyToDraw = true;
            needToCopyVBOData = false;
        }
        
        if (!readyToDraw)
            return;
        
        // prepare vertex VBO
        glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_vertexs);
        glVertexPointer(3, GL_FLOAT, 16, 0);
        
        // prepare normals VBO
        glEnableClientState(GL_NORMAL_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
        glNormalPointer(GL_FLOAT, 16, 0);
        
        // draw
        for (int i=0; i<m_Patches.size(); i++)
            m_Patches.at(i)->draw();
        
        // reset client states
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    // this function desperately needs serious error checking
    // i do NOT want the app to blow because of a missing file!
    short *TerrainTile::pointBufferFromFile(char *fileName) {
        // allocate buffer to hold uncompressed file contents
        short *buff = (short *)malloc(1201*1201*2);
        
        // now use a pipe to read the file
        FILE *fp = popen(fileName, "r");
        fread(buff, 2, 1201*1201, fp);
        pclose(fp);
        
        return buff;
    }
    
    #pragma mark LOAD IN SEPARATE THREAD
    TerrainTile::TerrainTile(int baseLatitude, int baseLongitude) {
        readyToDraw = needToCopyVBOData = false;
        
        // for each LOD level, create 16*16 patches
        // vertices per patch is variant
        int numPatches = 16*16;
        
        for (int j=0; j<numPatches; j++) {
            TerrainPatch *newPatch = new TerrainPatch();
            newPatch->setIndex(j);
            m_Patches.push_back(newPatch);
        }
        
        m_baseCoords[0] = baseLatitude;
        m_baseCoords[1] = baseLongitude;
        
        // init VBOs
        glGenBuffers(1, &VBO_vertexs);
        glGenBuffers(1, &VBO_normals);
    }
    
    void TerrainTile::setPosition(int position)
    {
        m_position = position;
    }
    
    void TerrainTile::loadFromFile()
    {
        // read from 0 to index 1201
        // if index % lod (for each lod) = 0, add to lod terrainpatch array
        std::cout << "TWS::TerrainTile loading from SRTM file on thread: " << pthread_self() << std::endl;
        
        unsigned long size = sizeof(Color4) * 1025 * 1025;
        std::cout << "TWS::TerrainTile will alloc " << size*2 << " bytes for normals and vertices" << std::endl;
        m_points2 = (Color4 *)malloc(sizeof(Color4) * 1025 * 1025);
        m_normals = (Color4 *)malloc(sizeof(Color4) * 1025 * 1025);
        
        // build filename
        char fileName[128];
        char latPrefix = m_baseCoords[0] >= 0 ? 'N' : 'S';
        char lonPrefix = m_baseCoords[1] >= 0 ? 'E' : 'W';
        sprintf(fileName, "/usr/bin/unzip -p Data/SRTM/%c%02d%c%03d.hgt.zip",
            latPrefix, abs(m_baseCoords[0]), lonPrefix, abs(m_baseCoords[1]));
        std::cout << "TWS::TerrainTile will load file " << fileName << std::endl;
        
        // load the zipped file and unzip in memory
        // keep the topography repository's footprint to a minimum
        // ie: 12GB instead of 60GB
        short *m_points = pointBufferFromFile(fileName);
        
        // bidimensional nearest neighbour downsampling (not quite bilinear interpolation)
        for (int i=0; i<1025; i++) {
            for (int j=0; j<1025; j++) {
                double latitude = m_baseCoords[0] + 1 - (i/1024.0f);
                double longitude= m_baseCoords[1] + (j/1024.0f);
                
                // extract altitude from nearest point of original vector
                int scalei = (int)(i*1200.0/1024.0);
                int scalej = (int)(j*1200.0/1024.0);

                short altitude = ntohs(m_points[scalei*1201+scalej]);
                if (altitude == -32768) {
                    unsigned int heightSum = 0;
                    unsigned int pointCount = 0;
                    int minI, maxI, minJ, maxJ;
                    // smooth this point
                    minI = (i > 0) ? i-1 : i;
                    maxI = (i < 1023) ? i+1 : i;
                    minJ = (j > 0) ? j-1 : j;
                    maxJ = (j < 1023) ? j+1 : j;
                    for (int i2 = minI; i2 <= maxI; i2++) {
                        for (int j2 = minJ; j2 <= maxJ; j2++) {
                            short localHeight = ntohs(m_points[(int)(i2*1200.0/1024.0*1201.0) + (int)(j2*1200/1024.0)]);
                            if (localHeight != -32768) {
                                heightSum += localHeight;
                                pointCount++;
                            }
                        }
                    }
                    altitude = (float)heightSum / (float)pointCount;
                }
                geocentricToCarthesian(&m_points2[i*1025+j], latitude, longitude, altitude);
            }
        }
        free(m_points);
        
        // prepare the normal vector
        this->calcNormals();
        
        for (int i=0; i<m_Patches.size(); i++) {
            m_Patches.at(i)->setVertices(m_points2);
            m_Patches.at(i)->calcBoundaries();
        }
        
        needToCopyVBOData = true;
    }
        
    void TerrainTile::setNormalAtPos(int pos, double *color)
    {
        double len = sqrt(color[0]*color[0] + color[1]*color[1] + color[2]*color[2]);
            
        color[0] /= len;
        color[1] /= len;
        color[2] /= len;
        
        m_normals[pos].r = color[0];
        m_normals[pos].g = color[1];
        m_normals[pos].b = color[2];
    }
    
    void TerrainTile::calcNormals() {
        int self, left, topleft, top, topright, right, botright, bot, botleft;
        for (int line=0; line<1025; line++) {
            for (int row=0; row<1025; row++) {
                self = line*1025 + row;
                left = self - 1; // ok 
                topleft = self - 1 - 1025; // ok
                top = self - 1025; // ok
                topright = self - 1025 + 1; // ok
                right = self + 1; // ok
                botright = self + 1025 + 1; // ok
                bot = self + 1025; // ok
                botleft = self + 1025 - 1; // ok
                
                // determine position of vertex on tile because not all
                // of them use all corners to alias. if they're on corner
                // or bounding row/column
                if (line==0 && row==0) {
                    // topleft corner
                    // we'll use the bottom and right vertexes
                    double v[3][3],n[2][3];
                    this->vectorFromTo(v[0], m_points2[self], m_points2[bot]);
                    this->vectorFromTo(v[1], m_points2[self], m_points2[botright]);
                    this->vectorFromTo(v[2], m_points2[self], m_points2[right]);
                    
                    this->crossProduct(n[0], v[0], v[1]);
                    this->crossProduct(n[1], v[1], v[2]);
                    
                    double nf[3] = {
                        n[0][0] + n[1][0],
                        n[0][1] + n[1][1],
                        n[0][2] + n[1][2]};
                        
                    this->setNormalAtPos(self, nf);
                }
                else if (line==0 && row>0 && row<1024) {
                    // first row
                    // we'll use the left, botleft; botleft, bot; bot, right
                    double v1[3],v2[3],v3[3],v4[3],n1[3],n2[3],n3[3];
                    this->vectorFromTo(v1, m_points2[self], m_points2[left]);
                    this->vectorFromTo(v2, m_points2[self], m_points2[botleft]);
                    this->vectorFromTo(v3, m_points2[self], m_points2[bot]);
                    this->vectorFromTo(v4, m_points2[self], m_points2[right]);
                    
                    this->crossProduct(n1, v1, v2);
                    this->crossProduct(n2, v2, v3);
                    this->crossProduct(n3, v3, v4);
                    
                    double n[3] = {
                        n1[0] + n2[0] + n3[0],
                        n1[1] + n2[1] + n3[1],
                        n1[2] + n2[2] + n3[2]};
                    
                    this->setNormalAtPos(self, n);
                }
                else if (line==0 && row==1024) {
                    // topright corner
                    // we'll use the left, botleft; botleft, bot
                    double v[3][3],n[2][3];
                    
                    this->vectorFromTo(v[0], m_points2[self], m_points2[left]);
                    this->vectorFromTo(v[1], m_points2[self], m_points2[botleft]);
                    this->crossProduct(n[0], v[0], v[1]);
                    
                    this->vectorFromTo(v[2], m_points2[self], m_points2[bot]);
                    this->crossProduct(n[1], v[1], v[2]);
                    
                    double nf[3] = {
                        n[0][0] + n[1][0],
                        n[0][1] + n[1][1],
                        n[0][2] + n[1][2]};
                        
                    this->setNormalAtPos(self, nf);
                }
                else if (line > 0 && line < 1024 && row == 0){
                    // left column
                    double v[4][3], n[3][3];
                    
                    // bottom to right
                    this->vectorFromTo(v[0], m_points2[self], m_points2[bot]);
                    this->vectorFromTo(v[1], m_points2[self], m_points2[right]);
                    this->vectorFromTo(v[2], m_points2[self], m_points2[topright]);
                    this->vectorFromTo(v[3], m_points2[self], m_points2[top]);
                    
                    this->crossProduct(n[0], v[0], v[1]);
                    this->crossProduct(n[1], v[1], v[2]);
                    this->crossProduct(n[2], v[2], v[3]);
                    
                    double nf[3] = {
                        n[0][0] + n[1][0] + n[2][0],
                        n[0][1] + n[1][1] + n[2][1],
                        n[0][2] + n[1][2] + n[2][2]};

                    this->setNormalAtPos(self, nf);
                }
                else if (line > 0 && line < 1024 && row == 1024) {
                    // right column
                    double v[4][3],n[3][3];
                    
                    this->vectorFromTo(v[0], m_points2[self], m_points2[top]);
                    this->vectorFromTo(v[1], m_points2[self], m_points2[left]);
                    this->vectorFromTo(v[2], m_points2[self], m_points2[botleft]);
                    this->vectorFromTo(v[3], m_points2[self], m_points2[bot]);
                    
                    this->crossProduct(n[0], v[0], v[1]);
                    this->crossProduct(n[1], v[1], v[2]);
                    this->crossProduct(n[2], v[2], v[3]);
                    
                    double nf[3] = {
                        n[0][0] + n[1][0] + n[2][0],
                        n[0][1] + n[1][1] + n[2][1],
                        n[0][2] + n[1][2] + n[2][2]};
                        
                    this->setNormalAtPos(self, nf);
                }
                else if (line==1024 && row==0) {
                    // bottomleft corner
                    double v[3][3],n[2][3];
                    
                    this->vectorFromTo(v[0], m_points2[self], m_points2[right]);
                    this->vectorFromTo(v[1], m_points2[self], m_points2[topright]);
                    this->vectorFromTo(v[2], m_points2[self], m_points2[top]);
                    
                    this->crossProduct(n[0], v[0], v[1]);
                    this->crossProduct(n[1], v[1], v[2]);
                    
                    double nf[3] = {
                        n[0][0] + n[1][0],
                        n[0][1] + n[1][1],
                        n[0][2] + n[1][2]};
                    
                    this->setNormalAtPos(self, nf);
                }
                else if (line==1024 && row > 1 && row < 1024) {
                    // bottom row
                    double v[4][3], n[3][3];
                    
                    this->vectorFromTo(v[0], m_points2[self], m_points2[right]);
                    this->vectorFromTo(v[1], m_points2[self], m_points2[topright]);
                    this->vectorFromTo(v[2], m_points2[self], m_points2[top]);
                    this->vectorFromTo(v[3], m_points2[self], m_points2[left]);
                    
                    this->crossProduct(n[0], v[0], v[1]);
                    this->crossProduct(n[1], v[1], v[2]);
                    this->crossProduct(n[2], v[2], v[3]);
                    
                    double nf[3] = {
                        n[0][0] + n[1][0] + n[2][0],
                        n[0][1] + n[1][1] + n[2][1],
                        n[0][2] + n[1][2] + n[2][2]};
                    
                    this->setNormalAtPos(self, nf);
                }
                else if (line==1024 && row==1024) {
                    // bottomright corner
                    double v[2][3], n[2][3];
                    
                    this->vectorFromTo(v[0], m_points2[self], m_points2[top]);
                    this->vectorFromTo(v[1], m_points2[self], m_points2[topleft]);
                    this->vectorFromTo(v[2], m_points2[self], m_points2[left]);
                    
                    this->crossProduct(n[0], v[0], v[1]);
                    this->crossProduct(n[1], v[1], v[2]);
                    
                    double nf[3] = {
                        n[0][0] + n[1][0],
                        n[0][1] + n[1][1],
                        n[0][2] + n[1][2]};
                    
                    this->setNormalAtPos(self, nf);
                }
                else if (line>0 && line <1024 && row>0 && row < 1024) {
                    // all others
                    double v[8][3],n[6][3];
                    
                    // topright to top
                    this->vectorFromTo(v[0], m_points2[self], m_points2[topright]);
                    this->vectorFromTo(v[1], m_points2[self], m_points2[top]);
                    this->crossProduct(n[0], v[0], v[1]);
                    
                    // top to left
                    this->vectorFromTo(v[2], m_points2[self], m_points2[left]);
                    this->crossProduct(n[1], v[1], v[2]);
                    
                    // left to botleft
                    this->vectorFromTo(v[3], m_points2[self], m_points2[botleft]);
                    this->crossProduct(n[2], v[2], v[3]);
                    
                    // botleft to bot
                    this->vectorFromTo(v[4], m_points2[self], m_points2[bot]);
                    this->crossProduct(n[3], v[3], v[4]);
                    
                    // bot to right
                    this->vectorFromTo(v[5], m_points2[self], m_points2[right]);
                    this->crossProduct(n[4], v[4], v[5]);
                    
                    // right to topright
                    this->crossProduct(n[5], v[5], v[0]);
                    
                    double nf[3] = {
                        (n[0][0]+n[1][0]+n[2][0]+n[3][0]+n[4][0]+n[5][0])/6.0,
                        (n[0][1]+n[1][1]+n[2][1]+n[3][1]+n[4][1]+n[5][1])/6.0,
                        (n[0][2]+n[1][2]+n[2][2]+n[3][2]+n[4][2]+n[5][2])/6.0 };
                    
                    this->setNormalAtPos(self, nf);
                }
            }
        }
    }
    
    void TerrainTile::crossProduct(double *normalvec, double *v1, double *v2) {
        normalvec[0] = (v1[1]*v2[2]) - (v1[2]*v2[1]);
        normalvec[1] = (v1[2]*v2[0]) - (v1[0]*v2[2]);
        normalvec[2] = (v1[0]*v2[1]) - (v1[1]*v2[0]);
        
        // normalize it
        double len = sqrt(normalvec[0]*normalvec[0] + normalvec[1]*normalvec[1] + normalvec[2]*normalvec[2]);
        normalvec[0] /= len;
        normalvec[1] /= len;
        normalvec[2] /= len;
    }
    
    void TerrainTile::vectorFromTo(double *vec, Color4 &pt1, Color4 &pt2) {
        vec[0] = pt2.r - pt1.r;
        vec[1] = pt2.g - pt1.g;
        vec[2] = pt2.b - pt1.b;
    }
    
    // cleanup and dealloc patches
    TerrainTile::~TerrainTile(){
        // free patches
        TerrainPatch *thePatch;
        int localSize = m_Patches.size();
        std::cout << "TWS::TerrainTile will dealoc " << localSize << " patches" << std::endl;
        for (int j=0; j < localSize; j++) {
            thePatch = (TerrainPatch *)m_Patches.back();
            delete thePatch;
            m_Patches.pop_back();
        }
        
        glDeleteBuffers(1, &VBO_vertexs);
        glDeleteBuffers(1, &VBO_normals);
        
        std::cout << "TWS::TerrainTile dying" << std::endl;
    }
};
