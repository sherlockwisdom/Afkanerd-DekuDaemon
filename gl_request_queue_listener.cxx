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






void curl_server( string TCP_HOST, string TCP_PORT, string URL, string message) {
	string func_name = "curl_server";
	string command = "curl -X POST -H \"Content-Type: text/plain\" " + TCP_HOST + ":" + TCP_PORT + "/" + URL + " -d \"" + message + "\"";
	cout << func_name << "=> command [" << command << "]" << endl;
	string terminal_output = helpers::terminal_stdout( command );
}

void isp_distribution(string isp, vector<map<string, string>> isp_request) {
	string func_name = "isp_distribution";
	if(MODEM_DAEMON.empty()) {
		cout << func_name << "=> No modem found, writing back to request file..." << endl;
		for(auto request_container : isp_request) {
			string message = helpers::unescape_string( request_container["message"] );
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
			string message = helpers::unescape_string( request_container["message"] );
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
map<string, string> gl_request_queue_listener( string path_request_file ) {
	string func_name = "gl_request_queue_listener";
	map<string, string> processed_request;

	/// Checks if file is available at path
	if( struct stat buffer; stat(path_request_file.c_str(), &buffer) == -1) {
		/// cout << func_name << "=> no request file, thus no request yet..." << endl;
		helpers::logger_errno( errno ) ;
		helpers::logger(func_name, "no request file, thus no request yet\n", "stdout");
		return processed_request;
	}

	string tmp_rand_filename = "tmp/" + helpers::random_string(); //TODO, move dir to global dir

	if( rename( path_request_file.c_str() , tmp_rand_filename.c_str() ) == -1) {
		helpers::logger(func_name, "random request filename: " + tmp_rand_filename +"\n");
		helpers::logger_errno( errno );
		return processed_request;
	}

	vector<string> request = helpers::read_file( tmp_rand_filename );
	helpers::logger(func_name, to_string( request.size() ) + " requested\n", "stdout", true);

	processed_request.insert(make_pair( tmp_rand_filename, request[0] ) ); //XXX: Always 1 request per file
	
	return processed_request;
}

vector<map<string,string>> dequeue_from_request_file( string path_request_file ) { //TODO: make filename an arg
	vector<map<string,string>> request_tuple_container;
	for( auto tmp_ln_buffer : helpers::read_file( path_request_file )) {
		if(tmp_ln_buffer.empty() or tmp_ln_buffer[0] == '#') continue;

		map<string,string> request_tuple = parser( tmp_ln_buffer);
		if( !request_tuple.empty()) 
			request_tuple_container.push_back(request_tuple);
	}
	return request_tuple_container;
}

map<string, vector<map<string,string>>> determine_isp_for_request(vector<map<string,string>> request_tuple_container) {
	string func_name = "determine_isp_for_request" ;
	map<string,vector<map<string,string>>> isp_sorted_request_container; //ISP=>container of messages
	for(int i=0;i<request_tuple_container.size();++i) {
		map<string, string> request = request_tuple_container[i];
		string number= request["number"];
		string isp = helpers::ISPFinder(number); //TODO: Make ISP finder a config file, not code - parser pending
		if(!isp.empty()) {
			isp_sorted_request_container[isp].push_back(request);
		}
		else {
			helpers::logger( func_name, "Could not determine ISP\n", "stderr" );
			string message = request["message"];
			string number = request["number"];
			isp_sorted_request_container["unknown"].push_back( request );
		}
	}

	return isp_sorted_request_container;
}
