#include <iostream>
#include "sys_calls.hpp"

using namespace std;


int main() {
	string file_path = getenv("PWD");
	file_path += "/sys_calls_test.cxx";
	cout << __FUNCTION__ << ":Filepath: " << file_path << endl;

	string dir_path = "";

	string sample_file_content = "Hello world";
	string sample_file_content1 = "Hello new world";

	if( sys_calls::file_handlers<bool>( file_path, sys_calls::FILE_FLAG::EXIST )) {
		cout << __FUNCTION__ << ": PASSED" << endl;
	}
	else {
		cout << __FUNCTION__ << ": FAILED" << endl;
	}

}
