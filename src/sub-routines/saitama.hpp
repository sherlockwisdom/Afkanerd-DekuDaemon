#ifndef SAITAMA_H_INCLUDED_
#define SAITAMA_H_INCLUDED_

using namespace std;

namespace saitama {

	map<string,string> executions {
		{"--:all_might:--", " <-- Executing All Might --> "}
	};

	void execute( string command ) {
		// String find the last of the information which cannot change things, witout changing all the other files in the system
		string respond = executions[ command ];

		if( respond.empty()) {
			logger::logger(__FUNCTION__, "No internal command for ADMIN!", "stderr");
			return;
		}


		logger::logger(__FUNCTION__, "Executing Predefined function: " + respond );
	}
}

#endif
