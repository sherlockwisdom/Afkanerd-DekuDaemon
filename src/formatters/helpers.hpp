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
	inline string remove_char(string, char);

	inline string escape_string(string, char);

	template<class T> T vector_to_whole_string( vector<T>, char del =' ');

	template<class T> T vector_to_whole_numbers( vector<T>, char del = ' ');

	inline string find_and_replace_substr( string, string, string );

	inline vector<string> string_split(string, char del = ' ', size_t start_pos = 0);

	inline void make_dir( string );

	template <class T> inline void write_file( string path_filename, T input, bool b_unescape_string = false, ios_base::openmode mode = ios::app );

	inline vector<string> read_file( string );

	inline bool file_exist( string);

	inline void sleep_thread( int );

	inline string terminal_stdout(string);

	inline string random_string();

	inline string to_uppercase(string);

	inline string to_lowercase(string);

	inline auto one_line_parser( string );

	inline vector<string> comma_seperate( string input, size_t limit_to = 0, bool ignore_in_quotes = false );

	inline vector<string> equal_seperate( string input, size_t limit_to = 0);

	inline vector<string> general_seperate( string input, char delimeter );

	inline vector<string> in_delimeter_extract( string input, char delimeter );
}

#endif
