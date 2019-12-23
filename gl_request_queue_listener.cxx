/* THREAD LISTENING FOR INCOMING REQUEST */

#include "declarations.hpp"


auto parser( string string_to_parse ) {
	string tmp_string_buffer = "";
	string tmp_key = "";
	map<string, string> request_tuple;
	bool ignore = false;
	bool safe = false;
	for(auto i : string_to_parse) {
		//XXX: checks for seperator
		if(i == 'n' and safe and ignore) {
			tmp_string_buffer += '\n';
			safe = false;
			continue;
		}
		if(i == '=' and !ignore) {
			tmp_key = tmp_string_buffer;
			tmp_string_buffer = "";
			continue;
		}
		if(i == ',' and !ignore) {
			request_tuple.insert(make_pair(tmp_key, tmp_string_buffer));
			tmp_key = "";
			tmp_string_buffer = "";
			continue;
		}
		if(i == '"') {
			ignore = !ignore;
			continue;
		}
		if(i == '\\' and ignore) {
			safe = true;
			continue;
		}
		tmp_string_buffer += i;
	}

	if(!tmp_key.empty() and !tmp_string_buffer.empty()) {
		request_tuple.insert(make_pair(tmp_key, tmp_string_buffer));
	}

	return request_tuple;
}

vector<map<string,string>> de_queue_from_request_file( ) { //TODO: make filename an arg

	string tmp_ln_buffer;
	ifstream sys_request_file_read(SYS_JOB_FILE.c_str());

	//XXX: Container contains maps which have keys as number and message
	vector<map<string,string>> request_tuple_container;
	while(getline(sys_request_file_read, tmp_ln_buffer)) {
		if(tmp_ln_buffer.empty() or tmp_ln_buffer[0] == '#') continue;

		map<string,string> request_tuple = parser( tmp_ln_buffer);
		if( !request_tuple.empty()) 
			request_tuple_container.push_back(request_tuple);
	}
	sys_request_file_read.close();
	return request_tuple_container;
}


auto determine_isp_for_request(vector<map<string,string>> request_tuple_container) {
	string func_name = "determine_isp_for_request" ;
	map<string,vector<map<string,string>>> isp_sorted_request_container; //ISP=>container of messages
	for(int i=0;i<request_tuple_container.size();++i) {
		map<string, string> request = request_tuple_container[i];
		string number= request["number"];
		string isp = helpers::ISPFinder(number);
		if(!isp.empty()) {
			isp_sorted_request_container[isp].push_back(request);
		}
		else {
			cout << func_name << "=> Could not determine ISP... writing back to request file" << endl;
			string message = helpers::escape_string( helpers::remove_carriage( request["message"] ) );
			string number = request["number"];
			helpers::write_to_request_file( message, number );
		}
	}

	return isp_sorted_request_container;
}



void curl_server( string TCP_HOST, string TCP_PORT, string URL, string message) {
	string func_name = "curl_server";
	string command = "curl -X POST -H \"Content-Type: text/plain\" " + TCP_HOST + ":" + TCP_PORT + "/" + URL + " -d \"" + message + "\"";
	cout << func_name << "=> command [" << command << "]" << endl;
	string terminal_output = helpers::terminal_stdout( command );
}

void isp_distribution(string func_name, string isp, vector<map<string, string>> isp_request) {
	if(MODEM_DAEMON.empty()) {
		cout << func_name << "=> No modem found, writing back to request file..." << endl;
		for(auto request_container : isp_request) {
			string message = helpers::escape_string( request_container["message"] );
			string number = request_container["number"];
			helpers::write_to_request_file( message, number );
		}
		return;
	}

	//TODO: determine all modems for this ISP then send out the messages, will help with even distribution
	map<string,string> isp_modems;
	cout << func_name << "=> checking for modems for this ISP" << endl;
	for( auto modem : MODEM_DAEMON ) {
		if( helpers::to_upper(modem.second).find( helpers::to_upper( isp )) != string::npos ) {
			isp_modems.insert( modem ); //FIXME: I doubt this
		}
	}

	cout << func_name << "=> number of modems for ISP| {" << isp_modems.size() << "}" << endl;

	//Send this information to online socket server
	string message = func_name + "=> number of modems for ISP| {" + to_string( isp_modems.size() ) + "}\n";
	std::thread message_curl_server(curl_server, GL_TCP_HOST, GL_TCP_PORT, GL_TCP_URL, message);
	message_curl_server.detach();

	if( isp_modems.size() < 1 ) {
		cout << func_name << "=> No modem found for ISP, writing back to request file and going to sleep " <<endl;
		//std::this_thread::sleep_for(std::chrono::seconds(GL_TR_SLEEP_TIME));
		std::this_thread::sleep_for(std::chrono::seconds(10));

		for(auto request_container : isp_request) {
			string message = helpers::escape_string( request_container["message"] );
			string number = request_container["number"];
			helpers::write_to_request_file( message, number );
		}
		return;
	}

	size_t request_index = 0;
	for( map<string,string>::iterator i = isp_modems.begin();i != isp_modems.end();++i ) {
		begin:
		string modem_imei = i->first;
		string modem_isp = i->second;

		if( request_index >= isp_request.size() ) break;
		cout << func_name << "=> request index at: " << request_index << endl;

		if(!helpers::modem_is_available(modem_imei)) {
			printf("%s=> Not available modem: ISP for +imei[%s] +ISP[%s]\n", func_name.c_str(), modem_imei.c_str(), modem_isp.c_str());
			continue;
		}

		map<string, string> request = isp_request[request_index];
		++request_index;

		string message = request["message"];
		string number = request["number"];
		
		helpers::write_modem_job_file( modem_imei, message, number );
		if( ++i; i == isp_modems.end() ) {
			i = isp_modems.begin();
			goto begin;
		}
		else --i;
	}
}

/// Listens to the request file for incoming queues
/// Input = filename, output = <stats for processed request>
map<string, string[2]> gl_request_queue_listener( string path_request_file ) {
	string func_name = "gl_request_queue_listener";
	map<string, string[2]> processed_request;

	/// Checks if file is available at path
	if( struct stat buffer; stat(path_request_file.c_str(), &buffer) == -1) {
		/// cout << func_name << "=> no request file, thus no request yet..." << endl;
		helpers::logger_errno( errno ) ;
		helpers::logger(func_name, "no request file, thus no request yet\n", "stdout");
		return processed_request;
	}

	string tmp_rand_filename = "/tmp/" + helpers::random_string();

	if( rename( path_request_file.c_str() , tmp_rand_filename.c_str() ) == -1) {
		helpers::logger(func_name, "random request filename: " + tmp_rand_filename +"\n");
		helpers::logger_errno( errno );
	}


	//goto statement here because sometimes shit has to continue from where it stopped
	/*
	DEQUEUE_JOBS: 
	vector<map<string,string>> request_tuple_container = de_queue_from_request_file();
	cout << func_name << "=> Job file contains: " << request_tuple_container.size() << " request..." << endl;

	//File is done reading so we can remove it
	remove(SYS_JOB_FILE.c_str());
	
	//Determine the ISP from here
	map<string, vector<map<string, string>>> isp_sorted_request_container = determine_isp_for_request(request_tuple_container);
	
	for(auto i : isp_sorted_request_container) {
		printf("%s=> For ISP[%s]----\n", func_name.c_str(), i.first.c_str());

		//TODO: Thread this!! No need sitting and waiting for one ISP before using the other
		std::thread tr_isp_distribution(isp_distribution, "ISP Distribution", i.first, i.second);
		tr_isp_distribution.detach();
	}	
	std::this_thread::sleep_for(std::chrono::seconds(GL_TR_SLEEP_TIME));
	*/
}
