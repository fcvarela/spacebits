/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  TerrainManager.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 08/10/31.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include "TerrainManager.h"
#include "Factory.h"
#include <iostream>
#include <pthread.h>
#include "CTargaImage.h"
#include "BMPImage.h"

void *asyncLoadTile(void *tileData);

namespace TWS {
    TerrainManager::TerrainManager() {
        std::cout << "TWS::TerrainManager allocing" << std::endl;
        mTerrainTiles[0] = mTerrainTiles[1] = mTerrainTiles[2] = mTerrainTiles[3] = NULL;
        
        // water offset
        m_waterOffset[0] = m_waterOffset[1] = 0.0;
        
        // init textures
        BMPImage *detailTexture;
        detailTexture = new BMPImage();
        detailTexture->loadFile("Data/Textures/t0.bmp");
        glGenTextures(1, &m_terrainTextures[0]);
        glBindTexture(GL_TEXTURE_2D, m_terrainTextures[0]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, detailTexture->m_image.sizeX, detailTexture->m_image.sizeY, GL_RGB, GL_UNSIGNED_BYTE, detailTexture->m_image.data);
        delete detailTexture;
        
        detailTexture = new BMPImage();
        detailTexture->loadFile("Data/Textures/t1.bmp");
        glGenTextures(1, &m_terrainTextures[1]);
        glBindTexture(GL_TEXTURE_2D, m_terrainTextures[1]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,detailTexture->m_image.sizeX, detailTexture->m_image.sizeY,GL_RGB, GL_UNSIGNED_BYTE, detailTexture->m_image.data);
        delete detailTexture;
        
        detailTexture = new BMPImage();
        detailTexture->loadFile("Data/Textures/w-normalmap.bmp");
        glGenTextures(1, &m_waterNormalMap);
        glBindTexture(GL_TEXTURE_2D, m_waterNormalMap);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,detailTexture->m_image.sizeX, detailTexture->m_image.sizeY,GL_RGB, GL_UNSIGNED_BYTE, detailTexture->m_image.data);
        delete detailTexture;
        
        detailTexture = new BMPImage();
        detailTexture->loadFile("Data/Textures/w-dudv.bmp");
        glGenTextures(1, &m_waterDUDVMap);
        glBindTexture(GL_TEXTURE_2D, m_waterDUDVMap);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,detailTexture->m_image.sizeX, detailTexture->m_image.sizeY,GL_RGB, GL_UNSIGNED_BYTE, detailTexture->m_image.data);
        delete detailTexture;
        
        glGenTextures(1, &m_reflectionTexture);
        glBindTexture(GL_TEXTURE_2D, m_reflectionTexture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        unsigned char *data = (unsigned char *)malloc(1280*800*3);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        free(data);
        
        std::cout << "Loading tex" << std::endl;
        CTargaImage *detailTexture2 = new CTargaImage();
        detailTexture2->Load("Data/Textures/skydome-colormap.tga");
        glGenTextures(1, &m_skyColorMap);
        glBindTexture(GL_TEXTURE_2D, m_skyColorMap);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, detailTexture2->GetWidth(), detailTexture2->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, detailTexture2->GetImage());
        delete detailTexture2;
        std::cout << "Loading tex" << std::endl;
        // build texture coords
        m_detailTextureCoords = (Color4 *)malloc(sizeof(Color4) * 1025 * 1025);
        for (int i=0; i<1025; i++) {
            for (int j=0; j<1025; j++) {
                // set detail texture coordinates
                m_detailTextureCoords[i*1025+j].r = (i/1024.0f);
                m_detailTextureCoords[i*1025+j].g = (j/1024.0f);
            }
        }
        
        // load, compile, link shaders
        char *vertSource = getShaderSource("Data/Shaders/terrain-vs.txt");
        char *fragSource = getShaderSource("Data/Shaders/terrain-fs.txt");
        
        // create shader ids
        m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        m_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        // pass shader source to gl
        glShaderSource(m_vertexShader, 1, (const GLchar**)&vertSource, NULL);
        glShaderSource(m_fragShader, 1, (const GLchar**)&fragSource, NULL);
        
        // compile source
        glCompileShader(m_vertexShader); printShaderLog(m_vertexShader);
        glCompileShader(m_fragShader); printShaderLog(m_vertexShader);
        
        // free source
        free(vertSource);
        free(fragSource);
        
        // create high level glsl program (vertex+frag shader)
        m_terrainShader = glCreateProgram();
        
        // attach shader ids to program
        glAttachShader(m_terrainShader, m_vertexShader);
        glAttachShader(m_terrainShader, m_fragShader);
        
        // link
        glLinkProgram(m_terrainShader);
        
        // dump program log
        std::cout << "TWS::TerrainManager shader log:" << std::endl;
        printShaderLog(m_terrainShader);
    }
    
    void TerrainManager::printShaderLog(GLuint obj) {
        GLsizei infologLength = 0;
        char infoLog[1024];
        
        if (glIsShader(obj))
            glGetShaderInfoLog(obj, 1024, &infologLength, infoLog);
        else
            glGetProgramInfoLog(obj, 1024, &infologLength, infoLog);
        
        if (infologLength > 0)
            printf("%s\n", infoLog);
    }
    
    char *TerrainManager::getShaderSource(const char *path) {
        FILE *fd;
        long len,
        r;
        char *str;
        
        if (!(fd = fopen(path, "r"))) {
            std::cerr << "TWS::TerrainManager can't open file " << path << " for reading" << std::endl;
            return NULL;
        }
        
        fseek(fd, 0, SEEK_END);
        len = ftell(fd);
        
        printf("File '%s' is %ld long\n", path, len);
        
        fseek(fd, 0, SEEK_SET);
        
        if (!(str = (char *)malloc(len * sizeof(char))))
        {
            fprintf(stderr, "Can't malloc space for '%s'\n", path);
            return NULL;
        }
        
        r = fread(str, sizeof(char), len, fd);
        
        str[r - 1] = '\0'; /* Shader sources have to term with null */
        
        fclose(fd);
        
        return str;
    }
    
    TerrainManager::~TerrainManager() {
        std::cout << "TWS::TerrainManager cleaning up tiles" << std::endl;
        
        for (int i=0; i<4; i++)
            if (mTerrainTiles[i] != NULL) delete mTerrainTiles[i];
        
        free(m_detailTextureCoords);
        
        glDeleteTextures(1, &m_terrainTextures[0]);
        glDeleteTextures(1, &m_terrainTextures[1]);
        
        std::cout << "TWS::TerrainManager dying" << std::endl;
    }
    
    void TerrainManager::reload() {
        for (int i=0; i<4; i++) {
            if (mTerrainTiles[i] != NULL) {
                if (!mTerrainTiles[i]->readyToDraw)
                    return;
            }
        }
        // base latitudes and longitudes of the 4 needed tiles
        int neededTiles[4][2];
        
        // get a pointer to the camera
        Camera *cam = TWS::getCamera();
        
        // save a copy of the absolute decimal value of the
        float base_coordsf[2];
        int base_coords[2];
        float baselat_frac;
        float baselon_frac;
        
        baselat_frac = modff(cam->getPosLLA(0), &base_coordsf[0]);
        baselon_frac = modff(cam->getPosLLA(1), &base_coordsf[1]);
        base_coords[0] = base_coordsf[0];
        base_coords[1] = base_coordsf[1];
        
        // check sign of frac part. if below zero, invert sign and complement to 1
        // the use the same calculation method for both positive and negative
        // base coords
        if (baselat_frac < 0.0) {
            baselat_frac = 1 - fabsf(baselat_frac);
            base_coords[0] -= 1;
        }
        
        if (baselon_frac < 0.0) {
            baselon_frac = 1 - fabsf(baselon_frac);
            base_coords[1] -= 1;
        }
        
        // determine which other tiles we need based on these coords
        // possibilities
        /*
         +---+---+
         | C | D |
         +---+---+
         | A | B |
         +---+---+
         */
        
        // create list of 4 needed tiles
        // item at position 0 is always the tile where the cam is over
        // once this is loaded, set 0,0 to the origin of the camera tile
        // update camera_coords if needed to offset that origin
        // create offsets for the other tiles so we can translatef before drawing them
        if (baselat_frac > 0.5 && baselon_frac > 0.5) {
            // A: we want to load tiles B, C, D and keep A
            neededTiles[0][0] = base_coords[0]; neededTiles[0][1] = base_coords[1]; // Keep A
            neededTiles[1][0] = base_coords[0]; neededTiles[1][1] = base_coords[1]+1; // Load B
            neededTiles[2][0] = base_coords[0]+1; neededTiles[2][1] = base_coords[1]; // Load C
            neededTiles[3][0] = base_coords[0]+1; neededTiles[3][1] = base_coords[1]+1; // Load D
        } else if (baselat_frac > 0.5 && baselon_frac < 0.5) {
            // B: we want to load tiles A, C, D and keep B
            neededTiles[0][0] = base_coords[0]; neededTiles[0][1] = base_coords[1]; // Keep B
            neededTiles[1][0] = base_coords[0]; neededTiles[1][1] = base_coords[1]-1; // Load A
            neededTiles[2][0] = base_coords[0]+1; neededTiles[2][1] = base_coords[1]-1; // Load C
            neededTiles[3][0] = base_coords[0]+1; neededTiles[3][1] = base_coords[1]; // Load D
        } else if (baselat_frac < 0.5 && baselon_frac > 0.5) {
            // C: we want to load tiles A, B, D
            neededTiles[0][0] = base_coords[0]; neededTiles[0][1] = base_coords[1]; // Keep C
            neededTiles[1][0] = base_coords[0]-1; neededTiles[1][1] = base_coords[1]; // Load A
            neededTiles[2][0] = base_coords[0]-1; neededTiles[2][1] = base_coords[1]+1; // Load B
            neededTiles[3][0] = base_coords[0]; neededTiles[3][1] = base_coords[1]+1; // Load D
        } else if (baselat_frac < 0.5 && baselon_frac < 0.5) {
            // D: we want to load tiles A, B, C
            neededTiles[0][0] = base_coords[0]; neededTiles[0][1] = base_coords[1]; // Keep D
            neededTiles[1][0] = base_coords[0]-1; neededTiles[1][1] = base_coords[1]-1; // Load A
            neededTiles[2][0] = base_coords[0]-1; neededTiles[2][1] = base_coords[1]; // Load B
            neededTiles[3][0] = base_coords[0]; neededTiles[3][1] = base_coords[1]-1; // Load C
        }
        
        // debug mode: print needed tiles to console and return
        /*printf("TWS::Terrainmanager loading tiles: %+02d%+03d.hgt %+02d%+03d.hgt %+02d%+03d.hgt %+02d%+03d.hgt\n",
         neededTiles[0][0], neededTiles[0][1],
         neededTiles[1][0], neededTiles[1][1],
         neededTiles[2][0], neededTiles[2][1],
         neededTiles[3][0], neededTiles[3][1]);
         return;
         */
        // unload all loaded tiles NOT in the list of 4 items
        for (int i=0; i<4; i++) {
            // walk the needed tile list
            bool deleteThisTile = true;
            for (int j=0; j<4; j++) {
                if (mTerrainTiles[i]!=NULL) {
                    if (mTerrainTiles[i]->baseLat() == neededTiles[j][0] && mTerrainTiles[i]->baseLon() == neededTiles[j][1]) {
                        // this tile is needed. do not delete
                        deleteThisTile = false;
                    }
                }
            }
            if (deleteThisTile) {
                // remove the tile from the tile vector
                delete mTerrainTiles[i];
                mTerrainTiles[i] = NULL;
            }
        }
        
        // load needed tiles NOT in the list of 4 items. put them in a free NULL entry
        for (int i=0; i<4; i++) {
            bool needToLoad = true;
            for (int j=0; j<4; j++) {
                if (mTerrainTiles[j] != NULL) {
                    if (neededTiles[i][0] == mTerrainTiles[j]->baseLat() && neededTiles[i][1] == mTerrainTiles[j]->baseLon())
                        needToLoad = false;
                }
            }
            // checked all loaded tiles. if needToLoad is still true, find a NULL slot and load the tile in
            if (needToLoad) {
                for (int j=0; j<4; j++) {
                    if (mTerrainTiles[j] == NULL) {
                        this->detachThreadForNewTile(j, i, neededTiles[i][0], neededTiles[i][1]);
                        break;
                    }
                }
            }
        }
    }
    
    void TerrainManager::detachThreadForNewTile(int tileIndex, int tilePosition, int baseLatitude, int baseLongitude) {
        mTerrainTiles[tileIndex] = new TerrainTile(baseLatitude, baseLongitude);
        mTerrainTiles[tileIndex]->setPosition(tilePosition); // sets A,B,C or D so we know offset to origin point
        pthread_t terrainLoaderThread;
        std::cout << "TWS::TerrainManager spawning thread to load tile from thread: " << pthread_self() << std::endl;
        int tid = pthread_create(&terrainLoaderThread, NULL, asyncLoadTile, (void*)mTerrainTiles[tileIndex]);
        std::cout << "TWS::TerrainManager finished spawning child thread: " << tid << std::endl;
    }
    
    void TerrainManager::draw(float sunLatOffset, float sunLonOffset) {
        // set array setup for dirt,grass,rock,snow
        glClientActiveTexture(GL_TEXTURE0);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 16, m_detailTextureCoords);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_terrainTextures[0]);
        
        glClientActiveTexture(GL_TEXTURE1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_terrainTextures[1]);
        
        glClientActiveTexture(GL_TEXTURE2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_waterNormalMap);
        
        glClientActiveTexture(GL_TEXTURE3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_waterDUDVMap);
        
        glClientActiveTexture(GL_TEXTURE4);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, m_reflectionTexture);
        
        glClientActiveTexture(GL_TEXTURE5);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, m_skyColorMap);
        
        // turn on shaders
        glUseProgram(m_terrainShader);
        
        glUniform1i(glGetUniformLocation(m_terrainShader, "terrainDetailSoft"), 0);
        glUniform1i(glGetUniformLocation(m_terrainShader, "terrainDetailHard"), 1);
        glUniform1i(glGetUniformLocation(m_terrainShader, "waterNormalMap"), 2);
        glUniform1i(glGetUniformLocation(m_terrainShader, "waterDUDVMap"), 3);
        glUniform1i(glGetUniformLocation(m_terrainShader, "reflectionTexture"), 4);
        glUniform1i(glGetUniformLocation(m_terrainShader, "colormap"), 5);
        m_sunLLAOffset[0] = sunLatOffset;
        m_sunLLAOffset[1] = sunLonOffset;
        m_waterOffset[0] += 0.001 / TWS::fps;
        m_waterOffset[1] += 0.001 / TWS::fps;
        glUniform2fv(glGetUniformLocation(m_terrainShader, "sunLLAOffset"), 1, m_sunLLAOffset);
        glUniform2fv(glGetUniformLocation(m_terrainShader, "waterCoordOffset"), 1, m_waterOffset);
        
        for (int i=0; i<4; i++)
            if (mTerrainTiles[i] != NULL) mTerrainTiles[i]->draw();
        
        // disable shader
        glUseProgram(0);
        
    }
}

void *asyncLoadTile(void *tileData) {
    TWS::TerrainTile *theTile = (TWS::TerrainTile *)tileData;
    theTile->loadFromFile();
    
    return NULL;
}
