#include "helpers.hpp"
#include "declarations.hpp"

bool ssh_send(string,string,string);
void write_for_urgent_transmission(string,string,string);

using namespace std;
/*

void modem_cleanse( string imei ) {
	map<string,string>::iterator it_modem_daemon = MODEM_DAEMON.find(imei);
	if(it_modem_daemon != MODEM_DAEMON.end()) 
		MODEM_DAEMON.erase( it_modem_daemon );
}


vector<string> get_modems_jobs(string folder_name) {
	return helpers::split( helpers::terminal_stdout((string)("ls -1 " + folder_name)), '\n', true );
}

map<string, string> read_request_file( string full_filename, string modem_imei) {
	string func_name = "read_request_file";
	printf("%s=> EXECUTING JOB FOR FILE: %s\n", func_name.c_str(), full_filename.c_str());
	ifstream read_job(full_filename.c_str());
	if(!read_job) {
		cerr << func_name << "=> error reading job: " << full_filename << endl;
		return (map<string,string>){};
	}

	string tmp_buffer, number, message = "";
	short int line_counter = 0;

	//Why is this doing this here??????
	//I see... some messages have \n characters in them... this part is to make sure they maintain that integrity
	while(getline(read_job, tmp_buffer)) {
		if(line_counter == 0) number = tmp_buffer;
		else if(line_counter > 0) {
			if( line_counter == 1) message = tmp_buffer;
			else message += "\n" + tmp_buffer;
		}
		++line_counter;
	}
	read_job.close();

	map<string,string> message_tuple = {{"message", message}, {"number", number}};
	if( message.empty() or number.empty()) return map<string,string>{};
	
	return message_tuple;
}



bool mmcli_send( string message, string number, string modem_index ) {
	string func_name = "mmcli_send";
	string sms_command = "./modem_information_extraction.sh sms send \"" + helpers::remove_carriage( message ) + "\" " + number + " " + modem_index;
	string terminal_stdout = helpers::terminal_stdout(sms_command);
	cout << func_name << "=> sending sms message...\n" << func_name << "=> \t\tStatus " << terminal_stdout << endl << endl;
	if(terminal_stdout.find("success") == string::npos and terminal_stdout.find("Success") == string::npos) {
		if(terminal_stdout.find("timed out") != string::npos) {
			fprintf(stderr, "%s=> Modem needs to sleep... going down for %d seconds\n", func_name.c_str(), GL_MMCLI_MODEM_SLEEP_TIME);
			std::this_thread::sleep_for(std::chrono::seconds(GL_MMCLI_MODEM_SLEEP_TIME));
			return true;
		}
		cerr << func_name << "=> MMCLI send failed" << endl;
		return false;
	}

	cout << func_name << "=> MMCLI send success" << endl;
	return true;
}

void update_modem_success_count( string modem_imei ) {
	//TODO: increment success count for this modem
	if( GL_SUCCESS_MODEM_LIST.find( modem_imei ) == GL_SUCCESS_MODEM_LIST.end() ) {
		GL_SUCCESS_MODEM_LIST.insert( make_pair( modem_imei, 0 ) );
	}

	GL_SUCCESS_MODEM_LIST[modem_imei] += 1;
}

bool ssh_send( string message, string number, string modem_ip ) {
	//TODO: Figure out how to make SSH tell if SMS has gone out or failed
	string func_name = "ssh_send";
	string sms_command = "ssh root@" + modem_ip + " -T -o \"ConnectTimeout=20\" \"sendsms '" + number + "' \\\"" + helpers::remove_carriage( message ) + "\\\"\"";
	string terminal_stdout = helpers::terminal_stdout(sms_command);
	cout << func_name << "=> sending sms message...\n" << func_name << "=> \t\tStatus " << terminal_stdout << endl << endl;

	if(terminal_stdout.find("failed") != string::npos ) {
		cerr << func_name << "=> ssh failed to send message! Doing emergency re-routing..." << endl;
		write_for_urgent_transmission( modem_ip, message, number );
	}

	return true; //FIXME: This is propaganda
}

void write_for_urgent_transmission( string modem_imei, string message, string number ) {
	//XXX: which modem has been the most successful
	string func_name = "write_for_urgent_transmission";
	if( !GL_SUCCESS_MODEM_LIST.empty() ) {
		string most_successful_modem;
		auto it_GL_SUCCESS_MODEM_LIST = GL_SUCCESS_MODEM_LIST.begin();
		int most_successful_modem_count = it_GL_SUCCESS_MODEM_LIST->second;
		string isp = MODEM_DAEMON[modem_imei];
		++it_GL_SUCCESS_MODEM_LIST;

		//FIXME: Something's wrong with this iterator
		for( auto it_GL_SUCCESS_MODEM_LIST : GL_SUCCESS_MODEM_LIST ) {
			if( it_GL_SUCCESS_MODEM_LIST.first != modem_imei and it_GL_SUCCESS_MODEM_LIST.second >= most_successful_modem_count and helpers::to_upper(MODEM_DAEMON[it_GL_SUCCESS_MODEM_LIST.first]).find( helpers::to_upper(isp) ) != string::npos ) {
				most_successful_modem_count = it_GL_SUCCESS_MODEM_LIST.second;
				most_successful_modem = it_GL_SUCCESS_MODEM_LIST.first;
			}
		}
		printf("%s=> Most successful modem | %s | count | %d\n", func_name.c_str(), most_successful_modem.c_str(), most_successful_modem_count);
		if( most_successful_modem.empty() ) {
			//FIXME: Should check for another modem rather than send things back to the request file
			fprintf( stderr, "%s=> No modem found for emergency transmission... writing back to request file\n", func_name.c_str() );
			helpers::write_to_request_file( message, number );
			return;
		}

		string modem_index;
		bool ssh_modem = is_ssh_modem( most_successful_modem );
		if( !ssh_modem ) {
			for(auto modem_details : MODEM_POOL) {
				if( modem_details.second[0] == most_successful_modem ) {
					modem_index = modem_details.first;
					break;
				}
			}
		}
		else {
			modem_index = most_successful_modem;
		}

		//FIXME: This solution is not checking for SSH modems
		if( modem_index.empty()) {
			//FIXME: Should check for another modem rather than send things back to the request file
			helpers::write_to_request_file( message, number );
		}
		else {
			helpers::write_modem_job_file( modem_index, message, number );
		}
	}
	else {
		helpers::write_to_request_file( message, number );
	}
}


void write_modem_details( string modem_imei, string isp ) {
	string func_name = "write_modem_details";
	printf("%s=> Writing modem details to modem [%s] file...\n", func_name.c_str(), modem_imei.c_str());
	ofstream write_modem_file( SYS_FOLDER_MODEMS + "/" + modem_imei + "/.details.dek", ios::trunc);
	write_modem_file << isp << endl;
	write_modem_file.close();
}

void modem_listener(string func_name, string modem_imei, string modem_index, string ISP, bool watch_dog = true, string type = "MMCLI") {
	//XXX: Just 1 instance should be running for every modem_imei
	printf("%s=> Started instance of modem=> \n+imei[%s] +index[%s] +isp[%s] +type[%s]\n\n", func_name.c_str(), modem_imei.c_str(), modem_index.c_str(), ISP.c_str(), type.c_str());

	//XXX: Log details of this instance
	write_modem_details( modem_imei, ISP );

	MODEM_DAEMON.insert(make_pair(modem_imei, ISP));
	while(GL_MODEM_LISTENER_STATE) {
		if(watch_dog) {
			if(!helpers::modem_is_available( modem_imei ) ) {
				printf("%s=> Killed instance of modem because disconncted\n\t+imei[%s] +index[%s] +isp[%s] +type[%s]\n", func_name.c_str(), modem_imei.c_str(), modem_index.c_str(), ISP.c_str(), type.c_str());
				modem_cleanse( modem_imei );
				break;
			}
		}
		//read the modems folder for changes
		vector<string> jobs = get_modems_jobs((string)(SYS_FOLDER_MODEMS + "/" + modem_imei));
		printf("%s=> [%lu] found jobs...\n", func_name.c_str(), jobs.size());
		for(auto filename : jobs) {
			string full_filename = SYS_FOLDER_MODEMS + "/" + modem_imei + "/" + filename;
			map<string,string> request_file_content = read_request_file( full_filename, modem_imei );

			if( request_file_content.empty() ) {
				fprintf(stderr, "%s=> Nothing returned from file....", func_name.c_str() );
				continue;
			}
			if( request_file_content["message"].empty() ) {
				fprintf( stderr, "%s=> Found bad file --- no message--- deleting....", func_name.c_str());
				if( remove(full_filename.c_str()) != 0 ) {
					cerr << func_name << "=> failed to delete job!!!!!" << endl;
					char str_error[256];
					cerr << func_name << "=> errno message: " << strerror_r(errno, str_error, 256) << endl;
				}
				continue;
			}
			if( request_file_content["number"].empty() ) {
				fprintf( stderr, "%s=> Found bad file --- no number--- deleting....", func_name.c_str());
				if( remove(full_filename.c_str()) != 0 ) {
					cerr << func_name << "=> failed to delete job!!!!!" << endl;
					char str_error[256];
					cerr << func_name << "=> errno message: " << strerror_r(errno, str_error, 256) << endl;
				}
				continue;
			}

			string message = request_file_content["message"];
			string number = request_file_content["number"];

			//printf("%s=> processing job: number[%s], message[%s]\n", func_name.c_str(), number.c_str(), message.c_str());
			if(type == "MMCLI") {
				mmcli_send( message, number, modem_index ) ? update_modem_success_count( modem_imei ) : write_for_urgent_transmission( modem_imei, message, number );
			}

			else if(type == "SSH") {
				ssh_send( message, number, modem_index ) ? update_modem_success_count( modem_imei ) : write_for_urgent_transmission( modem_imei, message, number );
			}

			//XXX: Test if it fails to delete this file
			if( remove(full_filename.c_str()) != 0 ) {
				cerr << func_name << "=> failed to delete job!!!!!" << endl;
				char str_error[256];
				cerr << func_name << "=> errno message: " << strerror_r(errno, str_error, 256) << endl;
			}
		}

		std::this_thread::sleep_for(std::chrono::seconds(GL_TR_SLEEP_TIME));
	}

	return;
}


void ssh_extractor( string ip_gateway ) {
	string func_name = "configure_ssh_modems";

	//verify ssh is actually a modem
	string ssh_stdout = helpers::terminal_stdout((string)("ssh root@" + ip_gateway + " -T -o \"ConnectTimeout=10\" deku"));

	//cout << func_name << "=> ssh output: " << ssh_stdout << endl;
	vector<string> ssh_stdout_lines = helpers::split(ssh_stdout, '\n', true);
	if(ssh_stdout_lines[0] == "deku:verified:") {
		cout << func_name << "=> SSH server ready for SMS!" << endl;
		if(mkdir((char*)(SYS_FOLDER_MODEMS + "/" + ip_gateway).c_str(), STD_DIR_MODE) != 0 && errno != EEXIST) {
			char str_error[256];
			cerr << func_name << ".error=> " << strerror_r(errno, str_error, 256) << endl;
		}
		else {
			if(MODEM_DAEMON.find(ip_gateway) != MODEM_DAEMON.end()) {
				cout << func_name << "=> instance of SSH already running... watch dog reset!" << endl;
				std::this_thread::sleep_for(std::chrono::seconds(GL_TR_SLEEP_TIME));
				return;
			}

			std::thread tr_ssh_listener(modem_listener, "\tSSH Listener", ip_gateway, ip_gateway, ssh_stdout_lines[1], true, "SSH");
			tr_ssh_listener.detach();
		}
	}
	else {
		cout << func_name << "=> Could not verify SSH server!" << endl;
	}

	return;
}

vector<string> extract_modem_details ( string modem_imei ) {
	string func_name = "extracat_modem_details";

	ifstream read_modem_file ( SYS_FOLDER_MODEMS + "/" + modem_imei + "/.details.dek");
	vector<string> file_details;
	
	if( !read_modem_file.good()) {
		cerr << func_name << "=> No detail file for modem " << modem_imei << endl;
	}

	else {
		string tmp_string;
		while( getline( read_modem_file, tmp_string ) ) file_details.push_back( tmp_string );
	}
	
	read_modem_file.close();
	
	return file_details;
}
*/

map<string,string> modem_extractor(string func_name, string modem_index ) {
	string str_stdout = helpers::GET_MODEM_INFO();
	string modem_service_provider = "";

	vector<string> modem_information = helpers::split(str_stdout, '\n', true);
	string modem_imei = helpers::split(modem_information[0], ':', true)[1];
	string modem_sig_quality = helpers::split(modem_information[1], ':', true)[1];

	map<string, string> modem_info = {
		{ "imei", modem_imei },
		{ "signal_quality", modem_sig_quality }
	}

	if(modem_information.size() != 3) {
		std::this_thread::sleep_for(std::chrono::seconds(GL_TR_SLEEP_TIME));
		//printf("%s=> modem information extracted - incomplete [%lu]\n", func_name.c_str(), modem_information.size());
		logger::logger(func_name, "modem information not available for extraction", "stderr", true);
		// Check if modem has .detail file

		if( helpers::file_exist( SYS_FOLDER_MODEMS + "/" + modem_imei + "/.details.txt" ) ) {
			logger::logger( func_name, "extracting from details file..." );
			if( vector<string> details_content = helpers::read_file( SYS_FOLDER_MODEMS + "/.details.txt"); !details_content.empty() ) {
				modem_info.insert(make_pair("isp", details_content[0]));
			}
			else {
				logger::logger(func_name, "empty detail file...", "stderr", true) ;
			}
		}
		else {
			logger::logger( func_name, "No detail file, manually create if needed", "stdout", true);
			return modem_info;
		}
	}
	
	if(!sanitation_check) return;
	//printf("%s=> modem information... [%s]\n", func_name.c_str(), modem_information[2].c_str());
	//XXX: Check if another an instance of the modem is already running
	if(MODEM_DAEMON.find(modem_imei) != MODEM_DAEMON.end()) {
		cout << func_name << "=> Instance of Modem already running... watch dog reset!" << endl;
		std::this_thread::sleep_for(std::chrono::seconds(GL_TR_SLEEP_TIME));
		return;
	}
	if(modem_service_provider.empty()) 
		modem_service_provider = helpers::split(modem_information[2], ':', true)[1]; //FIXME: What happens when cannot get ISP

	printf("%s=> +ISP[%s] +index[%s] - ", func_name.c_str(), modem_service_provider.c_str(), modem_index.c_str());
	if(mkdir((char*)(SYS_FOLDER_MODEMS + "/" + modem_imei).c_str(), STD_DIR_MODE) != 0 && errno != EEXIST) {
		char str_error[256];
		cerr << "FAILED\n" << func_name << ".error=> " << strerror_r(errno, str_error, 256) << endl;
	}
	else {
		std::thread tr_modem_listener(modem_listener, "\tModem Listener", modem_imei, modem_index, modem_service_provider, true, "MMCLI");
		tr_modem_listener.detach();
	}
}

bool is_ssh_modem( string ip ) {
	return ip.find( GL_SSH_IP_GATEWAY ) != string::npos;
}

string modem_information_extraction( string arg ) {
	string func_name = "modem_information_extraction";
	string ex_command = MODEM_INFORMATION_EXTRACTION_SCRIPT[ SYSTEM_STATE ];
	ex_command = ex_command + " " + arg;
	logger::logger(func_name, ex_command + "\n");

	return ex_command;
}

string get_modem_type( string modem_index ) {
	return is_ssh_modem( modem_index ) ? "ssh" : "mmcli";
}

vector<map<string,string>> gl_modem_listener( ) {
	string func_name = "gl_modem_listener";
	vector<map<string,string>> list_of_modems;

	string str_stdout = helpers::terminal_stdout( modem_information_extraction( "list" ));
	logger::logger( func_name, str_stdout + "\n" );

	if(str_stdout.empty()) {
		logger::logger(func_name, "No modems found!", "stderr" );
		return list_of_modems;
	}
	else {
		vector<string> modem_indexes = helpers::split( str_stdout, '\n', true);
		for( auto& modem_index : modem_indexes ) {
			modem_index = helpers::remove_char( modem_index, ' ', 'E');
			map<string,string> modem_information = {
				{"index", modem_index},
				{"type", get_modem_type( modem_index )}
			};
			
			list_of_modems.push_back( modem_information );
		}
	}

	return list_of_modems;
}

/*
void daemon_start_modem_listener() {
	vector<map<string,string>> list_of_modems = gl_modem_listener();
	
}
*/
