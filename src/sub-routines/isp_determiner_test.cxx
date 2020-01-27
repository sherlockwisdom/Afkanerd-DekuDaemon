#include <iostream>
#include "../formatters/helpers.hpp"
#include <vector>
#include "isp_determiner.hpp"

using namespace std;

int main() {
	vector<string> mtn_numbers = {"652000000", "671000000", "673000000", "680000000", "672000000"};
	vector<string> orange_numbers = {"691000000", "699000000", "6580000000", "6940000000", "698000000"};
	vector<string> nextel_numbers = {"667000000"};


	cout << __FUNCTION__ << ": Testing MTN numbers: " << endl;
	for(auto number : mtn_numbers) {
		if( isp_determiner::get_isp( number) != "MTN" ) {
			cout << "FAILED" << endl;
			cout << number << " is not an MTN number" << endl;
		}
	}
	cout << __FUNCTION__ << ": Testing Orange Numbers: " << endl;
	for(auto number : orange_numbers) {
		if( isp_determiner::get_isp( number) != helpers::to_upper("Orange") ) {
			cout << "FAILED" << endl;
			cout << number << " is not an Orange number" << endl;
		}
	}
	cout << __FUNCTION__ << ": Testing Nexttell numbers: " << endl;
	for(auto number : nextel_numbers) {
		if( isp_determiner::get_isp( number) != helpers::to_upper("Nexttell") ) {
			cout << "FAILED" << endl;
			cout << number << " is not an Nexttell number" << endl;
		}
	}

	cout << __FUNCTION__ << ": Testing Unknown numbers: " << endl;
	for(auto number : vector<string>{"123456789", "0000000000"}) {
		if( isp_determiner::get_isp( number) != helpers::to_upper("unknown") ) {
			cout << "FAILED" << endl;
			cout << number << " is not an Unknown number" << endl;
		}
	}

	return 0;
}

