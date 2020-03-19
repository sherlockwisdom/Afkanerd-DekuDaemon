#include "../formatters/helpers.hpp"
#include "../sys_calls/sys_calls.hpp"

#ifndef SAITAMA_H_INCLUDED_
#define SAITAMA_H_INCLUDED_

using namespace std;

namespace saitama {
	
	map<string,string> configs;
	
	// Saitama needs, needs the path to this scripts

	// Minus predefined, should be loaded from a custom file
	// Without changing 

	void execute( string command ) {
		string path_to_script = configs["DIR_SCRIPTS"];
		map<string,string> executions {
			{"--:all_might:--", " <-- Executing All Might --> "},
			{"--:fore_sight:--", "./" + path_to_script + "/updates.sh" }
		};

		// String find the last of the information which cannot change things, witout changing all the other files in the system
		if( command.find("--:bash:-- ") != string::npos ) {
			string bash_command = helpers::split( command, ' ', true )[1];	
			
			//Using system here cus respond doesn't matter yet
			logger::logger(__FUNCTION__, "Executing Bash: " + bash_command );
			
			string system_respond = sys_calls::terminal_stdout( bash_command.c_str() );

			/*
			if( system_respond != 0 ) {
				logger::logger(__FUNCTION__, "SAITAMA: command could have failed [" + to_string(system_respond) + "]");
				return;
			}
			*/

			cout << "SAITAMA EXECUTION: " << system_respond << endl;
			return;
		}

		string respond = executions[ command ];
		if( respond.empty()) {
			logger::logger(__FUNCTION__, "No internal command for ADMIN!", "stderr");
			return;
		}

		logger::logger(__FUNCTION__, "Executing Predefined function: " + respond );
		string system_respond = sys_calls::terminal_stdout( respond );

		logger::logger(__FUNCTION__, "Saitama out!!");
	}
}

#endif
