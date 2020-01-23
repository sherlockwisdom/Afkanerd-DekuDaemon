#include <iostream>
#include "comma_equals_parser.hpp"

using namespace std;
//TODO: Allow parser to choose parsing or ignoring double quotes

int main() {
	string sample = "number=00000,message=Hello world\"this is a sample message,retry=true";
	vector<string> comma_expected_sample = {"number=00000", "message=Hello world\"this is a sample message", "retry=true"};
	vector<string> parser_sample = parsers::comma_seperate( sample );

	cout << __FUNCTION__ << ":Testing: comma_seperate" << endl;
	if( comma_expected_sample == parser_sample ) {
		cout << __FUNCTION__  << ": Passed" << endl;
	}
	else {
		cout << __FUNCTION__ << ": Failed" << endl;
		for(auto i : parser_sample ) cout << i << ", ";
		cout << endl;
	}
	cout << endl;
	cout << __FUNCTION__ << ":Testing: equal_seperate" << endl;


	return 0;
}
