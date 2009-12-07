/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  SkyDome.h
 *  TWS
 *
 *  Created by Filipe Varela on 09/02/23.
 *  Copyright 2009 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_SKYDOME
#define _TWS_SKYDOME

#define GEO_EARTH_RADIUS 6371008.7714f
#define PI 3.14159265f
#define DEG2RAD 0.0174532925

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef linux
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#endif

#include "Vertex.h"

namespace TWS
{
    class SkyDome
    {
        private:
            Color4 *m_points,*m_normals;
            int numVerts;
            
            // shader
            GLuint m_vertexShader;
            GLuint m_fragShader;
            GLuint m_shaderProgram;
            GLuint m_skydomeTexture;
            GLfloat m_sunLLAOffset[2];
            
            void printShaderLog(GLuint obj);
            char *getShaderSource(const char *path);
            
        public:
            void draw(float sunLatOffset, float sunLonOffset);
            SkyDome();
            ~SkyDome();
    };
}

#endif
