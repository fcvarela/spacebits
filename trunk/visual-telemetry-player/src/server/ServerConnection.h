/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  ServerConnection.h
 *  TWS
 *
 *  Created by Filipe Varela on 09/06/05.
 *  Copyright 2009 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_SERVERCONNECTION
#define _TWS_SERVERCONNECTION

namespace TWS {
	class ServerConnection
	{
	public:
		ServerConnection(int connectionSocket);
		~ServerConnection();
		void handle();

		bool m_done;

	private:
		int m_connectionSocket;
		bool parseMsg(char *inbuff, char *outbuff);
	};
}

#endif
