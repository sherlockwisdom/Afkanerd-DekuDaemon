#include "modem_listener.cxx"

using namespace std;


int main() {
	map<string, string> list_of_test_modems = {{"imei","3956c86f0af850256cc1689f4ee2441d9432dbdb"}, {"index","1"}}; //FORMAT, {"imei", "index"}
	vector<string> list_of_test_modems_indexes = {"1"};
	vector<string> list_of_test_modems_isp = {"MTN"};
	vector<string> list_of_test_modems_imei = {"3956c86f0af850256cc1689f4ee2441d9432dbdb"};

	Modems modems;

	modems = modems.getAll();
	if(modems.getAllIndexes().size() == list_of_test_modems.size()) {
	}

	else {
		logger::logger(__FUNCTION__, "Not all modems accounted", "stderr" );
	}

	if(modems.getAllIndexes() == list_of_test_modems_indexes) {
	
	}
	else {
		logger::logger(__FUNCTION__, "Modems indexes do not match", "stderr");
	}

	if(modems.getAllISP() == list_of_test_modems_isp) {}
	else {
		logger::logger(__FUNCTION__, "Modems ISP do not match", "stderr");
	}

	if(modems.getAllIMEI() == list_of_test_modems_imei) {}
	else {
		logger::logger(__FUNCTION__, "Modems IMEI do not match", "stderr");
	}

	return 0;
}
