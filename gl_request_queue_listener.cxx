/* THREAD LISTENING FOR INCOMING REQUEST */

#include "helpers.hpp"
#include "logger.hpp"
#include "declarations.hpp"

#include <map>
#include <vector>

map<string, string> gl_request_queue_listener( string path_request_file ) {
	string func_name = "gl_request_queue_listener";
	map<string, string> processed_request;

	/// Checks if file is available at path
	if( struct stat buffer; stat(path_request_file.c_str(), &buffer) == -1) {
		/// cout << func_name << "=> no request file, thus no request yet..." << endl;
		logger::logger(func_name, "no request file, thus no request yet\n", "stdout");
		logger::logger_errno( errno ) ;
		return processed_request;
	}

	string tmp_rand_filename = "tmp/" + helpers::random_string(); //TODO, move dir to global dir

	if( rename( path_request_file.c_str() , tmp_rand_filename.c_str() ) == -1) {
		logger::logger(func_name, "random request filename: " + tmp_rand_filename +"\n");
		logger::logger_errno( errno );
		return processed_request;
	}

	vector<string> request = helpers::read_file( tmp_rand_filename );
	logger::logger(func_name, to_string( request.size() ) + " requested\n", "stdout", true);

	for( auto s_request : request) 
		processed_request.insert(make_pair( tmp_rand_filename, s_request ) ); //XXX: Always 1 request per file
	
	return processed_request;
}

vector<map<string,string>> dequeue_from_request_file( string path_request_file ) { //TODO: make filename an arg
	vector<map<string,string>> request_tuple_container;
	for( auto tmp_ln_buffer : helpers::read_file( path_request_file )) {
		if(tmp_ln_buffer.empty() or tmp_ln_buffer[0] == '#') continue;

		map<string,string> request_tuple = helpers::one_line_parser( tmp_ln_buffer);
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
			logger::logger( func_name, "Could not determine ISP\n", "stderr" );
			isp_sorted_request_container["unknown"].push_back( request );
		}
	}

	return isp_sorted_request_container;
}


void isp_distribution(string isp, vector<map<string, string>> isp_request) {
	string func_name = "isp_distribution";
	helpers::make_dir( SYS_ISP_DISTRIBUTION + "/" + helpers::to_upper( isp ) );	
	for(auto request : isp_request ) {
		map<string, string> s_request = request;
		string filename = helpers::random_string();
		string message = request["message"];
		string number = request["number"];
		string c_request = number + "\n" + message;
		helpers::write_file( SYS_ISP_DISTRIBUTION + "/" + helpers::to_upper( isp ) + "/" + filename, c_request, ios::trunc);
	}
}

void daemon_function_for_threading() {
	string func_name = "daemon_function_for_threading";
	auto incoming_request = gl_request_queue_listener( SYS_REQUEST_FILE ); //map<string,string> filename, message
	for( auto request : incoming_request ) {
		string request_filename = request.first;
		//vector<map<string,string>> dequeued_request = dequeue_from_request_file( filename );
		map<string, vector<map<string,string>>> isp_for_request = determine_isp_for_request ( dequeue_from_request_file( request_filename ) );
		
		for( auto stats_request : isp_for_request ) {
			logger::logger( func_name, stats_request.first + " = " + to_string( stats_request.second.size() ) + "\n" );
			isp_distribution( stats_request.first, stats_request.second );
		}
	}
}
