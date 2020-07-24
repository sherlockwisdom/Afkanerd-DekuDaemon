#ifndef HELPER_H_INCLUDED_
#define HELPER_H_INCLUDED_
#include <iostream>
#include <sys/stat.h> //mkdir
#include <errno.h>
#include <stdio.h>
#include <string.h>

using namespace std;

namespace logger {

	extern string show_state;

	extern void logger( const char* func_name, string output, string output_stream = "stdout", bool show_production = false);
	
	extern void logger_errno( int t_errno, int line_num = -1 );

	extern void logger_tester( string func_name, bool output, string output_stream = "stdout" );
}


#endif
