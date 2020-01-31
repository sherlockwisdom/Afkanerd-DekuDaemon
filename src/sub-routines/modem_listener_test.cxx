#include "modem_listener.cxx"

using namespace std;


int main() {
	Modems modems;

	modems = modems.getAll();
	
	if(modems.getAllIndexes().size() == list_of_test_modems.size()) {
	}

	else {}

	if(modems.getAllIndexes() == list_of_test_modems) {}
	else {}

	if(modems.getAllIsp() == list_of_test_modems_isp) {}
	else {}

	if(modems.getIMEI() == list_of_test_modems_imei) {}
	else {}
}
