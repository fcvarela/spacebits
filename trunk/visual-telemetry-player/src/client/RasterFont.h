/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  raster_font.h
 *  proj_cg
 *
 *  Created by Filipe Varela on 07/05/16.
 */

#ifndef _RASTERFONT
#define _RASTERFONT

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#endif

#ifdef linux
#include <GL/gl.h>
#endif

namespace TWS
{
    class RasterFont
    {
        private:
            GLuint fontOffset;
            GLubyte rasters[127][13];
        
        public:
            RasterFont();
            ~RasterFont();
            void printString(char *s);
    };
}

#endif
