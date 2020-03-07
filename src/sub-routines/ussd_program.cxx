#include "ussd.cxx"

using namespace std;


int main(int argc, char** argv) {
	if(argc < 2 ) {
		logger::logger(__FUNCTION__, "No arguments passed", "stderr", true);
		return 1;
	}

	vector<string> request;
	//vector<string> arguments {"5", "2"};
	vector< vector<string> > arguments;
	//TODO: Path to script should be passed as an argument
	// string path_to_script = "\"/home/sherlock/Desktop/Deku Daemon/scripts\"";
	string path_to_script;
	//TODO: Modem index should be passed as an argument
	string modem_index;

	for(int i=1;i<argc;++i) {
		if((string)argv[i] == "-m") {
			if(i+1 >= argc) {
				// TODO: 
				return 1;
			}
			logger::logger(__FUNCTION__, "Executing modem: " + (string)argv[i+1]);
			modem_index = (string)argv[i+1];
			++i;
		}
		else if((string)argv[i] == "-c") {
			if( i+1 >= argc ) {
				return 1;
			}

			logger::logger(__FUNCTION__, "Configuration file: " + (string)argv[i+1]);
			path_to_script = (string)argv[i+1];
			++i;
		}

		else if((string)argv[i] == "-sc") {
			if( i+1 >= argc ) {
				logger::logger(__FUNCTION__, "USSD needed after -sc command", "stderr", true);
				return 1;
			}
			logger::logger(__FUNCTION__, "processing ussd: " + (string)argv[i+1]);
			string shortcode = (string)argv[i+1];
			i++;
			
			request = helpers::split(shortcode, '|', true);
			continue;
		}
		else if((string)argv[i] == "-f") {
			string filepath = (string)argv[i+1];
			++i;

			// It should all go in one line and seperated the usual way |
			for( auto args : helpers::read_file( filepath ) ) 
				arguments.push_back( helpers::split( args, '|', true ));
			continue;
		}
	}

	if( modem_index.empty()) {
		logger::logger(__FUNCTION__, "Modem index not supplied", "stderr", true);
		return 1;
	}

	if( path_to_script.empty()) {
		logger::logger(__FUNCTION__, "Configuration path missing", "stderr", true);
		return 1;
	}

	map<string,string> config {{"DIR_SCRIPTS", path_to_script}};	

	USSD ussd(modem_index, config);

	if( request.size() == 1 ) {
		logger::logger(__FUNCTION__, "Executing 1 command...", "stdout", true);
		logger::logger("", ussd.initiate( request[0] ), "stdout", true);
	}
	else if( request.size() > 1 ) {
		if( arguments.empty()) {
			multimap<string,string> values = ussd.initiate_series( request );
			for(auto value : values ) {
				logger::logger(__FUNCTION__, value.first + "\n====>\n" + value.second + "\n", "stdout", true);
			}
		}
		else {
			for(auto arg : arguments ) {
				logger::logger(__FUNCTION__, "Executing with args: " + helpers::vector_to_string(arg, ' '));
				multimap<string,string> values = ussd.initiate_series( arg, request);
				for(auto value : values ) {
					logger::logger(__FUNCTION__, value.first + "\n====>\n" + value.second + "\n", "stdout", true);
				}
				string _continue;
				//logger::logger(__FUNCTION__, "Done executing... continue? [yes|no]: ");
				cout << __FUNCTION__ << "=> Done executing... continue? [yes|no]: ";
				getline(cin, _continue);

				if(_continue == "yes") {
					helpers::sleep_thread( 3 );
					continue;
				}
				else break;
			}
		}
	}
	else {
		//TODO: some error
	}
	return 0;
}
