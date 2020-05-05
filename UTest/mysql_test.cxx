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
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword, mysqlDatabase);

	CHECK( mysql.connect() );
}

TEST(Mysql_integration, set_database) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword);
	CHECK( mysql.connect() );

	CHECK( mysql.set_database( mysqlDatabase ) == true );

	std::string non_existent_database = "__DEKU__non_existent__";
	CHECK( mysql.set_database( non_existent_database ) == false );
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

	bool create_database_state = mysql.create_database( mysqlDatabase );
	bool has_database = mysql.has_database( mysqlDatabase );
	CHECK( create_database_state == true and has_database == true );

	bool delete_database_state = mysql.delete_database( mysqlDatabase );
	has_database = mysql.has_database( mysqlDatabase );
	CHECK( delete_database_state == true and has_database == false );
}

TEST(Mysql_integration, has_table ) {
}

TEST(Mysql_integration, create_table) {
}

TEST(Mysql_integration, delete_table) {
}

int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
