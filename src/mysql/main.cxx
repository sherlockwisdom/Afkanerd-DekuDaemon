#include <iostream>
#include <mysql.h>

using namespace std;

int main() {
	
	MYSQL *mysqlConnection;
	MYSQL_RES *mysqlResult;
	MYSQL_ROW mysqlRow;

	string server = "localhost_1";
	string user = "root";
	string password = "asshole";
	string database = "__DEKU__";


	mysqlConnection = mysql_init( NULL );

	auto mysql_connection_state_good = mysql_real_connect( mysqlConnection, server.c_str(), user.c_str(), password.c_str(), database.c_str(), 0, NULL, 0);

	if( !mysql_connection_state_good ) {
		cerr << "Failed to connect to database: " << mysql_error( mysqlConnection ) << endl;
	}

	return 0;
}
