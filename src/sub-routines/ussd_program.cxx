#include "ussd.cxx"

using namespace std;


int main(int argc, char** argv) {
	if(argc < 2 ) {
		logger::logger(__FUNCTION__, "No arguments passed", "stderr", true);
		return 1;
	}

	vector<string> request;
	vector<string> arguments;
	//TODO: Path to script should be passed as an argument
	string path_to_script = "\"/home/sherlock/Desktop/Deku Daemon/scripts\"";
	//TODO: Modem index should be passed as an argument
	string modem_index = 0;

	for(int i=1;i<argc;++i) {
		if((string)argv[i] == "-sc") {
			string shortcode = (string)argv[i+1];
			i++;
			
			request = helpers::split(shortcode, '|', true);
			continue;
		}
		else if((string)argv[i] == "-f") {
			string filepath = (string)argv[i+1];
			++i;

			arguments = helpers::read_file( filepath );
			continue;
		}
	}

	map<string,string> config {{"DIR_SCRIPTS", path_to_script}};	

	USSD ussd(modem_index, config);

	if( request.size() == 1 ) {
		logger::logger(__FUNCTION__, "Executing 1 command...", "stdout", true);
		logger::logger(__FUNCTION__, ussd.initiate( request[0] ), "stdout", true);
	}
	else if( request.size() > 1 ) {
		auto values = ussd.initiate_series( request );
		for(auto value : values ) {
			logger::logger(__FUNCTION__, value.first + "\n====>\n" + value.second + "\n", "stdout", true);
		}
	}
	else {
		//TODO: some error
	}
	return 0;
}
