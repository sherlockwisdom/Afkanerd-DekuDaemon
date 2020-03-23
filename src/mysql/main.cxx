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

	auto mysql_query_state = mysql_query( mysqlConnection, "SELECT * FROM __REQUEST__");

	if( mysql_query_state != 0 ) {
		cerr << "Failed to query database: " << mysql_error( mysqlConnection ) << endl;
	}

	mysqlResult = mysql_use_result( mysqlConnection );

	MYSQL_FIELD *fields = mysql_fetch_fields( mysqlResult );
	size_t num_fields = mysql_num_fields( mysqlResult );
	for(size_t i=0;i<num_fields;++i) {
		cout << "FIELD: " << fields[i].name << endl;
	}

	for(MYSQL_ROW mysqlRow = mysql_fetch_row( mysqlResult ); mysqlRow != NULL ; mysqlRow = mysql_fetch_row( mysqlResult ) ) {
		auto num_of_fields = mysql_num_fields( mysqlResult );
		for( size_t i = 0; i< num_of_fields; ++i) {
			cout << mysqlRow[i] << ",";
		}
		cout << endl;
	}

	mysql_free_result ( mysqlResult );
	mysql_close ( mysqlConnection );
	

	return 0;
}
