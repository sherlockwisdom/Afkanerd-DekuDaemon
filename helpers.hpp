#ifndef HELPERS_H_INCLUDED_
#define HELPERS_H_INCLUDED_
#include <algorithm>
#include <random>
#include "declarations.hpp"
using namespace std;

namespace helpers {
	string unescape_string( string input ) {
		for(size_t i=0;i<input.size();++i) {
			if( input[i] == '\n' ) {
				input.erase(i, 1);
				input.insert(i, "\\n");
			}
		}
		return input;
	}


	void logger( string func_name, string output, string output_stream = "stdout", bool show_production = false) {
		if( (output.empty() || CURRENT_SYSTEM_STATE == "production" || CURRENT_SYSTEM_STATE == "PRODUCTION") and !show_production) return;

		if( output_stream == "stdout" || output_stream == "STDOUT" ) {
			cout << "[logger.info] - " << func_name << "=> " << output;
		}
		else if( output_stream == "stderr" || output_stream == "STDERR" ) {
			cerr << "[logger.error] - " << func_name << "=> " << output;
		}

		else cerr << "[logger.error] - LOGGER DOESN'T HAVE THAT STATE YET" << endl;
	}


	void logger_errno( int t_errno ) {
		char str_error[256];
		string error_message = strerror_r( t_errno, str_error, 256);
		cout << "[logger_errno] - ERRNO: " << t_errno << endl;
		cout << "[logger_errno] - MESSAGE: " << error_message << "=> " << endl;
	}

	void write_file( string path_filename, auto input, ios_base::openmode mode = ios::app, bool unescape_string = false ) { //TODO: what about auto
		if( unescape_string ) input = unescape_string( input );
		ofstream writefile( path_filename.c_str(), mode );
		writefile << input;
		writefile.close();
	}

	vector<string> read_file( string filename ) {
		// TODO: add variable to read into string not vector, change return type to auto when this happens
		ifstream readfile( filename.c_str() );
		vector<string> file_contents;
		if( !readfile.good() ) return file_contents;

		string tmp;
		while(getline(readfile, tmp)) file_contents.push_back( tmp );
		readfile.close();
		
		return file_contents;
	}

	string terminal_stdout(string command) {
		string data;
		FILE * stream;
		const int max_buffer = 1024;
		char buffer[max_buffer];
		command.append(" 2>&1");

		stream = popen(command.c_str(), "r");
		if (stream) {
			while (!feof(stream)) if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
			pclose(stream);
		}
		return data;
	}

	vector<string> split(string _string, char del = ' ', bool strict = false) {
		vector<string> return_value;
		string temp_string = "";
		for(auto _char : _string) {
			if(_char==del) {
				if(strict and temp_string.empty()) continue;
				return_value.push_back(temp_string);
				temp_string="";
			}
			else {
				temp_string+=_char;
			}
		}
		if(strict and !temp_string.empty()) return_value.push_back(temp_string);

		return return_value;
	}


	string ISPFinder(string number) {
		if(number[0] == '6') {
			switch(number[1]) {
				case '5':
					switch(number[2]) {
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
							return "MTN";
						break;

						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							return "ORANGE";
						break;

					}
				break;

				case '7': return "MTN";
				break;

				case '8':
					  switch(number[2]) {
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
							return "MTN";
						break;

						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							return "NEXTEL";
						break;
					  }
				break;

				case '9': return "ORANGE";
				break;
			}
		}
		return "";
							
	}

	string random_string(){
	     string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	     random_device rd;
	     mt19937 generator(rd());

	     shuffle(str.begin(), str.end(), generator);

	     return str.substr(0, 32);    // assumes 32 < number of characters in str         
	}

	string to_upper(string input) {
		string str = input;
		transform(str.begin(), str.end(),str.begin(), ::toupper);
		return str;
	}


	//Customized just to work for those needing tools to continue working on deku
	bool modem_is_available(string modem_imei) {
		string list_of_modem_indexes = helpers::terminal_stdout("./modem_information_extraction.sh list");
		vector<string> modem_indexes = helpers::split(list_of_modem_indexes, '\n', true);

		for(auto modem_index : modem_indexes) {
			if(modem_index == modem_imei && modem_imei.find("192.168") != string::npos) return true;
			string modem_information = helpers::terminal_stdout((string)("./modem_information_extraction.sh extract " + modem_index));
			vector<string> imei_info = helpers::split(modem_information, ':', true);
			if(imei_info[0] == "equipment_id") {
				if(imei_info[1].find(modem_imei) != string::npos) return true;
			}	
		}
		return false;
	}

	//TODO: Make work load checking functional
	int read_log_calculate_work_load(string modem_path) {
		string func_name = "Read Log Calculate Workload";
		//cout << func_name << "=> started calculating work load" << endl;
		ifstream modem_log_read(modem_path.c_str());
		//XXX: Assumption is the file is good if it's passed in here
		string tmp_buffer;
		int total_count = 0;
		while(getline(modem_log_read, tmp_buffer)) {
			//XXX: timestamp:count
			string timestamp = helpers::split(tmp_buffer, ':', true)[0];
			string count = helpers::split(tmp_buffer, ':', true)[1];
			total_count += atoi(count.c_str());
		}
		modem_log_read.close();
		//cout << func_name << "=> calculating work load ended..." << endl;
		return total_count;
	}



	string remove_carriage( string input, char location = 'B' ) {
		//TODO: Make location determine where it takes the input file from
		size_t space_location = input.find('\n');
		while( space_location != string::npos and space_location == 0) {
			input.erase( 0, 1);
			space_location = input.find('\n');
		}

		return input;
	}

	void write_to_request_file( string message, string number ) {
		string func_name = "write_to_request_file";
		if( message.empty() or number.empty() ) return;
		ofstream write_to_request_file(SYS_REQUEST_FILE, ios::app);
		printf("%s=> message[%s] : number [%s]\n", func_name.c_str(), message.c_str(), number.c_str());
		write_to_request_file << "number=" << number << ",message=\"" << unescape_string( remove_carriage( message )) << "\"" << endl;
		write_to_request_file.close();	
	}


	void write_modem_job_file( string modem_imei, string message, string number ) {
		if( message.empty() or number.empty() ) return;
		string func_name = "write_modem_job_file";
		printf("%s=> \tJob for modem with info: IMEI: %s\n", func_name.c_str(), modem_imei.c_str());

		string rand_filename = helpers::random_string();
		rand_filename = rand_filename.erase(rand_filename.size() -1, 1);
		rand_filename += ".jb";

		printf("%s=> \tCreating job with filename - %s\n", func_name.c_str(), rand_filename.c_str());
		ofstream job_write((char*)(SYS_FOLDER_MODEMS + "/" + modem_imei + "/" + rand_filename).c_str());
		job_write << number << "\n" << remove_carriage( message );
		job_write.close();
	}
}

#endif
