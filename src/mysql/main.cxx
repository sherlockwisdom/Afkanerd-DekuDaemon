#include <iostream>
#include <mysql.h>

using namespace std;

int main() {
	
	MYSQL *mysqlConnection;
	MYSQL_RES *mysqlResult;
	MYSQL_ROW mysqlRow;

	string server = "localhost";
	string user = "root";
	string password = "asshole";
	string database = "__DEKU__";


	mysqlConnection = mysql_init( NULL );

	auto mysql_connection_state_good = mysql_real_connect( mysqlConnection, server.c_str(), user.c_str(), password.c_str(), database.c_str(), 0, NULL, 0);
	auto mysql_connection_state_bad = mysql_real_connect( mysqlConnection, "localhost_1", user.c_str(), password.c_str(), database.c_str(), 0, NULL, 0 );

	cout << "Good state: "  << mysql_connection_state_good << endl;
	cout << "Bad state: " << mysql_connection_state_bad << endl;

	return 0;
}
