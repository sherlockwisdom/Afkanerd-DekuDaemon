#include "start_routines.hpp"

using namespace std;

int main() {
	string sys_config_file = "test_dir/test_config.txt";

	map<string,string> sample_config_file = get_system_configs( file_contents );

	if(
			sample_config_file.find("DIR_ISP") != sample_config_file.end() and
			sample_config_file.find("STD_NAME_REQUEST_FILE") != sample_config_file.end() and
			sample_config_file.find("DIR_REQUEST_FILE") != sample_config_file.end()
	  ) {
		cout << "Passed..." << endl;
	}
	else {
		cout << "Failed..." << endl;
	}

	return 0;
}
