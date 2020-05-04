#include <mysql.h>
#include <iostream>
#include <map>
#include <vector>

#ifndef mysql_H_INCLUDED_
#define mysql_H_INCLUDED_

using namespace std;


class MySQL {
	
	private:
	string server;
	string user;
	string password;
	string database;
	
	MYSQL* mysqlConnection;
	MYSQL_RESULT* mysqlResult;

	bool initialized = false;
	
	public:
	MySQL(string server, string user, string password, string database);
	MySQL();
	~MySQL();

	map<string,map<string,string>> query( string );

	bool connect();
	bool is_init() const;

	//TODO: Put some getters and setters
	void setConnectionDetails( string server, string user, string password, string database );
	void close();

	string get_server() const;
	string get_user() const;
	string get_password() const;
	string get_database() const;

	bool set_database( string database );
	bool has_database( string database ) const;
	bool delete_database( string database );
	bool delete_database(  );
	bool create_database( string database );
};

#endif
