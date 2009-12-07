/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Database.h
 *  
 *
 *  Created by Filipe Varela on 09/06/05.
 *  Copyright 2009 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_DATABASE
#define _TWS_DATABASE

#include <mysql++.h>
#include <iostream>
#include <iomanip>

namespace TWS {
	class Database
	{
	public:
		Database();
		~Database();

		void countPersistents();
		void printPersistents();
		bool authenticateUser(std::string &username, std::string &password);

	private:
		mysqlpp::Connection *m_connection;
	};
}

#endif
