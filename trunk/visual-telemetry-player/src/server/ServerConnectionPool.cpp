/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  ServerConnectionPool.cpp
 *  TWS
 *
 *  Created by Filipe Varela on 09/06/05.
 *  Copyright 2009 Filipe Varela. All rights reserved.
 *
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <exception>
#include <iostream>

#include "ServerFactory.h"
#include "ServerConnectionPool.h"

namespace TWS {
	ServerConnectionPool::ServerConnectionPool() {
		std::cout << "TWS::ServerConnectionPool alloc" << std::endl;
	}

	void ServerConnectionPool::startConnLoop() {
		m_packetCount = 0;
		gettimeofday(&start, 0);

		pthread_t udp_thread;
		pthread_t tcp_thread;

		pthread_create(&udp_thread, NULL, _initUDP, (void *)this);
		pthread_create(&tcp_thread, NULL, _initTCP, (void *)this);

		struct timespec cleanup_ts;
		cleanup_ts.tv_sec = 0;
		cleanup_ts.tv_nsec = 10 * 10000000;

		while (1) {
			nanosleep(&cleanup_ts, NULL);
			cleanup();
		}
	}

	ServerConnectionPool::~ServerConnectionPool() {
		// kill all connections
		disconnectAll();
		close(serverSocket);
		std::cout << "TWS::ServerConnectionPool dealloc" << std::endl;
	}

	void *_initUDP(void *thePool) {
		ServerConnectionPool *pool = (ServerConnectionPool *)thePool;
		pool->initUDP();
	}

	void *_initTCP(void *thePool) {
		ServerConnectionPool *pool = (ServerConnectionPool *)thePool;
		pool->initTCP();
	}

	void ServerConnectionPool::initUDP() {
		std::cout << "TWS::ServerConnectionPool initUDP started" << std::endl;
		struct sockaddr_in si_me, si_other;
		socklen_t slen = sizeof(si_other);
		int s, i;
		size_t packlen;
		char buf[512];
		double a,b;

		if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
			throw std::exception();

		memset((char *)&si_me, 0, sizeof(si_me));
		si_me.sin_family = AF_INET;
		si_me.sin_port = htons(6666);
		si_me.sin_addr.s_addr = htonl(INADDR_ANY);

		int one = 1;
		setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
		setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one));

		if (bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) < 0) {
			std::cerr << "TWS::ServerConnectionPool socket bind error" << std::endl;
			close(s);
			exit(1);
		}

		while(1) {
			if ((packlen = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&si_other, &slen)) < 0) {
				std::cerr << "TWS::ServerConnectionPool read error" << std::endl;
			} else {
				m_packetCount++;
				gettimeofday(&end, 0);
				a = end.tv_sec + (end.tv_usec/1000000.0);
				b = start.tv_sec + (start.tv_usec/1000000.0);
				if (a-b >= 1.0) {
					gettimeofday(&start, 0);
					std::cout << "TWS::ServerConnectionPool packs per sec: " << m_packetCount/(a-b) << std::endl;
					m_packetCount = 0;
				}
			}
		}
	}

	void ServerConnectionPool::initTCP() {
		std::cout << "TWS::ServerConnectionPool initTCP started" << std::endl;
		pthread_t thread;
		int ret;

		if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
			std::cerr << "TWS::ServerConnection could not create socket" << std::endl;
			exit(1);
		}

		struct sockaddr_in server_addr;
		bzero(&server_addr, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		server_addr.sin_port = htons(6667);

		int one = 1;
		setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
		setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one));

		if (bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
			perror("bind");
			std::cerr << "TWS::ServerConnection could not bind socket" << std::endl;
			close(serverSocket);
			exit(1);
		}

		if (listen(serverSocket, 5) < 0) {
			std::cerr << "TWS::ServerConnection could not listen on socket" << std::endl;
			close(serverSocket);
			exit(1);
		}

		while (1) {
			struct sockaddr_in clientName = { 0 };
			int connectionSocket = 0;
			socklen_t clientNameLength = sizeof(clientName);

			if ((connectionSocket = accept(serverSocket, (struct sockaddr *)&clientName, &clientNameLength)) < 0) {
				std::cerr << "TWS::ServerConnection cannot accept connections" << std::endl;
				close(serverSocket);
				close(connectionSocket);
				break;
			}

			ServerConnection *conn = new ServerConnection(connectionSocket);
			m_connections.push_back(conn);
			if (pthread_create(&thread, NULL, connectionHandler, (void *)conn) != 0) {
				std::cerr << "TWS::ServerConnection could not spawn connection handler thread" << std::endl;
				exit(1);
			}
		}
	}

	void ServerConnectionPool::disconnectAll() {
		int len = m_connections.size();

		for (int i=0; i<len; i++) {
			ServerConnection *conn = m_connections[i];
			delete conn;
			m_connections.erase(m_connections.begin() + i);
		}
	}

	void ServerConnectionPool::cleanup() {
		int len = m_connections.size();

		for (int i=0; i<len; i++) {
			ServerConnection *conn = m_connections[i];
			if (conn->m_done == true) {
				delete conn;
				m_connections.erase(m_connections.begin() + i);
			}
		}
	}

	void *connectionHandler(void *connection) {
		ServerConnection *conn = (ServerConnection *)connection;
		conn->handle();
	}
}
