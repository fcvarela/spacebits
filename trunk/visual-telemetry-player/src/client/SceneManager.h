/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  SceneManager.h
 *  
 *
 *  Created by Filipe Varela on 08/10/13.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_SCENEMANAGER
#define _TWS_SCENEMANAGER

#include "TerrainManager.h"
#include "Planet.h"
#include "SkyDome.h"
#include "RasterFont.h"
#include <SDL/SDL.h>
#include "Window.h"

namespace TWS
{
	class SceneManager {
		private:
			TerrainManager *m_Terrain;
			Planet *m_Planet;
			SkyDome *m_SkyDome;
			Window m_Window;
			unsigned int m_Keys[SDLK_LAST];
			unsigned int m_mouseButtons[2];
			unsigned short m_mousePosition[2];
            
			void draw();
			void processKeys();
			void resize(int x, int y);
			bool processEvents();
			void processClicks();
			bool terrainScene;
			GLfloat sunLightPos[4];
			GLfloat sunLightDir[4];
			GLfloat sunLightAmbientColor[4], sunLightDiffuseColor[4], sunLightSpecularColor[4];
			double suncoords[2];
			GLfloat skyColor[4];
			struct timeval start,end;
			long framecount;
            
			RasterFont *debugFont;
			void renderDebugText();
			void positionSunLight();
		public:
			SceneManager();
			~SceneManager();
            
			bool init();
			void run();
	};
};

#endif
