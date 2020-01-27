#include <iostream>
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
	for(auto number : mtn_numbers) {
		if( isp_determiner::get_isp( number) != "Orange" ) {
			cout << "FAILED" << endl;
			cout << number << " is not an Orange number" << endl;
		}
	}
	cout << __FUNCTION__ << ": Testing Nexttell numbers: " << endl;
	for(auto number : mtn_numbers) {
		if( isp_determiner::get_isp( number) != "Nexttell" ) {
			cout << "FAILED" << endl;
			cout << number << " is not an Nexttell number" << endl;
		}
	}

	return 0;
}

