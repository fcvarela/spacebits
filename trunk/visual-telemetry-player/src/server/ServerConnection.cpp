/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  ServerConnection.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 09/06/05.
 *  Copyright 2009 Filipe Varela. All rights reserved.
 *
 */

#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include "ServerConnection.h"
#include "ServerFactory.h"

namespace TWS {
	ServerConnection::ServerConnection(int connectionSocket) {
		std::cout << "TWS::ServerConnection(" << connectionSocket << ") alloc" << std::endl;
		m_done = false;
		m_connectionSocket = connectionSocket;
	}

	void ServerConnection::handle() {
		char inbuffer[512] = "";
		char outbuffer[512] = "";
		int count;

		/* handle the new connection request  */
		while ((count = read(m_connectionSocket, inbuffer, sizeof(inbuffer))) > 0) {
			std::cout << "TWS::ServerConnection(" << m_connectionSocket << ") got msg" << std::endl;

			/* write message back to the client */
			if (parseMsg(inbuffer, outbuffer))
				write(m_connectionSocket, outbuffer, count);
			else
				m_done = true;

			bzero(inbuffer, sizeof(inbuffer));
			bzero(inbuffer, sizeof(outbuffer));
		}

		// Client closed connection: close socket and finish thread
		close(m_connectionSocket);
		m_done = true;
	}

	bool ServerConnection::parseMsg(char *inbuffer, char *outbuffer) {
		std::string fullCmd = std::string(inbuffer);

		if (fullCmd.substr(0,3) == "001") {
			std::string username = fullCmd.substr(3,8);
			std::string password = fullCmd.substr(11,8);

			std::cout << "TWS::ServerConnection authenticating user: " << username << std::endl;
			bool auth = getDatabase()->authenticateUser(username, password);

			if (auth)
				std::cout << "TWS::ServerConnection authentication passed" << std::endl;
			else {
				std::cout << "TWS::ServerConnection authentication failed" << std::endl;
				return false;
			}

			return true;
		}

		std::cout << "TWS::ServerConnection unknown message format" << std::endl;
		return false;
	}

	ServerConnection::~ServerConnection() {
		close(m_connectionSocket);
		std::cout << "TWS::ServerConnection(" << m_connectionSocket << ") dealloc" << std::endl;
	}
}
