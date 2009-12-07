/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
*  SceneManager.cpp
*  
*
*  Created by Filipe Varela on 08/10/13.
*  Copyright 2008 Filipe Varela. All rights reserved.
*
*/

#include <iostream>
#include <sys/time.h>

#include "math.h"
#include "Astro.h"

#include "SceneManager.h"
#include "RasterFont.h"
#include "FrustumTools.h"
#include "Factory.h"
#include "PersistentActor.h"

namespace TWS {
	SceneManager::SceneManager() {
		// initial position
		TWS::getCamera()->setPosLLA(38.940555, 72.013055, 60000.0);
		TWS::getCamera()->setRotXYZ(0.0, 0.0, 0.0);

		// sunlight init
		sunLightPos[0] = sunLightPos[1] = sunLightPos[2] = 0.0;
		sunLightPos[3] = 1.0;

		sunLightDir[0] = sunLightDir[1] = sunLightDir[2] = 0.0;

		GLfloat sac[4] = {0.1,0.1,0.1,1.0}; memcpy(sunLightAmbientColor, sac, sizeof(sac));
		GLfloat sdc[4] = {1.0,1.0,1.0,1.0}; memcpy(sunLightDiffuseColor, sdc, sizeof(sdc));
		GLfloat ssc[4] = {1.0,1.0,1.0,1.0}; memcpy(sunLightSpecularColor, ssc, sizeof(ssc));

		GLfloat sky[4] = {0.0f,0.55f,0.6f,1.0f}; memcpy(skyColor, sky, sizeof(sky));
		terrainScene = false;
		std::cout << "TWS::SceneManager inited" << std::endl;
	}

	SceneManager::~SceneManager() {
		std::cout << "TWS::SceneManager will dealoc children" << std::endl;
		delete m_Terrain;
		delete m_Planet;
		std::cout << "TWS::SceneManager dying" << std::endl;
	}

	void SceneManager::resize(int x, int y) {
		if (y <= 0)
			y = 1;

		glViewport(0,0,x,y);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f,1.0*(GLfloat)x/(GLfloat)y,1.0f,25000000.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	bool SceneManager::processEvents() {
		SDL_Event event;
		SDLKey sym;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: return false; break;
				case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
					m_mouseButtons[0] = 1;
				if (event.button.button == SDL_BUTTON_RIGHT)
					m_mouseButtons[1] = 1;
				break;
				case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
					m_mouseButtons[0] = 0;
				if (event.button.button == SDL_BUTTON_RIGHT)
					m_mouseButtons[1] = 0;
				break;
				case SDL_MOUSEMOTION:
					// use relative motion for fps/cam
					// absolute positioning for rts
					m_mousePosition[0] = event.motion.x;
					m_mousePosition[1] = event.motion.y;
					/*
					std::cout << "Mouse motion: " << event.motion.x << "," << event.motion.y <<
					"(" << event.motion.xrel << "," << event.motion.yrel << ")" << std::endl;*/
				break;
				case SDL_KEYDOWN:
				sym = event.key.keysym.sym;
				if (sym == SDLK_ESCAPE)
					return false;
				m_Keys[sym] = 1;
				break;
				case SDL_KEYUP:
				sym = event.key.keysym.sym;
				m_Keys[sym] = 0;
				break;
				case SDL_VIDEORESIZE: resize(event.resize.w, event.resize.h); break;
				default: break;
			}
		}
		return true;
	}

	void SceneManager::run() {
		while (processEvents()) {
			draw();
			processKeys();
			SDL_GL_SwapBuffers();
		}
	}

	void SceneManager::processKeys() {
		Camera *cam = TWS::getCamera();

		double angle_inc = 0.0000003 * cam->getPosLLA(2);
		float zoom_inc = 0.03 * cam->getPosLLA(2);

		// pitch, yaw
		if (m_Keys[SDLK_RIGHT])
			cam->incrementRotXYZ(0.0, -1.0, 0.0);
		if (m_Keys[SDLK_LEFT])
			cam->incrementRotXYZ(0.0, 1.0, 0.0);
		if (m_Keys[SDLK_UP])
			cam->incrementRotXYZ(-1.0, 0.0, 0.0);
		if (m_Keys[SDLK_DOWN])
			cam->incrementRotXYZ(1.0, 0.0, 0.0);
		if (m_Keys[SDLK_d])
			cam->incrementPosLLA(0.0, angle_inc, 0.0);
		if (m_Keys[SDLK_a])
			cam->incrementPosLLA(0.0, -angle_inc, 0.0);
		if (m_Keys[SDLK_w])
			cam->incrementPosLLA(angle_inc, 0.0, 0.0);
		if (m_Keys[SDLK_s])
			cam->incrementPosLLA(-angle_inc, 0.0, 0.0);
		if (m_Keys[SDLK_q])
			cam->incrementPosLLA(0.0, 0.0, zoom_inc);
		if (m_Keys[SDLK_z])
			cam->incrementPosLLA(0.0, 0.0, -zoom_inc);
	}

	void SceneManager::processClicks() {
		//if (m_mouseButtons[0] != 1)
		//  return;
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;

		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);

		winX = (float)m_mousePosition[0];
		winY = (float)viewport[3] - (float)m_mousePosition[1];

		// bottom left
		glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
		m_mouseButtons[0] = 0;
	}

	bool SceneManager::init() {
		if (!m_Window.createWindow(1680, 1050, 32, true, "TWS")) {
			std::cout << "Could not create window" << std::endl;
			return false;
		}

		// specify shading model
		glShadeModel(GL_SMOOTH);

		// enable blending and specify how to blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// specify line antialias
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		// line and point width
		glLineWidth(1.0);
		glPointSize(1.0);

		// specify culling method and enable
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		// perspective correction
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		// wireframe for debug
		glPolygonMode(GL_FRONT,GL_FILL);

		// depth testing
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.0);

		// enable lighting
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT_MODEL_AMBIENT);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, sunLightPos);
		glLightfv(GL_LIGHT0, GL_AMBIENT, sunLightAmbientColor);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, sunLightDiffuseColor);
		glLightfv(GL_LIGHT0, GL_SPECULAR, sunLightSpecularColor);

		// set fog properties
		glFogi(GL_FOG_MODE, GL_EXP);
		glFogfv(GL_FOG_COLOR, skyColor);
		glFogf(GL_FOG_DENSITY, 1.0f);
		glHint(GL_FOG_HINT, GL_NICEST);
		glFogf(GL_FOG_START, 8000.0f);
		glFogf(GL_FOG_END, 30000.0f);
		glFogi(GL_FOG_COORD_SRC, GL_FOG_COORDINATE);

		this->m_Planet = new Planet();
		this->m_Terrain = new TerrainManager();
		this->m_SkyDome = new SkyDome();
		this->debugFont = new RasterFont();

		return true;
	}

	void SceneManager::draw() {
		Camera *cam = TWS::getCamera();
		TWS_Astro::getsuncoords(&suncoords[0]);

		// copy to vertex
		Color4 sun = Color4(0,0,0,0);

		// set z-buffer resolution according to needed by altitude
		if (cam->getPosLLA(2) < 30000.0) {
			// terrain scene. was it already a terrain scene?
			if (!terrainScene) {
				// not... update camera xyz from lla
				cam->LLA2XYZ();
				terrainScene = true;
			}

			// update light position
			float lat_offset = cam->getPosLLA(0) - suncoords[0];
			float lon_offset = cam->getPosLLA(1) - suncoords[1];

			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			GLfloat near = 1.0;
			GLfloat far = 40000.0;
			GLdouble half_height = near * tan( 22.5 * 0.0174532925);
			GLdouble half_width = half_height * m_Window.getWidth()/m_Window.getHeight();
			glFrustum( -half_width, half_width, -half_height, half_height, near, far );
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			// prepare for reflection
			glCullFace(GL_FRONT);

			// set clip plane to avoid processing water vertices
			#pragma mark SET THE REFLECTION CLIP PLANE
			glPushMatrix();
				glRotatef(-cam->getRotXYZ(0),1.0,0.0,0.0);
				glRotatef(-cam->getRotXYZ(1),0.0,1.0,0.0);
				glTranslatef(0.0,cam->getPosXYZ(1),0.0);
				glRotatef(-lon_offset,0.0,0.0,1.0);
				glRotatef(-lat_offset,1.0,0.0,0.0);
				glTranslatef(0.0,-30000.0,0.0);
				glPushMatrix();
					glScalef(1.0,-1.0,1.0);
					sunLightPos[0] = sunLightPos[1] = sunLightPos[2] = 0.0;
					glLightfv(GL_LIGHT0, GL_POSITION, sunLightPos);
				glPopMatrix();
			glPopMatrix();

			glPushMatrix();
				glDepthMask(GL_FALSE);
				glRotatef(-cam->getRotXYZ(0),1.0,0.0,0.0);
				glRotatef(-cam->getRotXYZ(1),0.0,1.0,0.0);
				glTranslatef(0.0,cam->getPosXYZ(1),0.0);
				glScalef(1.0,-1.0,1.0);
				m_SkyDome->draw(lat_offset, lon_offset);
				glDepthMask(GL_TRUE);
			glPopMatrix();

			// terrain
			glPushMatrix();
				glRotatef(-cam->getRotXYZ(0),1.0,0.0,0.0);
				glRotatef(-cam->getRotXYZ(1),0.0,1.0,0.0);
				glTranslatef(-cam->getPosXYZ(0), -cam->getPosXYZ(1), -cam->getPosXYZ(2));
				calculateFrustum();
				glPushMatrix();
					glScalef(1.0,-1.0,1.0);
					const double eq[4] = {0.0,0.0,0.0,0.0};
					glClipPlane(GL_CLIP_PLANE0, eq);
					glEnable(GL_CLIP_PLANE0);
					m_Terrain->draw(lat_offset,lon_offset);
					// save reflected scene to texture
					glBindTexture(GL_TEXTURE_2D, m_Terrain->m_reflectionTexture);
					glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,0,0,1280,800,0);
					glDisable(GL_CLIP_PLANE0);
				glPopMatrix();
			glPopMatrix();

			// clear color and depth buffers (clean screen)
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			// reset cull face
			glLoadIdentity();
			glCullFace(GL_BACK);

			glPushMatrix();
				glRotatef(-cam->getRotXYZ(0),1.0,0.0,0.0);
				glRotatef(-cam->getRotXYZ(1),0.0,1.0,0.0);
				glTranslatef(0.0,-cam->getPosXYZ(1),0.0);
				glRotatef(-lon_offset,0.0,0.0,1.0);
				glRotatef(-lat_offset,1.0,0.0,0.0);
				glTranslatef(0.0,30000.0,0.0);
				glPushMatrix();
					sunLightPos[0] = sunLightPos[1] = sunLightPos[2] = 0.0;
					glLightfv(GL_LIGHT0, GL_POSITION, sunLightPos);
				glPopMatrix();
			glPopMatrix();

			glPushMatrix();
				glDepthMask(GL_FALSE);
				glRotatef(-cam->getRotXYZ(0),1.0,0.0,0.0);
				glRotatef(-cam->getRotXYZ(1),0.0,1.0,0.0);
				glTranslatef(0.0,-cam->getPosXYZ(1),0.0);
				m_SkyDome->draw(lat_offset, lon_offset);
				glDepthMask(GL_TRUE);
			glPopMatrix();

			// terrain
			glPushMatrix();
				glRotatef(-cam->getRotXYZ(0),1.0,0.0,0.0);
				glRotatef(-cam->getRotXYZ(1),0.0,1.0,0.0);
				glTranslatef(-cam->getPosXYZ(0), -cam->getPosXYZ(1), -cam->getPosXYZ(2));
				calculateFrustum();
				m_Terrain->draw(lat_offset,lon_offset);
				PersistentActor *theActor = new PersistentActor();
				theActor->setPosLLA(38.678667,-9.171191,300);
				theActor->LLA2XYZ();
				theActor->draw();
				delete theActor;
				//processClicks(); // after terrain is drawn
			glPopMatrix();
		}
		else {
			if (terrainScene) {
				// last frame was a terrain scene. copy xyz to lat/lon
				cam->XYZ2LLA();
				terrainScene = false;
			}
			glClearColor(0.0,0.0,0.0,1.0);
			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			// planetary scene
			gluPerspective(45.0f,m_Window.getWidth()/m_Window.getHeight(),cam->getPosLLA(2)-5000,cam->getPosLLA(2)+GEO_EARTH_RADIUS*2);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			// set camera position, line-of-sight and up vector
			Color4 temp_cam = Color4(0,0,0,0);
			geocentricToCarthesianReal(&temp_cam, cam->getPosLLA(0), cam->getPosLLA(1), cam->getPosLLA(2));
			gluLookAt(temp_cam.r, temp_cam.g, temp_cam.b, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
			geocentricToCarthesianReal(&sun, suncoords[0], -suncoords[1], pow(2, 8*sizeof(long))-1);

			// copy vertex to light position
			sunLightPos[0] = sun.r;
			sunLightPos[1] = sun.g;
			sunLightPos[2] = sun.b;

			// reposition light
			glLightfv(GL_LIGHT0, GL_POSITION, sunLightPos);

			// draw the planet
			m_Planet->draw(TWS::getCamera()->getPosLLA(2));
		}

		// increment time interval frame counter
		framecount++;

		// display debug info
		renderDebugText();
	}

	void SceneManager::renderDebugText() {
		// display on-screen debug text
		char tmp[255];

		// estimate fps and cap at 30
		gettimeofday(&end,0);

		double a,b;
		a = end.tv_sec + (end.tv_usec/1000000.0);
		b = start.tv_sec + (start.tv_usec/1000000.0);

		framecount++;

		Camera *cam = TWS::getCamera();

		if (cam->getPosLLA(2) < 40000.0)
			m_Terrain->reload();

		TWS::fps = 1.0/(a-b);

		gettimeofday(&start, 0);
		glClear(GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f,1280.0/800.0,1.0,1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_LIGHTING);
		glColor3f(0.0,1.0,0.0);
		glRasterPos3f(-0.62, 0.15, -1.0);
		sprintf(tmp, "SUN XYZ: %3.2f %3.2f %3.2f", sunLightPos[0], sunLightPos[1], sunLightPos[2]);
		debugFont->printString(tmp);

		glColor3f(0.0,1.0,0.0);
		glRasterPos3f(-0.62, 0.35, -1.0);
		sprintf(tmp, "LLA: %3.2f %3.2f %3.2f", cam->getPosLLA(0), cam->getPosLLA(1), cam->getPosLLA(2));
		debugFont->printString(tmp);

		glRasterPos3f(-0.62, 0.30, -1.0);
		sprintf(tmp, "POS: %3.4f %3.4f %3.4f", cam->getPosXYZ(0), cam->getPosXYZ(1), cam->getPosXYZ(2));
		debugFont->printString(tmp);

		glRasterPos3f(-0.62, 0.25, -1.0);
		sprintf(tmp, "Rot: %3.2f %3.2f %3.2f", cam->getRotXYZ(0), cam->getRotXYZ(1), cam->getRotXYZ(2));
		debugFont->printString(tmp);

		glRasterPos3f(-0.62, 0.20, -1.0);
		sprintf(tmp, "FPS: %3.2f", fps);
		debugFont->printString(tmp);

		strcpy(tmp, "TWS Engine - FCVARELA09");
		glRasterPos3f(-0.62, -0.25, -1.0);
		debugFont->printString(tmp);
		strcpy(tmp, (char *)glGetString(GL_RENDERER));
		glRasterPos3f(-0.62, -0.30, -1.0);
		debugFont->printString(tmp);
		strcpy(tmp, (char *)glGetString(GL_VERSION));
		glRasterPos3f(-0.62, -0.35, -1.0);
		debugFont->printString(tmp);

		glEnable(GL_LIGHTING);
	}
};
