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
	extern string remove_char(string, char);

	extern string escape_string(string, char);

	template<class T> T vector_to_whole_string( vector<T>, char del =' ');

	template<class T> T vector_to_whole_numbers( vector<T>, char del = ' ');

	extern string find_and_replace_substr( string, string, string );

	extern vector<string> string_split(string, char del = ' ', size_t start_pos = 0);

	extern void make_dir( string );

	static void write_file( string path_filename, auto input, bool b_unescape_string = false, ios_base::openmode mode = ios::app );

	extern vector<string> read_file( string );

	extern bool file_exist( string);

	extern void sleep_thread( int );

	extern string terminal_stdout(string);

	extern string random_string();

	extern string to_uppercase(string);

	extern string to_lowercase(string);

	extern auto one_line_parser( string );

	extern vector<string> comma_seperate( string input, size_t limit_to = 0, bool ignore_in_quotes = false );

	extern vector<string> equal_seperate( string input, size_t limit_to = 0);

	extern vector<string> general_seperate( string input, char delimeter );

	extern vector<string> in_delimeter_extract( string input, char delimeter );
}

#endif
