#include <iostream>

using namespace std;


int main() {
	while( 1) {
		string mem_location = "memory define";
		cout << &mem_location << endl;
		getline(cin, mem_location);
	}

	return 0;
}
