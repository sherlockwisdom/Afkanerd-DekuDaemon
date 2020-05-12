#ifndef HELPERS_H_INCLUDED_
#define HELPERS_H_INCLUDED_
#include <algorithm>
#include <random>
#include <fstream>
#include <thread>
#include <type_traits>
#include <map>
#include "logger.hpp"
using namespace std;

namespace helpers {
	string remove_char(string, char);

	string escape_string(string, char);

	template<class T> T vector_to_whole_string( vector<T>, char del =' ');

	template<class T> T vector_to_whole_numbers( vector<T>, char del = ' ');

	string find_and_replace_substr( string, string, string );

	vector<string> string_split(string, char del = ' ', size_t start_pos = 0);

	void make_dir( string );

	template <class T> void write_file( string path_filename, T input, bool b_unescape_string = false, ios_base::openmode mode = ios::app );

	vector<string> read_file( string );

	bool file_exist( string);

	void sleep_thread( int );

	string terminal_stdout(string);

	string random_string();

	string to_uppercase(string);

	string to_lowercase(string);

	auto one_line_parser( string );

	vector<string> comma_seperate( string input, size_t limit_to = 0, bool ignore_in_quotes = false );

	vector<string> equal_seperate( string input, size_t limit_to = 0);

	vector<string> general_seperate( string input, char delimeter );

	vector<string> in_delimeter_extract( string input, char delimeter );
}

#endif
