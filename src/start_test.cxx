#include "start_routines.hpp"
#include "parsers/comma_equals_parser.hpp"

using namespace std;

int main() {
	string sys_config_file = "test_dir/sample_config.txt";
	if( system_check( sys_config_file ) ) cout << "System file check passed" << endl;
	else cout << "System file check failed" << endl;


	vector<string> file_contents = helpers::read_file( sys_config_file );
	logger::logger(__FUNCTION__, "Read sys config file " + to_string(file_contents.size()));

	map<string,string> sample_config_file = get_system_configs( file_contents );
	logger::logger(__FUNCTION__, "Read config file " + to_string(sample_config_file.size()));

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

	string sample="number=1234,message=\"Hello world,remove commas from,here\"";
	vector<string> comma_parsed= parsers::comma_seperate( sample, 0 );

	for(auto i : comma_parsed) cout << i << endl;

	return 0;
}
