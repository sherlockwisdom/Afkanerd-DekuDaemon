// Custom header files
#include "../src/mysql/mysql.cxx"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"


/// Important notice:
// - The variables are customized to my dev machine, a dynamic form of testing would be appreciated
// customise this before beginning the test
std::string mysqlServer = "localhost";
std::string mysqlUser = "root";
std::string mysqlPassword = "asshole";
std::string mysqlDatabase = "CPPUTEST_MYSQL_TEST_DATABASE";
std::string mysqlTable = "CPPUTEST_MYSQL_TEST_TABLE";
std::string mysqlTable_columnTypes = "CPPUTEST_MYSQL_TEST_FIELD VARCHAR(255) NOT NULL,"
				      "CPPUTEST_MYSQL_TEST_FIELD1 INT NOT NULL AUTO_INCREMENT,"
				      "CPPUTEST_MYSQL_TEST_FIELD2 DATE,"
				      "PRIMARY KEY ( CPPUTEST_MYSQL_TEST_FIELD1 )";

TEST_GROUP(Mysql) {};
TEST_GROUP(Mysql_integration) {};

TEST(Mysql, Constructor) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword, mysqlDatabase);

	STRCMP_EQUAL( mysqlServer.c_str(), mysql.get_server().c_str());
	STRCMP_EQUAL( mysqlUser.c_str(), mysql.get_user().c_str());
	STRCMP_EQUAL( mysqlPassword.c_str(), mysql.get_password().c_str());
	STRCMP_EQUAL( mysqlDatabase.c_str(), mysql.get_database().c_str());
}

TEST(Mysql, setConnectionDetails) {
	MySQL mysql;
	mysql.setConnectionDetails( mysqlServer, mysqlUser, mysqlPassword, mysqlDatabase );

	STRCMP_EQUAL( mysqlServer.c_str(), mysql.get_server().c_str());
	STRCMP_EQUAL( mysqlUser.c_str(), mysql.get_user().c_str());
	STRCMP_EQUAL( mysqlPassword.c_str(), mysql.get_password().c_str());
	STRCMP_EQUAL( mysqlDatabase.c_str(), mysql.get_database().c_str());
}

TEST(Mysql, connect) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword);

	CHECK( mysql.connect() );
}

TEST(Mysql_integration, set_database) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword);
	CHECK( mysql.connect() );

	// bool create_database_state = mysql.create_database( mysqlDatabase );
	bool create_database_state = mysql.create_database( mysqlDatabase );
	bool has_database_state = mysql.has_database( mysqlDatabase );
	bool set_database_state = mysql.set_database( mysqlDatabase );
	CHECK( create_database_state == true and has_database_state == true and set_database_state == true);

	bool delete_database_state = mysql.delete_database( mysqlDatabase );
	has_database_state = mysql.has_database( mysqlDatabase );
	CHECK( delete_database_state == true and has_database_state == false and mysql.set_database( mysqlDatabase ) == false );
}

TEST(Mysql_integration, has_database ) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword);
	CHECK( mysql.connect() );

	bool create_database_state = mysql.create_database( mysqlDatabase );
	bool has_database = mysql.has_database( mysqlDatabase );
	bool delete_database = mysql.delete_database( mysqlDatabase );

	CHECK( create_database_state == true and has_database == true and delete_database == true);

	has_database = mysql.has_database( mysqlDatabase );
	delete_database = mysql.delete_database( mysqlDatabase );

	CHECK( has_database == false and delete_database == false);
}

TEST(Mysql_integration, create_database) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword);
	CHECK( mysql.connect() );

	if( mysql.has_database( mysqlDatabase ) )
		mysql.delete_database( mysqlDatabase );
	bool create_database_state = mysql.create_database( mysqlDatabase );
	bool has_database = mysql.has_database( mysqlDatabase );
	CHECK( create_database_state == true and has_database == true );

	bool delete_database_state = mysql.delete_database( mysqlDatabase );
	has_database = mysql.has_database( mysqlDatabase );
	CHECK( delete_database_state == true and has_database == false );
}

TEST(Mysql_integration, has_table ) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword, mysqlDatabase);
	CHECK( mysql.connect() );

	if( !mysql.has_database( mysqlDatabase ) ) {
		bool create_database_state = mysql.create_database( mysqlDatabase );
		bool has_database = mysql.has_database( mysqlDatabase );
		CHECK( create_database_state == true and has_database == true );
	}

	bool create_table = mysql.create_table( mysqlTable, mysqlTable_columnTypes );
	bool has_table = mysql.has_table( mysqlTable );
	CHECK( create_table == true and has_table == true );

	bool delete_database_state = mysql.delete_database( mysqlDatabase );
	bool has_database = mysql.has_database( mysqlDatabase );
	CHECK( delete_database_state == true and has_database == false );
}

TEST(Mysql_integration, create_table) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword);
	CHECK( mysql.connect() );

	if( !mysql.has_database( mysqlDatabase ) ) {
		bool create_database_state = mysql.create_database( mysqlDatabase );
		bool has_database = mysql.has_database( mysqlDatabase );
		CHECK( create_database_state == true and has_database == true );
	}
	mysql.set_database( mysqlDatabase );

	bool has_table = mysql.has_table( mysqlTable );
	if( has_table ) 
		mysql.delete_table( mysqlTable );

	bool create_table = mysql.create_table( mysqlTable, mysqlTable_columnTypes );
	has_table = mysql.has_table( mysqlTable );
	CHECK( create_table == true and has_table == true );

	bool delete_table_state = mysql.delete_table( mysqlTable );
	has_table = mysql.has_table( mysqlTable );
	CHECK( delete_table_state == true and has_table == false );
}

TEST(Mysql_integration, delete_table) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword);
	CHECK( mysql.connect() );

	if( !mysql.has_database( mysqlDatabase ) ) {
		bool create_database_state = mysql.create_database( mysqlDatabase );
		bool has_database = mysql.has_database( mysqlDatabase );
		CHECK( create_database_state == true and has_database == true );
	}
	mysql.set_database( mysqlDatabase );

	bool has_table = mysql.has_table( mysqlTable );
	if( !has_table ) {
		bool create_table = mysql.create_table( mysqlTable, mysqlTable_columnTypes );
		has_table = mysql.has_table( mysqlTable );
		CHECK( has_table == true and create_table == true );
	}

	bool delete_table_state = mysql.delete_table( mysqlTable );
	has_table = mysql.has_table( mysqlTable );
	CHECK( delete_table_state == true and has_table == false );
}

int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
