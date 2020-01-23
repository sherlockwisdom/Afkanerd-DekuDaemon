#include <vector>
#ifndef COMMA_EQUAL_PARSER_H_INCLUDED_
#define COMMA_EQUAL_PARSER_H_INCLUDED_

using namespace std;

namespace parsers {
	vector<string> comma_seperate( string input ) {
		vector<string> seperated;
		size_t comma_pos = input.find(',');
		while(comma_pos != string::npos) {
			seperated.push_back( input.substr(0, comma_pos));
			input.erase(0, comma_pos+1);

			comma_pos = input.find(',');
		}
		seperated.push_back( input ); 
		return seperated;
	}

	vector<string> equal_seperate( string input ) {
		vector<string> seperated;
		size_t comma_pos = input.find('=');
		while(comma_pos != string::npos) {
			seperated.push_back( input.substr(0, comma_pos));
			input.erase(0, comma_pos+1);

			comma_pos = input.find('=');
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
