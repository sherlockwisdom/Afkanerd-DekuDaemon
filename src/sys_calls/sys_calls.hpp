#include <sys/stat.h> //mkdir
#ifndef SYS_CALLS_H_INCLUDED_
#define SYS_CALLS_H_INCLUDED_
using namespace std;

namespace sys_calls {
	enum FILE_FLAG{EXIST};
	bool file_listeners( string file_path ) {}

	template<class T>
	T file_handlers( string file_path, FILE_FLAG flag) {
		switch( flag ) {
			case EXIST:
				struct stat buffer; 
				return stat( file_path.c_str(), &buffer) == 0;
			break;
		}
	}

	int dir_handlers( string dir_path ) {}
}


#endif
