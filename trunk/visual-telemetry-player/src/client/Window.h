/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TWS_WINDOW
#define _TWS_WINDOW

#include <string>
#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#ifdef macosx
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#ifdef linux
#include <GL/gl.h>
#include <GL/glu.h>
#endif

using std::string;

namespace TWS
{
	class Window
	{
		private:
			int m_Width;
			int m_Height;
			int m_Bpp;
			bool m_Fullscreen;
			string m_Title;
			
		public:
			Window();
			~Window();
			
			bool createWindow(int width, int height, int bpp, bool fullscreen, const string& title);
			float getHeight();
			float getWidth();
	};
};

#endif

