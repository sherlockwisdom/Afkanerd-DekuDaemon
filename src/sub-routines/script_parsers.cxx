#include "../formatter/helpers.hpp"

using namespace std;


// keywords = class, EXECUTE

namespace script_parser {
	void parse( vector<string> script_lines ) {
		string setting_state;
		string rule_program;
		string rule_programs_args;
		
		vector<string> state_rules;

		map<string,map<string,string>> interpreted_rules;
		map<string,map<string,string>> interpreted_states;
		for(auto line : script_lines ) {
			if(line[0] == '[' and line[1] == ']') {
				line[0].erase(0,1);
				line[line.size()-1].erase(line.size()-1, 1);

				vector<string> tokens = helpers::string_split( line, ' ' );
				if( tokens.size() < 2 ) return;

				if( tokens[0] == "class" ) {
					vector<string> tokens1 = helpers::string_split( tokens[1] );
					if( tokens1[0] == "ISP" ) {
						setting_state = tokens1[1];
					}
				}
				else if( tokens[0] == "EXECUTE" ) {
					rules = tokens[1];
				}
			}
			else if( line.empty() or line == "\n") {
				if( !setting_state.empty() and !state_rules.empty()) {
					interpreted_states.insert(make_pair(setting_state, state_rules));
					setting_state = "";
					state_rules.clear();
				}
				
				if( !state_rules.empty() and !rule_program.empty() and !rule_program_args.empty()) {
					interpreted_rules.insert(make_pair(rule, execute_rules));
				}
			}
			else {
				vector<string> state_rules_split = helpers::string_split( line[0], '=');
				if( state_rules_split[0] == "execute") {
					state_rules.push_back(state_rules_split[1]);
				}
				else if( state_rules_split[0] == "program" ) { 
					rule_program = state_rules_split[1];
				}
				else if( state_rules_split[0] == "args") {
					rule_program_args = state_rules_split[1];
					if( !rule_program.empty()) {
						program_rules.insert(make_pair(rule_program, rule_program_args));
						rule_program = "";
						rule_programs_args = "";
					}
				}
			}
		}
	}
}
