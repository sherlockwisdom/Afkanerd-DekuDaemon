#include "modem_listener.hpp"
#include "../formatters/helpers.hpp"
#include "../sys_calls/sys_calls.hpp"
#include <mutex>

using namespace std;

//class Modems
Modems::Modems( map<string,string> configs, STATE state ) {
	this->state = state;
	switch( state ) {
		case TEST:
			logger::show_state = "TESTING";
		break;

		case PRODUCTION:
			logger::show_state = "PRODUCTION";
		break;
	}

	this->configs = configs;

	// Connect to MySQL Server
	this->mysqlConnection.setConnectionDetails( configs["MYSQL_SERVER"], configs["MYSQL_USER"], configs["MYSQL_PASSWORD"], configs["MYSQL_DATABASE"]);

	// TODO: Add method to set mysql function here
	if( !this->mysqlConnection.connect()) {
		logger::logger(__FUNCTION__, " - Failed connecting to MYSQL database!", "stderr", true);
		exit( 1 );
	}

	logger::logger(__FUNCTION__, " - MYSQL Connection Established!", "stdout", true);
}

void Modems::set_modem_sleep_time( int sleep_time ) {
	this->modem_sleep_time = sleep_time;
}

void Modems::set_exhaust_count( int modem_exhaust_count ) {
	this->modem_exhaust_count = modem_exhaust_count;
}

void Modems::db_insert_modems_workload( map<string, string> modem ) {
	string select_workload_query = "SELECT * FROM __DEKU__.MODEM_WORK_LOAD WHERE IMEI='" + modem["imei"] + "' and DATE = DATE(NOW())";
	logger::logger(__FUNCTION__, "Checking for modem in DB workload");

	map<string, vector<string>> query_respond = this->mysqlConnection.query( select_workload_query );
	if(query_respond.empty()) {
		logger::logger(__FUNCTION__, "Modem not in workload - Executing Insert queries");
		string replace_workload_query = "REPLACE INTO __DEKU__.MODEM_WORK_LOAD (IMEI, DATE) VALUES (\'" + modem["imei"] + "\', NOW())";
		this->mysqlConnection.query( replace_workload_query );

		return;
	}
}

void Modems::db_insert_modems( map<string,string> modem ) {
	string insert_modem_query = "INSERT INTO __DEKU__.MODEMS (IMEI, TYPE, STATE, POWER) VALUES(\'"
	+ modem["imei"]
	+ "','" 
	+ helpers::to_lowercase( modem["type"] ) 
	+ "','active'," +
	+ "'plugged')";

	logger::logger(__FUNCTION__, "Inserting modem into DB");
	// Insert affects rows, but doesn't return anything
	this->mysqlConnection.query( insert_modem_query );
}

void Modems::begin_scanning() {
	// TODO: set exhaust count as default in class declarations
	// TODO: set sleep time as default in class declaractions
	// TODO: set modem state PRODUCTION as default in class declarations

	// TODO: If problem with IMEI in modem, change modem to un_plugged

	while( 1 ) { //TODO: Use a variable to control this loop
		// First it gets all availabe modems
		logger::logger(__FUNCTION__, "Refreshing modem list..");
		map<string,map<string,string>> available_modems = sys_calls::get_available_modems( this->configs["DIR_SCRIPTS"] );
		logger::logger(__FUNCTION__, "Number of Available modems: " + to_string( available_modems.size() ));

		// Second it filters the modems and stores them in database
		for(auto modem : available_modems) {
			bool has_modems_imei = this->available_modems.find( modem.first ) != this->available_modems.end() ? true : false;
			if( !has_modems_imei ) {
				logger::logger(__FUNCTION__, " ====> NEW MODEM DETECTED <======");
				logger::logger(__FUNCTION__, "IMEI: " + modem.first );
				map<string,string> details = modem.second;
				logger::logger(__FUNCTION__, "TYPE: " + details["type"] );
				logger::logger(__FUNCTION__, "ISP: " + details["operator_name"] );
				logger::logger(__FUNCTION__, "==================================");
				

				// Thid stores modem in list of modems
				string imei = modem.first;
				string isp = helpers::to_upper(details["operator_name"] );
				string type = helpers::to_upper(details["type"] );
				string index = details["index"];


				//TMP solution to some ISP crisis
				if(isp.find("COVID") != string::npos) 
					isp = "MTN";
				else if(isp.find("6") != string::npos) 
					isp = "ORANGE";
				this->available_modems.insert(make_pair( modem.first, new Modem(imei, isp, type, index, this->configs, this->mysqlConnection)));

				// Forth Starts the modems and let is be free
				std::thread tr_modem = std::thread(&Modem::start, std::ref(this->available_modems[modem.first]));
				tr_modem.detach();

				// Optional Fith, tries storing the modems in a sql database
				try {
					this->db_insert_modems( details );
					this->db_insert_modems_workload( details );
				}
				catch(std::exception& e) {
					logger::logger(__FUNCTION__, e.what(), "stderr" );
				}
			}
		}

		for(auto it_modem = this->available_modems.begin(); it_modem != this->available_modems.end(); ++it_modem ) {
			auto modem = *it_modem;
			logger::logger(__FUNCTION__, modem.second->getInfo() + " Checking availability");
			if( !modem.second->is_available() ) {
				logger::logger(__FUNCTION__, modem.second->getInfo() + " | Delisting Modem...");

				delete modem.second;
				this->available_modems.erase(modem.first );
				if(this->available_modems.empty()) break;
			}
		}
		logger::logger(__FUNCTION__, "Number of Available modems (After Delisting): " + to_string( available_modems.size() ));

		// Seventh: Rinse and repeat
		helpers::sleep_thread( 10 );
	}
}

vector<string> Modems::getAllIndexes() {
	vector<string> list;
	for(auto modem : this->modemCollection ) {
		list.push_back( modem->getIndex() );
	}
	return list;
}

vector<string> Modems::getAllISP() {
	vector<string> list;
	for(auto modem : this->modemCollection) {
		list.push_back( modem->getISP() );
	}
	return list;
}

vector<string> Modems::getAllIMEI() {
	vector<string> list;
	for(auto modem: this->modemCollection) {
		list.push_back( modem->getIMEI() );
	}
	return list;
}

vector<Modem*> Modems::getAllModems() {
	return this->modemCollection;
}
