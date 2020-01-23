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

	vector<string> equal_seperate( string input ) {}
}

#endif
