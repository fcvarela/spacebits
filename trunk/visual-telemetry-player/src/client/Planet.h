/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Planet.h
 *  TWS
 *
 *  Created by Filipe Varela on 08/10/17.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_PLANET
#define _TWS_PLANET

#include <string>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#ifdef linux
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#define GEO_EARTH_RADIUS 6371008.7714f

namespace TWS {
    class Planet
    {
        public:
            Planet();
            ~Planet();
            void draw(int altitude);
        private:
            GLuint m_shaderProgram, m_vertexShader, m_fragShader;
            GLuint m_atmosphereShaderProgram, m_atmosphereVertexShader, m_atmosphereFragShader;
            GLuint planetListIndex, atmosphereListIndex;
            GLuint m_planetTexture;
            
            char *getShaderSource(const char *path);
            void printShaderLog(GLuint obj);
    };
}

#endif
