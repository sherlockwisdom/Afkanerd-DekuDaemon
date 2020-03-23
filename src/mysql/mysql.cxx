#include "mysql.hpp"
#include "../formatters/helpers.hpp"
#include <cstring>
#include <iostream>

using namespace std;

MySQL::MySQL(string server, string username, string password, string database = "" ) {
	this->server = server;
	this->username = username;
	this->password = password;
	this->database = database;

	mysqlConnection = mysql_init( NULL );
}

MySQL::MySQL() {}

map<string, vector<string>> MySQL::query( string query ) {
	map<string, vector<string>> query_results;
	auto mysql_query_state = mysql_query( this->mysqlConnection, query.c_str() );

	if( mysql_query_state != 0 ) {
		logger::logger(__FUNCTION__, "Failed to query database: " + string(mysql_error( mysqlConnection ), strlen(mysql_error( mysqlConnection))), "stderr");
	}

	MYSQL_RES *mysqlResult = mysql_use_result( mysqlConnection );

	MYSQL_FIELD *fields = mysql_fetch_fields( mysqlResult );
	for(MYSQL_ROW mysqlRow = mysql_fetch_row( mysqlResult ); mysqlRow != NULL ; mysqlRow = mysql_fetch_row( mysqlResult ) ) {
		auto num_of_fields = mysql_num_fields( mysqlResult );
		for( size_t i = 0; i< num_of_fields; ++i) {
			string row_header = fields[i].name;
			if( query_results.find( row_header ) != query_results.end() ) 
				query_results.insert(make_pair(row_header, vector<string>{}));
			query_results[row_header].push_back( mysqlRow[i] );
		}
	}

	return query_results;
}
