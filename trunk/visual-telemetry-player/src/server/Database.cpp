/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  Database.cpp
 *  
 *
 *  Created by Filipe Varela on 09/06/05.
 *  Copyright 2009 Filipe Varela. All rights reserved.
 *
 */

#include "Database.h"
#include "IniTools.h"

namespace TWS {
	Database::Database() {
		char hostname[128],name[128],username[128],password[128];
		
		TWS::stringForHeadingKey("Database", "hostname", hostname, sizeof(hostname)-1);
		TWS::stringForHeadingKey("Database", "name", name, sizeof(name)-1);
		TWS::stringForHeadingKey("Database", "username", username, sizeof(username)-1);
		TWS::stringForHeadingKey("Database", "password", password, sizeof(password)-1);
		
		std::cout << "TWS::Database alloc" << std::endl;
		try {
			m_connection = new mysqlpp::Connection();
			m_connection->connect(name, hostname, username, password);
		} catch (const mysqlpp::Exception& e) {
			std::cout << "TWS::Database database connection error: " << std::endl << " + " << e.what() << std::endl;
		}
	}

	Database::~Database() {
		if (!m_connection)
			delete(m_connection);
			
		std::cout << "TWS::Database dealloc" << std::endl;
	}

	void Database::countPersistents() {
		mysqlpp::Query query = m_connection->query("select count(1) persistent_count from persistent_actors");
		mysqlpp::StoreQueryResult res = query.store();

		if (res)
			std::cout << "TWS::Database got " << res[0]["persistent_count"] << " persistent actors" << std::endl;
	}

	void Database::printPersistents() {
		mysqlpp::Query query = m_connection->query("select * from persistent_actors");
		mysqlpp::StoreQueryResult res = query.store();

		if (res) {
			for (size_t i=0; i<res.num_rows(); i++) {
				std::cout << "TWS::Database got object with latitude: " << res[i]["latitude"] << " longitude: " << res[i]["longitude"] << std::endl;
			}
		}
	}

	bool Database::authenticateUser(std::string &username, std::string &password) {
		mysqlpp::Query query = m_connection->query("select count(1) valid from users where username='"+username+"' and password=md5('"+password+"')");
		mysqlpp::StoreQueryResult res = query.store();

		if (res)
			return res[0]["valid"];

		return false;
	}
}
