#include "helpers.hpp"
using namespace std;

namespace helpers {
	/** Removes all occurence of specified character from string */
	string remove_char( string input, char del ) {
		input.erase(std::remove(input.begin(), input.end(), del ), input.end());
		return input;
	}

	/** escapes special characters in a string */
	// TODO: This function doesn't make any sense in the first place
	string escape_string( string input, char del ) {
		for(size_t i=0;i<input.size();++i) {
			if( input[i] == del ) {
				input.erase(i, 1);
				string tmp_string = "";
				tmp_string += del;
				tmp_string = "\\\\" + tmp_string;
				input.insert(i, tmp_string);
				i = i + 3;
			}
		}
		return input;
	}

	/** Takes a vector<string> and converts to a std::string */
	template<class T>
	T vector_to_whole_string( vector<T> in_vector, char del = ' ') { // this should be a template of dynamic input
		T _return = "";
		if(std::is_same<T, std::string>::value) { //std::is_same_v -> C++17
			for(auto _string : in_vector ) {
				if(!_return.empty()) _return += del;
				_return += _string;
			}
		}
		return _return;
	}

	/** Takes a vector<int> and converts to a int */
	template<class T>
	T vector_to_whole_numbers( vector<T> in_vector, char del = ' ') {
		T _return = 0;
		if( std::is_same<T, int>::value) {
			string temp_hold = "";
			for(size_t i=0;i<in_vector.size();++i ) {
				// TODO: use del to add custom paddings to numbers
				temp_hold += to_string(in_vector[i]);
			}

			_return = atoi(temp_hold.c_str());
		}
		return _return;
	}

	string find_and_replace_substr( string str_find, string str_replace, string input ) {
		string backup_original = input;
		string return_string = "";
		size_t start_pos = 0;
		size_t find_pos = input.find(str_find);
		while(find_pos != string::npos) {
			return_string += input.substr(start_pos, find_pos);
			return_string += str_replace;
			input.erase(start_pos, (find_pos) + str_find.size() );
			find_pos = input.find(str_find);
		}
		if(!input.empty()) return_string += input;
		return return_string.empty() ? backup_original : return_string;
	}

	vector<string> string_split(string _string, char del = ' ', size_t start_pos = 0) { // TODO: Add start delimeter, test other sections of models which require this part of code
		// 
		vector<string> return_vector;
		size_t is_delimeter = _string.find( del );
		size_t pos = 0;
		while( is_delimeter != string::npos ) {
			if( pos < start_pos ) {
				++pos;
				continue;
			}
			string hold_to_del = _string.substr(0, is_delimeter);
			return_vector.push_back( hold_to_del );
			_string.erase(0, is_delimeter + 1);

			is_delimeter = _string.find( del );
			++pos;
		}
		return_vector.push_back( _string );
		return return_vector;
	}

	/** Creates a dir in Linux */
	void make_dir( string path_dirname ) {
		size_t start_pos = path_dirname[0] == '/' ? 1 : 0;
		vector<string> recursive_paths = helpers::string_split(path_dirname, '/', start_pos);
		string make_me = recursive_paths[0];
		for(size_t i=0;i<recursive_paths.size();++i) {
			logger::logger(__FUNCTION__, "Making dir: " + make_me, "stdout", false);
			if( i!=0) make_me += "/" + recursive_paths[i];
			if( mkdir( make_me.c_str(), 0777 ) == -1) {
				if( errno != 17 ) logger::logger_errno( errno );
			}
		}
		
		return;
	}

	/** Writes to a file at file path, just a wrapper for the iostream methods */
	template<class T>
	void write_file( string path_filename, T input, bool b_unescape_string = false, ios_base::openmode mode = ios::app ) {
		//if( b_unescape_string ) input = unescape_string( input );
		ofstream writefile( path_filename.c_str(), mode );
		writefile << input;
		writefile.close();
	}

	/** Reads file with filename at file_path, just another wrapper for the iostream methods */
	vector<string> read_file( string filename ) {
		ifstream readfile( filename.c_str() );
		vector<string> file_contents;
		if( !readfile.good() ) return file_contents;

		string tmp;
		while(getline(readfile, tmp)) file_contents.push_back( tmp );
		readfile.close();
		
		return file_contents;
	}

	/** Checks if file exist at path - linux system calls used*/
	bool file_exist( string path_filename ) {
		struct stat buffer; 
		return stat( path_filename.c_str(), &buffer) == 0;
	}

	/** Sleeps the thread for duration of time */
	void sleep_thread( int duration ) {
		std::this_thread::sleep_for(std::chrono::seconds( duration ));
	}

	/** Operates a Linux terminal command and stores the std/stderr outputs in a string */
	string terminal_stdout(string command) {
		string data;
		FILE * stream;
		const int max_buffer = 1024;
		char buffer[max_buffer];
		command.append(" 2>&1");

		stream = popen(command.c_str(), "r");
		if (stream) {
			while (!feof(stream)) if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
			pclose(stream);
		}
		return data;
	}

	/** Generates a random string */
	string random_string(){
	     string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	     random_device rd;
	     mt19937 generator(rd());

	     shuffle(str.begin(), str.end(), generator);

	     return str.substr(0, 32);    // assumes 32 < number of characters in str         
	}

	/** Transform a string to uppercase */
	string to_uppercase(string input) {
		string str = input;
		transform(str.begin(), str.end(),str.begin(), ::toupper);
		return str;
	}

	/** Transforms a string to lowercase */
	string to_lowercase(string input) {
		string str = input;
		transform(str.begin(), str.end(),str.begin(), ::tolower);
		return str;
	}

	/** Custom one line parser, returns a key->value pair, TODO: make this better - remove is better*/
	auto one_line_parser( string string_to_parse ) {
		string tmp_string_buffer = "";
		string tmp_key = "";
		map<string, string> request_tuple;
		bool ignore = false;
		bool safe = false;
		for(auto i : string_to_parse) {
			if(i == 'n' and safe and ignore) {
				tmp_string_buffer += '\n';
				safe = false;
				continue;
			}
			if(i == '=' and !ignore) {
				tmp_key = tmp_string_buffer;
				tmp_string_buffer = "";
				continue;
			}
			if(i == ',' and !ignore) {
				request_tuple.insert(make_pair(tmp_key, tmp_string_buffer));
				tmp_key = "";
				tmp_string_buffer = "";
				continue;
			}
			if(i == '"') {
				ignore = !ignore;
				continue;
			}
			if(i == '\\' and ignore) {
				safe = true;
				continue;
			}
			tmp_string_buffer += i;
		}

		if(!tmp_key.empty() and !tmp_string_buffer.empty()) {
			request_tuple.insert(make_pair(tmp_key, tmp_string_buffer));
		}

		return request_tuple;
	}
}

#endif
