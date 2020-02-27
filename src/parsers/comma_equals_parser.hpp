#include <vector>
#ifndef COMMA_EQUAL_PARSER_H_INCLUDED_
#define COMMA_EQUAL_PARSER_H_INCLUDED_

using namespace std;

namespace parsers {
	vector<string> comma_seperate( string input, size_t limit_to = 0, bool ignore_in_quotes = false ) {
		vector<string> seperated;
		size_t comma_pos = input.find(',');
		size_t found_counter = 0;
		size_t quotes_pos = input.find('"');
		size_t quotes_pos_1 = input.find('"', quotes_pos + 1);
		while(comma_pos != string::npos) {
			/*
			cout << "comma pos: " << comma_pos << endl;
			cout << "quotes pos: " << quotes_pos << endl;
			cout << "quotes pos 1: " << quotes_pos_1 << endl;
			cout << endl;
			*/
			if(ignore_in_quotes and (quotes_pos != string::npos and quotes_pos_1 != string::npos) and (comma_pos > quotes_pos and comma_pos < quotes_pos_1)) {
				//TODO: needs serious work - this is a hack
				comma_pos = input.find(',', quotes_pos_1+1);
				quotes_pos = input.find('"', quotes_pos_1+1);
				quotes_pos_1 = input.find('"', quotes_pos + 1);
				continue;
			}
			++found_counter;
			seperated.push_back( input.substr(0, comma_pos));
			input.erase(0, comma_pos+1);
			if(ignore_in_quotes) {
				quotes_pos = input.find('"');
				quotes_pos_1 = input.find('"', quotes_pos + 1);
			}
			comma_pos = input.find(',');

			if( limit_to != 0 and found_counter >= limit_to ) break;
		}
		seperated.push_back( input ); 
		return seperated;
	}

	vector<string> equal_seperate( string input, size_t limit_to = 0) {
		vector<string> seperated;
		size_t comma_pos = input.find('=');
		size_t found_counter = 0;
		while(comma_pos != string::npos) {
			++found_counter;
			seperated.push_back( input.substr(0, comma_pos));
			input.erase(0, comma_pos+1);
			comma_pos = input.find('=');

			if( limit_to != 0 and found_counter >= limit_to ) break;
		}
		seperated.push_back( input ); 
		return seperated;
	}

	vector<string> general_seperate( string input, char delimeter ) {
		vector<string> seperated;
		size_t comma_pos = input.find(delimeter);
		while(comma_pos != string::npos) {
			seperated.push_back( input.substr(0, comma_pos));
			input.erase(0, comma_pos+1);

			comma_pos = input.find(delimeter);
		}
		seperated.push_back( input ); 
		return seperated;
	}


	vector<string> in_delimeter_extract( string input, char delimeter ) {
		vector<string> extracted;
		size_t start_del_pos = input.find( delimeter);
		size_t end_del_pos = input.find( delimeter, start_del_pos + 1);
		while(start_del_pos != string::npos and end_del_pos != string::npos) {
			extracted.push_back(input.substr(start_del_pos +1, ((end_del_pos) - (start_del_pos+1))));
			input.erase(0, end_del_pos+1);

			start_del_pos = input.find(delimeter);
			end_del_pos = input.find(delimeter, start_del_pos + 1);
		}
		return extracted;
	}
}

#endif
