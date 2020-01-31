#include "modem_listener.cxx"

using namespace std;


int main() {
	map<string, string> list_of_test_modems;
	vector<string> list_of_test_modems_indexes;
	vector<string> list_of_test_modems_isp;
	vector<string> list_of_test_modems_imei;
	Modems modems;

	modems = modems.getAll();
	if(modems.getAllIndexes().size() == list_of_test_modems.size()) {
	}

	else {}

	if(modems.getAllIndexes() == list_of_test_modems_indexes) {}
	else {}

	if(modems.getAllISP() == list_of_test_modems_isp) {}
	else {}

	if(modems.getAllIMEI() == list_of_test_modems_imei) {}
	else {}

	return 0;
}
