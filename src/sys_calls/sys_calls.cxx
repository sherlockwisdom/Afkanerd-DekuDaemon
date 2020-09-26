#include "sys_calls.hpp"
#include "../formatters/helpers.hpp"
using namespace std;

namespace sys_calls {
	void sys_reboot() {
		int cmd =  0x1234567;
		int reboot_state = reboot( cmd );
		logger::logger(__FUNCTION__, "REBOOT RETURNED: " + to_string( reboot_state));
		logger::logger_errno( errno, __LINE__, __FUNCTION__ );
	}

	bool file_handlers( string file_path, FILE_FLAG flag) {
		switch( flag ) {
			case EXIST:{
				struct stat buffer; 
				return stat( file_path.c_str(), &buffer) == 0;
			}
			break;

			case DEL:
				if( std::remove(file_path.c_str()) == 0) return true;
				else return false;
			break;

		}
		return false;
	}

	void make_dir( string path_dirname ) {
		size_t start_pos = path_dirname[0] == '/' ? 1 : 0;
		vector<string> recursive_paths = helpers::string_split(path_dirname, '/', start_pos);
		string make_me = recursive_paths[0];
		for(size_t i=0;i<recursive_paths.size();++i) {
			//logger::logger(__FUNCTION__, "Making dir: " + make_me, "stdout", false);
			if( i!=0) make_me += "/" + recursive_paths[i];
			if( mkdir( make_me.c_str(), 0777 ) == -1) {
				if( errno != 17 ) logger::logger_errno( errno, __LINE__, __FUNCTION__ );
			}
		}
		
		return;
	}

	string terminal_stdout(string command) {
		string data;
		FILE * stream;
		const int max_buffer = 1024 * 10; // This isn't good
		char buffer[max_buffer];
		command.append(" 2>&1");

		try {
			stream = popen(command.c_str(), "r");
			if (stream) {
				while (!feof(stream)) 
					if (fgets(buffer, max_buffer, stream) != NULL) 
						data.append(buffer);
				pclose(stream);
			}
		}
		catch( std::exception& e) {
			logger::logger(__FUNCTION__, e.what(), "stderr", true);
		}
		return data;
	}

	/*
	void terminal_stdout(map<string,string>& return_values, string command) {
		string data;
		FILE * stream;
		const int max_buffer = 1024;
		char buffer[max_buffer];
		command.append(" 2>&1");

		stream = popen(command.c_str(), "r");
		if (stream) {
			while (!feof(stream)) 
				if (fgets(buffer, max_buffer, stream) != NULL) 
					data.append(buffer);
			return_values.insert(make_pair("return", to_string(pclose(stream))));
		}
		return_values.insert(make_pair("data", data));
	}
	*/

	bool rename_file( string path_filename, string new_path_filename) {
		if(std::rename( path_filename.c_str(), new_path_filename.c_str()) == -1 ) {
			logger::logger_errno(errno, __LINE__, __FUNCTION__ );
			logger::logger(__FUNCTION__, "From: " + path_filename + "\nTo: " + new_path_filename, "stderr", true );
			return false;
		}
		
		return true;
	}

	string isp_exchange( string isp, string isp_rules ) {
		vector<string> rules = helpers::string_split( isp_rules, ':' );
	
		for( auto _isp : rules ) {
			vector<string> raw_isp = helpers::string_split( _isp, '{');
			/*
			if( raw_isp[1][0] != '{' or raw_isp[1][isp.size() -1] != '}') {
				logger::logger(__FUNCTION__, "Invalid ISP Exchange", "stderr");
				continue;
			}
			*/
			if( raw_isp[1][raw_isp[1].size() -1] != '}') {
				logger::logger(__FUNCTION__, "Invalid ISP Exchange", "stderr");
				continue;
			}
			// raw_isp[1].erase(0,1);
			raw_isp[1].erase(raw_isp[1].size() -1, 1);
			logger::logger(__FUNCTION__, "Parsed ISP: " + raw_isp[1]);

			vector<string> exchanges = helpers::string_split( raw_isp[1], ',');
			for( auto exchange : exchanges ) {
				if( isp.find( exchange ) != string::npos or exchange.find( isp ) != string::npos) {
					isp = raw_isp[0];
					return isp;
				}
			}
		}
		return isp;
	}
	
	vector<string> get_modem_details ( string path_to_script, string index, map<string,string> configs ) {
		vector<string> details;
		string type = index.find("192.168") != string::npos ? "ssh" : "mmcli";
		string modem_information = type == "ssh" ? sys_calls::terminal_stdout("ssh root@"+index+" -oPasswordAuthentication=no deku") : sys_calls::terminal_stdout( path_to_script + "/modem_information_extraction.sh extract " + index );
		vector<string> ln_modem_information = helpers::string_split(modem_information, '\n');

		logger::logger(__FUNCTION__, "Type: " + type + "\tSize: " + to_string( ln_modem_information.size() ));

		if( type == "ssh") {
			if(ln_modem_information.size() < 2) {
				//logger::logger(__FUNCTION__, "Incomplete data for modem at index: " + index, "stderr");
				return details;
			}
			
			if(ln_modem_information[0].find("deku:verified:") != string::npos) {
				details.push_back(index );// equipment_id
				details.push_back(helpers::to_uppercase(ln_modem_information[1]) ); // operator_name
				details.push_back(type );// type
			}
		}
		else if( type == "mmcli") {
			if( ln_modem_information.size() != 4 ) {
				// Not valid modem
				return details;
			}

			vector<string> split_equipment_id = helpers::string_split(ln_modem_information[0], ':');
			vector<string> split_operator_name = helpers::string_split(ln_modem_information[2], ':');
			vector<string> split_operator_id = helpers::string_split(ln_modem_information[3], ':');
			if(split_equipment_id.size() == 2 and (split_operator_name.size() == 2 || split_operator_id.size() == 2)) {
				details.push_back(helpers::string_split(ln_modem_information[0], ':')[1] );// equipment_id

				string isp, isp_id;
				if( split_operator_name.size() == 2 ){
					isp = helpers::string_split(ln_modem_information[2], ':')[1];// operator_name
				}
				if( split_operator_id.size() == 2 )
					isp_id = helpers::string_split(ln_modem_information[3], ':')[1];// operator_id
				if( isp.empty()) {
					if( isp_id.empty()) 
						return vector<string>{};
					else
						isp = isp_id;
				}

				/*
				if(isp.find("COVID") != string::npos or isp.find("62401") != string::npos) 
					isp = "MTN";
				else if(isp.find("62402") != string::npos)
					isp = "ORANGE";
				*/

				if( configs.find("ISP_EXCHANGE") != configs.end()) {
					isp = isp_exchange( isp, configs["ISP_EXCHANGE"]);
				}
				details.push_back( isp );
				details.push_back(type );// mmcli || ssh
			}
		}
		return details;
	}

	map<string,map<string,string>> get_available_modems( string path_to_script, map<string,string> configs ) {
		map<string,map<string,string>> available_modems;
		string list_of_modem_indexes = sys_calls::terminal_stdout( path_to_script + "/modem_information_extraction.sh list");
		vector<string> modem_indexes = helpers::string_split(list_of_modem_indexes, '\n');
		logger::logger(__FUNCTION__, "Listed " + to_string(modem_indexes.size()) + " index(es)");

		for(auto& index : modem_indexes ) {
			logger::logger(__FUNCTION__, "Working with index #" + index);
			index = helpers::remove_char( index, ' ');
			vector<string> details = get_modem_details( path_to_script, index, configs );
			if( details.size() != 3 ) {
				logger::logger(__FUNCTION__, "Not enough details for modem index: " + index, "stderr");
				continue;
			}
			map<string,string> in_details = {
				{"imei", details[0]},
				{"operator_name", details[1]},
				{"type", details[2]},
				{"index", index }
			};
			available_modems.insert(make_pair(details[0], in_details));
		}
		return available_modems;
	}
}
