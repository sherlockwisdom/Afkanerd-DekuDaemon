#include "mysql.hpp"

using namespace std;

MySQL::MySQL(string server, string username, string password, string database = "" ) {}

MySQL::MySQL() {}

map<string, vector<string>> MySQL::query( string query ) {
	auto mysql_query_state = mysql_query( this->mysqlConnection, query );

	if( mysql_query_state != 0 ) {
		logger::logger(__FUNCTION__, "Failed to query database: " + mysql_error( mysqlConnection ), "stderr");
	}

	mysqlResult = mysql_use_result( mysqlConnection );

	for(MYSQL_ROW mysqlRow = mysql_fetch_row( mysqlResult ); mysqlRow != NULL ; mysqlRow = mysql_fetch_row( mysqlResult ) ) {
		auto num_of_fields = mysql_num_fields( mysqlResult );
		string row_header = mysqlR
		for( size_t i = 0; i< num_of_fields; ++i) {
			cout << mysqlRow[i] << ",";
		}
		cout << endl;
	}
}
