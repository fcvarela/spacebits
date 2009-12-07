/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  TerrainManager.h
 *  TWS
 *
 *  Created by Filipe Varela on 08/10/31.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_TERRAINMANAGER
#define _TWS_TERRAINMANAGER

#include "TerrainTile.h"

namespace TWS {
    class TerrainManager
    {
        private:
            TerrainTile *mTerrainTiles[4];
            // texture (grass, etc)
            GLuint m_terrainTextures[2];
            GLuint m_waterNormalMap;
            GLuint m_waterDUDVMap;
            GLuint m_skyColorMap;
            GLfloat m_waterOffset[2];
            GLfloat m_sunLLAOffset[2];
            
            Color4 *m_detailTextureCoords;
            
            // shader
            GLuint m_vertexShader;
            GLuint m_fragShader;
            GLuint m_terrainShader;
            
            char *getShaderSource(const char *path);
            void printShaderLog(GLuint obj);
            void detachThreadForNewTile(int tileIndex, int position, int baseLatitude, int baseLongitude);
            
        public:
            TerrainManager();
            ~TerrainManager();
            GLuint m_reflectionTexture;
            
            // draw tiles
            void draw(float sunLatOffset, float sunLonOffset);
            
            // recalculate 4 loaded tiles
            void reload(void);
    };
}

#endif
