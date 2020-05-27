#include "modem_listener.hpp"

using namespace std;

void Modems::handle_sigint( int signal ) {
	logger::logger(__FUNCTION__, "ENDING, CLEANING UP", "stdout", true);

	size_t iterator = 0;
	for(auto it_av_modem = Modems::available_modems.begin(); it_av_modem != Modems::available_modems.end(); ++it_av_modem, ++iterator) {
		logger::logger(__FUNCTION__, "CLEANSING: [" + to_string(iterator + 1) + "/" + to_string(Modems::available_modems.size()) + "]| " + it_av_modem->second->getInfo(), "stdout", true);
		delete it_av_modem->second;
	}
	// TODO: Release locked files
	// Should be save doing that here cus the modems have all been stopped above
	exit(1);
}

//class Modems
Modems::Modems( map<string,string> configs, STATE state ) {
	signal(SIGINT, handle_sigint);
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

vector<Modem*> Modems::find_modem( string modem_index ) {
	auto modems = this->get_available_modems();
	vector<Modem*> available_modems;

	for(auto _modem : modems ) {
		map<string,string> details = _modem.second;
		if( details["index"] == modem_index ) {
			available_modems.push_back( new Modem( details["imei"], details["isp"], details["type"], details["index"], this->configs));
			break;
		}
	}

	return available_modems;
}

vector<Modem*> Modems::find_modem_type( string modem_isp ) {
	auto modems = this->get_available_modems();
	vector<Modem*> available_modems;

	for(auto _modem : modems ) {
		map<string,string> details = _modem.second;
		if( details["isp"] != modem_isp ) continue;
		available_modems.push_back( new Modem( details["imei"], details["isp"], details["type"], details["index"], this->configs));
	}

	return available_modems;
}

void Modems::set_modem_sleep_time( int sleep_time ) {
	this->modem_sleep_time = sleep_time;
}

void Modems::set_exhaust_count( int modem_exhaust_count ) {
	this->modem_exhaust_count = modem_exhaust_count;
}

void Modems::db_insert_modems_workload( map<string, string> modem ) {
	string select_workload_query = "SELECT * FROM __DEKU__.MODEM_WORK_LOAD WHERE IMEI='" + modem["imei"] + "' and DATE = DATE(NOW())";
	// logger::logger(__FUNCTION__, "Checking for modem in DB workload");

	bool responds = this->mysqlConnection.query( select_workload_query );
	if( !responds ) {
		logger::logger(__FUNCTION__, "INSERT MODEM INTO DB FAILED", "stderr", true);
		return;
	}

	map<string, vector<string>> query_respond = this->mysqlConnection.get_results();
	if(query_respond.empty()) {
		// logger::logger(__FUNCTION__, "Modem not in workload - Executing Insert queries");
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

void Modems::db_switch_power_modems( map<string,string> modem, string state ) {
	string switch_modem_power_query = "UPDATE __DEKU__.MODEMS SET POWER = '"+state+"' WHERE IMEI='" + modem["imei"] + "'";
	logger::logger(__FUNCTION__, modem["imei"] + " - Switch modem power state");
	this->mysqlConnection.query( switch_modem_power_query );
}

map<string,map<string,string>> Modems::get_available_modems() {
	map<string,map<string,string>> available_modems = sys_calls::get_available_modems( this->configs["DIR_SCRIPTS"] );
		logger::logger(__FUNCTION__, "Number of Available modems: " + to_string( available_modems.size() ));

		return available_modems;
}

map<string, string> Modems::get_modem_details( map<string, string> modem ) {
	string imei = modem["imei"];
	string isp = helpers::to_uppercase(modem["operator_name"] );
	string type = helpers::to_uppercase(modem["type"] );
	string index = modem["index"];
	
	// TODO: Abstract this to a better solution
	// TODO: This is very important to work on
	if(isp.find("COVID") != string::npos or isp.find("62401") != string::npos) 
		isp = "MTN";
	else if(isp.find("62402") != string::npos)
		isp = "ORANGE";
	if( isp.empty() ) {
		logger::logger(__FUNCTION__, imei + "|" + index + " - No ISP", "stderr", true);
		return map<string,string>{};
	}

	map<string,string> modem_details {
		{"imei" , imei },
		{"isp" , isp },
		{"type" , type },
		{"index" , index } 
	};

	return modem_details;
}

void Modems::begin_scanning( bool request_listening = true, bool sms_listening = false, bool remote_control = false) {
	while( 1 ) { //TODO: Use a variable to control this loop
		// First it gets all availabe modems
		logger::logger(__FUNCTION__, "Refreshing modem list..");
		auto av_modems = this->get_available_modems();
		// auto av_modems = Modems::available_modems;

		// Second it filters the modems and stores them in database
		for(auto modem : av_modems) {
			bool has_modems_imei = false;
			if(!Modems::available_modems.empty()) 
				has_modems_imei = Modems::available_modems.find( modem.first ) != Modems::available_modems.end() ? true : false;
			if( !has_modems_imei ) {
				map<string,string> modem_details = this->get_modem_details( modem.second );
				if( modem_details.empty() ) continue;
				
				string imei = modem_details["imei"];
				string isp = modem_details["isp"];
				string type = modem_details["type"];
				string index = modem_details["index"];

				// Thid stores modem in list of modems

				try {
					Modems::available_modems.insert(make_pair( modem.first, new Modem(imei, isp, type, index, this->configs, this->mysqlConnection)));
				}
				catch( std::exception& e) {
					logger::logger(__FUNCTION__, e.what(), "stderr", true);
				}

				// Forth Starts the modems and let is be free
				logger::logger(__FUNCTION__, Modems::available_modems[modem.first]->getInfo() + " Starting...");
				Modems::available_modems[modem.first]->set_logger_show_state( logger::show_state );
				auto active_modem = Modems::available_modems[modem.first];
				if( request_listening ) {
					std::thread tr_modem = std::thread(&Modem::start, std::ref(active_modem));
					tr_modem.detach();
					//
					// Optional Fith, tries storing the modems in a sql database
					try {
						this->db_insert_modems( modem_details );
						this->db_insert_modems_workload( modem_details );
					}
					catch(std::exception& e) {
						logger::logger(__FUNCTION__, e.what(), "stderr" );
					}
				}

				// Check if sms is required here
				if( sms_listening ) {
					logger::logger(__FUNCTION__, "SMS LISTENER SET TO START");
					std::thread tr_modem_sms_listener = std::thread(&Modem::modem_sms_listener, std::ref(active_modem), remote_control);
					tr_modem_sms_listener.detach();
				}

			}
			else {
				logger::logger(__FUNCTION__, Modems::available_modems[modem.first]->getInfo() + " - Already present in system");
				this->db_switch_power_modems( modem.second, "plugged" );
			}
		}

		for(auto it_modem = Modems::available_modems.begin(); it_modem != Modems::available_modems.end(); ++it_modem ) {
			auto modem = *it_modem;
			logger::logger(__FUNCTION__, modem.second->getInfo() + " Checking availability");
			if( !modem.second->is_available() ) {
				logger::logger(__FUNCTION__, modem.second->getInfo() + " | Delisting Modem...");

				delete modem.second;
				Modems::available_modems.erase(modem.first );
				this->db_switch_power_modems( map<string,string>{{"imei", modem.second->getIMEI()}}, "not_plugged" );
				if(Modems::available_modems.empty()) break;
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

