#include <mysql.h>
#ifndef mysql_H_INCLUDED_
#define mysql_H_INCLUDED_

using namespace std;


class MySQL {
	
	private:
	string server;
	string username;
	string password;
	string database;
	
	MYSQL* mysqlConnection;
	
	public:
	MySQL(string server, string username, string password, string database = "");
	MySQL();

	map<string, vector<string>> query( string );

	//TODO: Put some getters and setters
};

#endif
