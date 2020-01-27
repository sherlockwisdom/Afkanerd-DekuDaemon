#include "formatters/helpers.hpp"
using namespace std;


int main() {
	cout << boolalpha << helpers::file_exist("formatters/") << endl;
	cout << boolalpha << helpers::file_exist("UTest.cxx") << endl;
	cout << boolalpha << helpers::file_exist("UTest_na.cxx") << endl;
	return 0;
}
