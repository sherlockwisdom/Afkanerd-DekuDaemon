#include "start_routines.hpp"

using namespace std;

int main() {
	string sys_config_file = "test_dir/test_config.txt";
	if( system_check( sys_config_file ) ) cout << "System file check passed" << endl;
	else cout << "System file check failed" << endl;


	vector<string> file_contents = helpers::read_file( sys_config_file );

	map<string,string> sample_config_file = get_system_configs( file_contents );

	if(
			sample_config_file.find("DIR_ISP") != sample_config_file.end() and
			sample_config_file.find("STD_NAME_REQUEST_FILE") != sample_config_file.end() and
			sample_config_file.find("DIR_REQUEST_FILE") != sample_config_file.end()
	  ) {
		cout << "Get system configs Passed..." << endl;
	}
	else {
		cout << "Get system configs Failed..." << endl;
	}

	return 0;
}
