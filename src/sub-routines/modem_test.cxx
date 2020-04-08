#include "modem_listener.cxx"
#include "saitama.hpp"

int main() {
	MySQL mysqlConnection("localhost", "root", "asshole", "__DEKU__");
	mysqlConnection.connect();

	string modem_index = "1";
	string modem_imei = "358812037638331";
	map<string,string> configs {
		{"DIR_SCRIPTS", "../../scripts"},
	};
	Modem modem;
	modem.set_configs ( configs );
	modem.setIndex( modem_index );
	modem.setIMEI( modem_imei );
	modem.set_mysql_connection( mysqlConnection );

	int workload = modem.db_get_workload();

	cout << "Workload: " << workload << endl;

	modem.db_reset_workload();
	workload = modem.db_get_workload();

	cout << "Workload: " << workload << endl;
	return 0;
}
