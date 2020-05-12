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

	void sys_reboot();

	bool file_handlers(string, FILE_FLAG);

	void make_dir(string);

	string terminal_stdout(string);

	void terminal_stdout(map<string,string>,string);

	bool rename_file(string,string);

	vector<string> get_modem_details(string,string);

	map<string,map<string,string>> get_available_modems(string);
}

#endif
