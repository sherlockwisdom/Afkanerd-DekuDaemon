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

	extern void sys_reboot();

	extern bool file_handlers(string, FILE_FLAG);

	extern void make_dir(string);

	extern string terminal_stdout(string);

	extern void terminal_stdout(map<string,string>,string);

	extern bool rename_file(string,string);

	extern vector<string> get_modem_details(string,string);

	extern map<string,map<string,string>> get_available_modems(string);
}

#endif
