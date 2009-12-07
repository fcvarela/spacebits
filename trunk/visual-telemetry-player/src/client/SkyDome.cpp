/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  SkyDome.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 09/02/23.
 *  Copyright 2009 Filipe Varela. All rights reserved.
 *
 */

#include <iostream>
#include <cstdlib>

#include "CTargaImage.h"
#include "SkyDome.h"
#include "Factory.h"

namespace TWS
{
    void SkyDome::draw(float sunLatOffset, float sunLonOffset)
    {
        glUseProgram(m_shaderProgram);
        
        m_sunLLAOffset[0] = sunLatOffset;
        m_sunLLAOffset[1] = sunLonOffset;
        
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_skydomeTexture);
        glUniform1i(glGetUniformLocation(m_shaderProgram, "colormap"), 0);
        glUniform2fv(glGetUniformLocation(m_shaderProgram, "sunLLAOffset"), 1, m_sunLLAOffset);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 16, m_points);
        glNormalPointer(GL_FLOAT, 16, m_normals);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, numVerts);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        
        glDisable(GL_TEXTURE_2D);
        glUseProgram(0);
    }
    
    SkyDome::SkyDome()
    {
        // init textures
        CTargaImage *detailTexture = new CTargaImage();
        detailTexture->Load("Data/Textures/skydome-colormap.tga");
        glGenTextures(1, &m_skydomeTexture);
        glBindTexture(GL_TEXTURE_2D, m_skydomeTexture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, detailTexture->GetWidth(), detailTexture->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, detailTexture->GetImage());
        delete detailTexture;
        
        // num vert call
        numVerts = (int)((360.0/2.0)*(90.0/2.0)*4);
        m_points = (Color4*)malloc(sizeof(Color4)*numVerts);
        m_normals = (Color4*)malloc(sizeof(Color4)*numVerts);
        
        // generate points
        int n=0;
        float domeRadius = 30000;
        for (int phi=0; phi<=90-2; phi+=2) {
            for (int theta=0; theta<=360-2; theta+=2) {
                m_points[n].r = domeRadius * sinf(phi*DEG2RAD) * cosf(theta*DEG2RAD);
                m_points[n].b = domeRadius * sinf(phi*DEG2RAD) * sinf(theta*DEG2RAD);
                m_points[n].g = domeRadius * cosf(phi*DEG2RAD);
                n++;
                
                m_points[n].r = domeRadius * sinf((phi+2)*DEG2RAD) * cosf(theta*DEG2RAD);
                m_points[n].b = domeRadius * sinf((phi+2)*DEG2RAD) * sinf(theta*DEG2RAD);
                m_points[n].g = domeRadius * cosf((phi+2)*DEG2RAD);
                n++;
                
                m_points[n].r = domeRadius * sinf(phi*DEG2RAD) * cosf((theta+2)*DEG2RAD);
                m_points[n].b = domeRadius * sinf(phi*DEG2RAD) * sinf((theta+2)*DEG2RAD);
                m_points[n].g = domeRadius * cosf(phi*DEG2RAD);
                n++;
                
                if (phi > -90 && phi < 90) {
                    m_points[n].r = domeRadius * sinf((phi+2)*DEG2RAD) * cosf((theta+2)*DEG2RAD);
                    m_points[n].b = domeRadius * sinf((phi+2)*DEG2RAD) * sinf((theta+2)*DEG2RAD);
                    m_points[n].g = domeRadius * cosf((phi+2)*DEG2RAD);
                    n++;
                }
            }
        }
        
        // generate normals
        for (int i=0; i<numVerts; i++) {
            m_normals[i].r = m_points[i].r;
            m_normals[i].g = m_points[i].g;
            m_normals[i].b = m_points[i].b;
            
            double len = sqrt(m_normals[i].r*m_normals[i].r + m_normals[i].g*m_normals[i].g + m_normals[i].b*m_normals[i].b);
            
            m_normals[0].r /= len;
            m_normals[0].g /= len;
            m_normals[0].b /= len;
        }
        
        // load, compile, link shaders
        char *vertSource = getShaderSource("Data/Shaders/skydome-vs.txt");
        char *fragSource = getShaderSource("Data/Shaders/skydome-fs.txt");
        
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
    }
    
    void SkyDome::printShaderLog(GLuint obj)
    {
        GLsizei infologLength = 0;
        char infoLog[1024];
 
        if (glIsShader(obj))
            glGetShaderInfoLog(obj, 1024, &infologLength, infoLog);
        else
            glGetProgramInfoLog(obj, 1024, &infologLength, infoLog);
 
        if (infologLength > 0)
            std::cout << "TWS::SkyDome " << infoLog << std::cout;
    }
    
    char *SkyDome::getShaderSource(const char *path)
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
    
    SkyDome::~SkyDome() {
        free(m_points);
    }
}
