/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  ServerConnectionPool.h
 *  TWS
 *
 *  Created by Filipe Varela on 09/06/05.
 *  Copyright 2009 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_SERVERCONNECTIONPOOL
#define _TWS_SERVERCONNECTIONPOOL

#include <vector>
#include <iterator>
#include "ServerConnection.h"

namespace TWS {
	class ServerConnectionPool
	{
	public:
		ServerConnectionPool();
		~ServerConnectionPool();
		void startConnLoop();
		void initUDP();
		void initTCP();

		void spawnServerConnection(int socketval);
		void cleanup(void);
		void disconnectAll(void);
	private:
		unsigned long long m_packetCount;
		std::vector<ServerConnection *> m_connections;
		std::vector<ServerConnection *>::iterator m_connectionIterator;
		struct timeval start,end;
		int serverSocket;
	};

	void *connectionHandler(void *connectionSocket);
	void *_initUDP(void *thePool);
	void *_initTCP(void *thePool);
}

#endif
