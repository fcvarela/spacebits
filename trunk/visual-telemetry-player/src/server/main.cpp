/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <signal.h>

#include <iostream>
#include <exception>

#include "IniTools.h"
#include "ServerFactory.h"
#include "ServerConnectionPool.h"

void INThandler(int);

TWS::ServerConnectionPool *pool;
TWS::Database *localdb;

int main(int argc, char *argv[])
{
	std::cout << "TWSServer bootstrap..." << std::endl;
	TWS::setIniFile("Config/server.ini");
	
	// install a handler to break the loop and kill parent sockets
	signal(SIGINT, INThandler);

	// Get a pointer to the database connection so we can clean it on quit
	try {
		localdb = TWS::getDatabase();
	
		// do some debug
		localdb->countPersistents();
		localdb->printPersistents();
	} catch (const std::exception& e) {
		std::cout << "TWSServer cannot connect to database" << std::endl;
		return 0;
	}
	
	// start the connection loop
	try {
		pool = new TWS::ServerConnectionPool();
		pool->startConnLoop();
	} catch (std::runtime_error e) {
		std::cout << "TWSServer cannot spawn connection pool" << std::endl << " + " << e.what() << std::endl;
		delete localdb;
		return 0;
	}

	return 0;
}

void INThandler(int sig)
{
	std::cout << "TWSServer terminating connection pool loop..." << std::endl;

	// clean the connection pool
	delete pool;

	// clean db connection
	delete localdb;

	exit(0);
}
