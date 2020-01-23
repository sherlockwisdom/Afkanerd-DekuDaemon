#include <iostream>
#include "comma_equals_parser.hpp"

using namespace std;
//TODO: Allow parser to choose parsing or ignoring double quotes

int main() {
	string sample = "number=00000,message=Hello world\"this is a sample message,retry=true";
	vector<string> comma_expected_sample = {"number=00000", "message=Hello world\"this is a sample message", "retry=true"};
	vector<string> equal_expected_sample = {"number", "00000,message", "Hello world\"this is a sample message,retry", "true"};
	vector<string> in_delimeter_expected_sample = {"message=Hello world\"this is a sample message"};

	vector<string> parser_sample = parsers::comma_seperate( sample );
	vector<string> parser_sample1 = parsers::equal_seperate( sample );
	vector<string> parser_sample2 = parsers::general_seperate( sample, ',');
	vector<string> parser_sample21 = parsers::general_seperate( sample, '=');
	vector<string> parser_sample3 = parsers::in_delimeter_extract( sample, ',');

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
	if( equal_expected_sample == parser_sample1 ) {
		cout << __FUNCTION__ << ": Passed" << endl;
	}
	else {
		cout << __FUNCTION__ << ": Failed" << endl;
		for(auto i : parser_sample1) cout << i << ", ";
		cout << endl;
	}

	cout << endl;
	cout << __FUNCTION__ << ":Testing: general_seperate" << endl;
	if( comma_expected_sample == parser_sample2 ) {
		cout << __FUNCTION__ << ": comma PASSED" <<endl;
	}
	else {
		cout << __FUNCTION__ << ": comma FAILED" << endl;
		for(auto i : parser_sample2 ) cout << i << ", ";
	}

	if( equal_expected_sample == parser_sample21 ) {
		cout << __FUNCTION__ << ": equal PASSED" << endl;
	}
	else {
		cout << __FUNCTION__ << ": equal FAILED" << endl;
		for(auto i : parser_sample21) cout << i << ", ";
	}

	cout << endl;
	cout << __FUNCTION__ << ":Testing: in_delimeter_seperate" << endl;
	if( in_delimeter_expected_sample == parser_sample3 ) {
		cout << __FUNCTION__ << ": PASSED" << endl;
	}
	else {
		cout << __FUNCTION__ << ": FAILED" << endl;
		for(auto i : parser_sample3 ) cout << i << ", ";
	}
	return 0;
}
