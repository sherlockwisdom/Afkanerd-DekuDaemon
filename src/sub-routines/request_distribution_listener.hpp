#ifndef REQUEST_DISTRIBUTION_LISTENER_H_INCLUDED_
#define REQUEST_DISTRIBUTION_LISTENER_H_INCLUDED_
#include <map>
#include <algorithm>
#include "../formatters/helpers.hpp"
#include "../parsers/comma_equals_parser.hpp"
#include "../sub-routines/isp_determiner.hpp"
#include "../sys_calls/sys_calls.hpp"


namespace request_distribution_listener {
	bool configs_check( map<string,string> );
	string isp_distributor(string,string,string,map<string,string>);
	map<string,string> request_parser(string);
	void request_distribution_listener(map<string,string>);
}


#endif
