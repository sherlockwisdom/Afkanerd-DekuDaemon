#include <iostream>
#include <mysql.h>

using namespace std;

int main() {
	
	MYSQL *mysqlConnection;
	MYSQL_RES *mysqlResult;

	string server = "localhost";
	string user = "root";
	string password = "asshole";
	string database = "__DEKU__";


	mysqlConnection = mysql_init( NULL );

	auto mysql_connection_state_good = mysql_real_connect( mysqlConnection, server.c_str(), user.c_str(), password.c_str(), database.c_str(), 0, NULL, 0);

	if( !mysql_connection_state_good ) {
		cerr << "Failed to connect to database: " << mysql_error( mysqlConnection ) << endl;
	}

	auto mysql_query_state = mysql_query( mysqlConnection, "show tables");

	if( mysql_query_state != 0 ) {
		cerr << "Failed to query database: " << mysql_error( mysqlConnection ) << endl;
	}

	mysqlResult = mysql_use_result( mysqlConnection );

	for(MYSQL_ROW mysqlRow = mysql_fetch_row( mysqlResult ); mysqlRow != NULL ; mysqlRow = mysql_fetch_row( mysqlResult ) ) {
		cout << mysqlRow[0] << endl;
	}
	

	return 0;
}
