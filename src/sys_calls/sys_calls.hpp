#ifndef SYS_CALLS_H_INCLUDED_
#define SYS_CALLS_H_INCLUDED_
#include <sys/stat.h> //mkdir
#include <fstream>
#include <typeinfo>
#include <unistd.h>
#include <sys/reboot.h>
#include <map>
#include <vector>

using namespace std;

namespace sys_calls {
	enum FILE_FLAG{EXIST, DEL};

	extern void sys_reboot();

	extern bool file_handlers(string, FILE_FLAG);

	extern void make_dir(string);

	extern string terminal_stdout(string);

	inline void terminal_stdout(map<string,string>& return_values, string command) {
		string data;
		FILE * stream;
		const int max_buffer = 1024;
		char buffer[max_buffer];
		command.append(" 2>&1");

		stream = popen(command.c_str(), "r");
		if (stream) {
			while (!feof(stream)) 
				if (fgets(buffer, max_buffer, stream) != NULL) 
					data.append(buffer);
			return_values.insert(make_pair("return", to_string(pclose(stream))));
		}
		if( data[data.size() -1] == '\n' or data[data.size() -1] == '\r' )
			data.erase(data.size() -1, 1);

		return_values.insert(make_pair("data", data));
	}

	extern bool rename_file(string,string);

	extern vector<string> get_modem_details(string,string,map<string,string>);

	extern map<string,map<string,string>> get_available_modems(string);
}

#endif
