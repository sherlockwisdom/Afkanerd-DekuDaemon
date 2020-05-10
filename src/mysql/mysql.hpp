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
	string error_message;
	
	MYSQL* mysqlConnection;
	MYSQL_RES* mysqlResult;

	bool initialized = false;
	
	public:
	MySQL(string server, string user, string password, string database);
	MySQL();
	~MySQL();

	map<string,vector<string>> get_results();

	bool connect();
	bool is_init() const;
	bool query( string query );

	//TODO: Put some getters and setters
	void setConnectionDetails( string server, string user, string password, string database );
	void set_error_message( const char* );
	void close();

	string get_server() const;
	string get_user() const;
	string get_password() const;
	string get_database() const;
	string get_error_message() const;

	bool set_database( string database );
	bool has_database( string database ) const;
	bool delete_database( string database );
	bool delete_database(  );
	bool create_database( string database );
	bool has_table( string table ) const;
	bool delete_table( string table );
	bool create_table( string column_name, string column_types );
};

#endif
