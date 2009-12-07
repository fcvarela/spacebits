/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Planet.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 08/10/17.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#include <iostream>

#include "Factory.h"
#include "Astro.h"
#include "Vertex.h"
#include "Planet.h"
#include "BMPImage.h"

namespace TWS {
    Planet::Planet() {
        GLUquadric *m_planetSpheroid;
        GLUquadric *m_atmosphereSpheroid;
        
        m_planetSpheroid = gluNewQuadric();
        gluQuadricNormals(m_planetSpheroid, GL_SMOOTH);
        gluQuadricTexture(m_planetSpheroid, GL_TRUE);
        
        m_atmosphereSpheroid = gluNewQuadric();
        
        // create the display lists for planet and atmosphere (glusphere is SLOW)
        planetListIndex = glGenLists(1);
        glNewList(planetListIndex, GL_COMPILE);
            gluSphere(m_planetSpheroid, GEO_EARTH_RADIUS, 100, 100);
        glEndList();
        glNewList(atmosphereListIndex, GL_COMPILE);
            gluSphere(m_atmosphereSpheroid, GEO_EARTH_RADIUS*1.025, 100, 100);
        glEndList();
        gluDeleteQuadric(m_planetSpheroid);
        gluDeleteQuadric(m_atmosphereSpheroid);
        
        BMPImage *planetTexture = new BMPImage();
        planetTexture->loadFile("Data/Textures/planet-earth.bmp");
        
        glGenTextures(1, &m_planetTexture);
        glBindTexture(GL_TEXTURE_2D, m_planetTexture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,planetTexture->m_image.sizeX, planetTexture->m_image.sizeY,GL_RGB, GL_UNSIGNED_BYTE, planetTexture->m_image.data);
        
        delete(planetTexture);
        
        // load shaders
        // load, compile, link shaders
        char *vertSource = getShaderSource("Data/Shaders/planet-vs.txt");
        char *fragSource = getShaderSource("Data/Shaders/planet-fs.txt");
        
        m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        m_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_vertexShader, 1, (const GLchar**)&vertSource, NULL);
        glShaderSource(m_fragShader, 1, (const GLchar**)&fragSource, NULL);
        glCompileShader(m_vertexShader);
        printShaderLog(m_vertexShader);
        glCompileShader(m_fragShader);
        printShaderLog(m_vertexShader);
        free(vertSource);
        free(fragSource);
        m_shaderProgram = glCreateProgram();
        glAttachShader(m_shaderProgram, m_vertexShader);
        glAttachShader(m_shaderProgram, m_fragShader);
        glLinkProgram(m_shaderProgram);
        printShaderLog(m_shaderProgram);
        
        // same thing for atmosphere
        char *atmosphereVertSource = getShaderSource("Data/Shaders/planetatmosphere-vs.txt");
        char *atmosphereFragSource = getShaderSource("Data/Shaders/planetatmosphere-fs.txt");
        
        m_atmosphereVertexShader = glCreateShader(GL_VERTEX_SHADER);
        m_atmosphereFragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_atmosphereVertexShader, 1, (const GLchar**)&atmosphereVertSource, NULL);
        glShaderSource(m_atmosphereFragShader, 1, (const GLchar**)&atmosphereFragSource, NULL);
        glCompileShader(m_atmosphereVertexShader);
        printShaderLog(m_atmosphereVertexShader);
        glCompileShader(m_atmosphereFragShader);
        printShaderLog(m_atmosphereVertexShader);
        free(atmosphereVertSource);
        free(atmosphereFragSource);
        m_atmosphereShaderProgram = glCreateProgram();
        glAttachShader(m_atmosphereShaderProgram, m_atmosphereVertexShader);
        glAttachShader(m_atmosphereShaderProgram, m_atmosphereFragShader);
        glLinkProgram(m_atmosphereShaderProgram);
        printShaderLog(m_atmosphereShaderProgram);
        
        std::cout << "TWS::Planet inited" << std::endl;
    }
    
    Planet::~Planet() {
        std::cout << "TWS::Planet will dealoc" << std::endl;
        glDeleteLists(planetListIndex,1);
        glDeleteLists(atmosphereListIndex,1);
    }
    
    void Planet::draw(int altitude) {
        // optional
        if (altitude < 30000)
            return;
        
        glUseProgram(m_shaderProgram);
        glUniform1i(glGetUniformLocation(m_shaderProgram, "planetSurface"), 0);
        glRotatef(90.0,0.0,0.0,1.0);
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_planetTexture);
        glCallList(planetListIndex);
        glDisable(GL_TEXTURE_2D);
        
        glUseProgram(m_atmosphereShaderProgram);
        Camera *cam = TWS::getCamera();
        glUniform3f(glGetUniformLocation(m_atmosphereShaderProgram, "v3CameraPos"), cam->getPosXYZ(0), cam->getPosXYZ(1), cam->getPosXYZ(2));
        double suncoords[3];
        double sunLightPos[3];
        TWS_Astro::getsuncoords(&suncoords[0]);
        Color4 sun = Color4(0,0,0,0);
        geocentricToCarthesianReal(&sun, suncoords[0], -suncoords[1], pow(2, 8*sizeof(long))-1);
        // copy vertex to light position
        sunLightPos[0] = sun.r / (pow(2, 8*sizeof(long))-1);
        sunLightPos[1] = sun.g / (pow(2, 8*sizeof(long))-1);
        sunLightPos[2] = sun.b / (pow(2, 8*sizeof(long))-1);
        glUniform3f(glGetUniformLocation(m_atmosphereShaderProgram, "v3LightPos"), sunLightPos[0], sunLightPos[1], sunLightPos[2]);
        glColor4f(1.0f,1.0f,1.0f,1.0f);
        glFrontFace(GL_CW);
        glCallList(atmosphereListIndex);
        glFrontFace(GL_CCW);
        glUseProgram(0);
    }
    
    void Planet::printShaderLog(GLuint obj)
    {
        GLsizei infologLength = 0;
        char infoLog[1024];
 
        if (glIsShader(obj))
            glGetShaderInfoLog(obj, 1024, &infologLength, infoLog);
        else
            glGetProgramInfoLog(obj, 1024, &infologLength, infoLog);
 
        if (infologLength > 0)
            printf("%s\n", infoLog);
    }
    
    char *Planet::getShaderSource(const char *path)
    {
        FILE *fd;
        long len,
        r;
        char *str;
        
        if (!(fd = fopen(path, "r")))
        {
            fprintf(stderr, "Can't open file '%s' for reading\n", path);
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
}