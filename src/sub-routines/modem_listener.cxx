#include "modem_listener.hpp"
#include "../formatters/helpers.hpp"

using namespace std;

Modems::Modems() {}

Modems Modems::getAll() {
	string list_of_modem_indexes = helpers::terminal_stdout("../../scripts/modem_information_extraction.sh list");
}
vector<string> Modems::getAllIndexes() {

}

vector<string> Modems::getAllISP() {}

vector<string> Modems::getAllIMEI() {}
