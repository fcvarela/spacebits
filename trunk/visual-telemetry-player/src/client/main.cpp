/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <iostream>
#include <SceneManager.h>

int main(int argc, char *argv[])
{
	//Create a lesson instance
	TWS::SceneManager *sceneManager = new TWS::SceneManager();

	//If we can initialize the lesson
	if(sceneManager->init())
        sceneManager->run();
	else 
		std::cout << "TWS::SceneManager initializiation failed!" << std::endl;
	
	delete sceneManager;
		
	return 0;
}
