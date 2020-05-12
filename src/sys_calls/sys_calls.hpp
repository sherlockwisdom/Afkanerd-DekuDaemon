#ifndef SYS_CALLS_H_INCLUDED_
#define SYS_CALLS_H_INCLUDED_
#include <sys/stat.h> //mkdir
#include <fstream>
#include <typeinfo>
#include <unistd.h>
#include <sys/reboot.h>

#include "../formatters/helpers.hpp"
using namespace std;

namespace sys_calls {
	enum FILE_FLAG{EXIST, DEL};

	inline void sys_reboot();

	inline bool file_handlers(string, FILE_FLAG);

	inline void make_dir(string);

	inline string terminal_stdout(string);

	inline void terminal_stdout(map<string,string>,string);

	inline bool rename_file(string,string);

	inline vector<string> get_modem_details(string,string);

	inline map<string,map<string,string>> get_available_modems(string);
}

#endif
