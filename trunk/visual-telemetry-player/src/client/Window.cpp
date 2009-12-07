/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <iostream>
#include "Window.h"

namespace TWS {
    Window::Window(): m_Width(0), m_Height(0), m_Bpp(0), m_Fullscreen(false) {}
    Window::~Window() {SDL_Quit();}
    
    bool Window::createWindow(int width, int height, int bpp, bool fullscreen, const string& title)
    {
        if( SDL_Init( SDL_INIT_VIDEO ) != 0 ) 
            return false;
        
        const SDL_VideoInfo *vid = SDL_GetVideoInfo();
        
        if (fullscreen) {
            // set total resolution
            m_Width = vid->current_w;
            m_Height = vid->current_h;
        } else {
            m_Height = height;
            m_Width = width;
        }
        
        std::cout << "TWS::Window setting params: " << ((fullscreen == true) ? "Full Screen @ " : "Windowed @ ") << m_Width << "," << m_Height << std::endl;
        
        m_Title = title;
        m_Fullscreen = fullscreen;
        m_Bpp = bpp;
        
        // request "at least" this
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
        
        // need to fix our coord system so we don't need 32 bits here
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
        
        // Set the title.
        SDL_WM_SetCaption(title.c_str(), title.c_str());
        
        // Flags tell SDL about the type of window we are creating.
        int flags = SDL_OPENGL;
        
        if (fullscreen == true)
            flags |= SDL_FULLSCREEN;
        
        // Create the window
        SDL_Surface * screen = SDL_SetVideoMode( m_Width, m_Height, m_Bpp, flags );
        if (screen == 0) return false;
        
        //SDL doesn't trigger off a ResizeEvent at startup, but as we need this for OpenGL, we do this ourself
        SDL_Event resizeEvent;
        resizeEvent.type = SDL_VIDEORESIZE;
        resizeEvent.resize.w = m_Width;
        resizeEvent.resize.h = m_Height;
        
        SDL_PushEvent(&resizeEvent);
        int rgbaSize[4],depthSize,msBuffers,msSamples,swapControl;
        
        SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &rgbaSize[0]);
        SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &rgbaSize[1]);
        SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &rgbaSize[2]);
        SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &rgbaSize[3]);
        SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &depthSize);
        SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &msBuffers);
        SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &msSamples);
        SDL_GL_GetAttribute(SDL_GL_SWAP_CONTROL, &swapControl);
        
        std::cout << "TWS::Window is set up" << std::endl;
        std::cout << "TWS::Window RGBA sizes are " << rgbaSize[0] << "," << rgbaSize[1] << "," << rgbaSize[2] << "," << rgbaSize[3] << " bits" << std::endl;
        std::cout << "TWS::Window SDL_GL_DEPTH_SIZE is " << depthSize << " bits" << std::endl;
        std::cout << "TWS::Window SDL_GL_MULTISAMPLEBUFFERS is " << msBuffers << std::endl;
        std::cout << "TWS::Window SDL_GL_MULTISAMPLESAMPLES is " << msSamples << std::endl;
        std::cout << "TWS::Window SDL_GL_SWAP_CONTROL (vsync) is " << ((swapControl == 0) ? "disabled" : "enabled") << std::endl;
        
        return true;
    }
    
    float Window::getHeight() {return (float)m_Height;}
    float Window::getWidth()  {return (float)m_Width;}
}

