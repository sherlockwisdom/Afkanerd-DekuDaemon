#include <iostream>

using namespace std;


int main() {
	while( 1) {
		auto pstring = (string*)0x7ffc282b43c0;
		cout << pstring << endl;
		cout << *pstring << endl;
		string tmp;
		getline(cin,tmp);
	}

	return 0;
}
