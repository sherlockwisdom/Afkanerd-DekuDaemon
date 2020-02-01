#include <sys/stat.h> //mkdir
#include <fstream>
#include <typeinfo>
#include "../formatters/helpers.hpp"
#ifndef SYS_CALLS_H_INCLUDED_
#define SYS_CALLS_H_INCLUDED_
using namespace std;

namespace sys_calls {
	enum FILE_FLAG{EXIST, READ, DEL};
	bool file_listeners( string file_path ) {}

	bool file_handlers( string file_path, FILE_FLAG flag) {
		switch( flag ) {
			case EXIST:{
				struct stat buffer; 
				return stat( file_path.c_str(), &buffer) == 0;
			}
			break;

			case DEL:
			break;
		}
	}

	void make_dir( string path_dirname ) {
		size_t start_pos = path_dirname[0] == '/' ? 1 : 0;
		vector<string> recursive_paths = helpers::split(path_dirname, '/', true, start_pos);
		string make_me = recursive_paths[0];
		for(int i=0;i<recursive_paths.size();++i) {
			logger::logger(__FUNCTION__, "Making dir: " + make_me, "stdout", false);
			if( i!=0) make_me += "/" + recursive_paths[i];
			if( mkdir( make_me.c_str(), 0777 ) == -1) {
				if( errno != 17 ) logger::logger_errno( errno );
			}
		}
		
		return;
	}

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


	bool delete_file( string path_filename) {}

	bool rename_file( string path_filename, string new_path_filename) {}
	
	bool unhide_file( string path_filename) {}

	bool hide_file( string path_filename) {
		
	}

	int dir_handlers( string dir_path ) {}
}


#endif
