#include "modem_listener.hpp"
#include "../formatters/helpers.hpp"
#include "../sys_calls/sys_calls.hpp"
#include <mutex>

using namespace std;

//class Modems
Modems::Modems( STATE state) {
	this->state = state;
	switch( state ) {
		case TEST:
			logger::show_state = "TESTING";
		break;

		case PRODUCTION:
			logger::show_state = "PRODUCTION";
		break;
	}
}

Modems::Modems() {
	logger::show_state = "TESTING";
}

void Modems::set_modem_sleep_time( int sleep_time ) {
	this->modem_sleep_time = sleep_time;
}

void Modems::set_exhaust_count( int modem_exhaust_count ) {
	this->modem_exhaust_count = modem_exhaust_count;
}

void Modems::__INIT__( map<string, string> configs ) {
	this->mysqlConnection.setConnectionDetails( configs["MYSQL_SERVER"], configs["MYSQL_USER"], configs["MYSQL_PASSWORD"], configs["MYSQL_DATABASE"]);
	if( !this->mysqlConnection.connect()) {
		logger::logger(__FUNCTION__, " - Failed connecting to MYSQL database!", "stderr", true);
		exit( 1 );
	}

	logger::logger(__FUNCTION__, " - MYSQL Connection Established!", "stdout", true);

	while( 1 ) { //TODO: Use a variable to control this loop
		logger::logger(__FUNCTION__, "Refreshing modem list..");
		string list_of_modem_indexes = sys_calls::terminal_stdout(configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh list");
		vector<string> modem_indexes = helpers::split(list_of_modem_indexes, '\n', true);
		logger::logger(__FUNCTION__, "Number of indexes: " + to_string(modem_indexes.size()));
		logger::logger(__FUNCTION__, "Number of collected modems: " + to_string(this->modemCollection.size()));
		for(auto& index : modem_indexes )	
			index = helpers::remove_char( index, ' ', 'E');

		vector<Modem*> tmp_modemCollection;
		for(auto index : modem_indexes) {
			Modem::STATE modem_state = Modem::TEST;
			if(this->state == TEST) modem_state = Modem::TEST; 
			else if(this->state == PRODUCTION) modem_state = Modem::PRODUCTION;
			
			Modem modem( configs, modem_state );
			modem.setIndex( index );
			modem.set_sleep_time( this->modem_sleep_time );
			modem.set_exhaust_count( this->modem_exhaust_count );

			logger::logger(__FUNCTION__, "Setting Exhaust count: " + to_string(this->modem_exhaust_count));
			string modem_information = modem.getType() == Modem::SSH ? sys_calls::terminal_stdout("ssh root@"+index+" -o 'ServerAliveInterval 10' deku") : sys_calls::terminal_stdout(configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh extract " + index );
			vector<string> ln_modem_information = helpers::split(modem_information, '\n', true);

			if(modem.getType() == Modem::SSH) {
				if(ln_modem_information.size() < 2) {
					logger::logger(__FUNCTION__, "Incomplete data for modem at index: " + index, "stderr");
				}
				else if(ln_modem_information[0].find("deku:verified:") != string::npos) {
					modem.setISP(helpers::to_upper(ln_modem_information[1]));
					modem.setIMEI(index);
				}
			}
			else if(modem.getType() == Modem::MMCLI) {
				for(auto ln : ln_modem_information) {
					//logger::logger(__FUNCTION__, "line: " + ln);
					vector<string> component = helpers::split(ln, ':', true);
					if((component.size() != 2 or component[1].empty())) {
						logger::logger(__FUNCTION__, "Incomplete data for modem at index: " + index, "stderr");
						continue;
					}
					else if(component[0] == "equipment_id") modem.setIMEI( component[1]);
					else if(component[0] == "operator_name") {
						//logger::logger(__FUNCTION__, "Setting ISP: " + component[1]);
						modem.setISP( helpers::to_upper(component[1]));
					}
				}
			}

			auto it_modemCollection = std::find_if(this->modemCollection.begin(), this->modemCollection.end(), [&](Modem* modem1){ return *modem1 == modem; });
			if(it_modemCollection == this->modemCollection.end()) {
				logger::logger(__FUNCTION__, modem.getInfo() + " - Not found in list");
				if(modem) {
					string insert_modem_query = "INSERT INTO __DEKU__.MODEMS (IMEI, TYPE, STATE, POWER) VALUES("
					+ modem.getIMEI() 
					+ "," 
					+ helpers::to_lowercase(modem.getType() == Modem::MMCLI ? "\"MMCLI\"" : "\"SSH\"") 
					+ ",\"active\"," +
					+ "\"plugged\")";
					this->mysqlConnection.query( insert_modem_query );

					logger::logger(__FUNCTION__, modem.getInfo() + " - Adding modem to list");
					tmp_modemCollection.push_back( new Modem(modem) );
				}
			}
			else {
				logger::logger(__FUNCTION__, modem.getInfo() + " - Modem already present...");
				//Find and store
				tmp_modemCollection.push_back(*it_modemCollection);
			}
		}
		//logger::logger(__FUNCTION__, "Exited..");
		logger::logger(__FUNCTION__, "Finished refreshing with " + to_string(tmp_modemCollection.size()) + " modems");
		this->modemCollection = tmp_modemCollection;

		logger::logger(__FUNCTION__, "Updating Plugged in list");

		if( this->modemCollection.empty() ) {
			string none_plugged_query = "UPDATE __DEKU__.MODEMS SET POWER = 'not_plugged' WHERE 1";
			this->mysqlConnection.query( none_plugged_query );
		}
		else {
			vector<string> list_imei;
			for(auto modem : this->modemCollection ) {
				list_imei.push_back( modem->getIMEI());
			}
			string plugged_query = "UPDATE __DEKU__.MODEMS SET POWER = 'plugged' WHERE IMEI = ";
			for(size_t i=0;i < this->modemCollection.size(); ++i) {
				string imei = this->modemCollection[i]->getIMEI();
				plugged_query += imei;
				if( (i + 1 ) < list_imei.size()) {
					plugged_query += " AND IMEI = ";
				}
			}
			this->mysqlConnection.query( plugged_query );

			string unplugged_query = "UPDATE __DEKU__.MODEMS SET POWER = 'not_plugged' WHERE IMEI != ";
			for(size_t i=0;i<list_imei.size();++i ) {
				string imei = list_imei[i];
				unplugged_query += imei;
				if( (i + 1 ) < list_imei.size()) {
					unplugged_query += " AND IMEI != ";
				}
			}

			logger::logger(__FUNCTION__, unplugged_query);
			this->mysqlConnection.query( unplugged_query );
		}
			
		// TODO: Should this take  up to 10 seconds?
		// TODO: change this time to a variable
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

void Modems::startAllModems() {
	while( 1 ) {  //TODO: Use an internal variable to control this loop
		logger::logger(__FUNCTION__, to_string(this->threaded_modems.size()) + " currently threaded modems");
		for(map<Modem*, std::thread>::iterator it=this->threaded_modems.begin();it!=this->threaded_modems.end();++it ) {
			//if(std::find(this->modemCollection.begin(), this->modemCollection.end(), it->first) == this->modemCollection.end()) { //This checks against mem values
			Modem* modem = it->first;
			if(std::find_if(this->modemCollection.begin(), this->modemCollection.end(), [&](Modem* modem1){ return *modem1 == *modem; }) == this->modemCollection.end()) { //This checks against type of modem
				logger::logger(__FUNCTION__, it->first->getInfo() + " - Modem not available, stopping thread");
				if(this->threaded_modems.find(it->first) != this->threaded_modems.end()) {
					it->first->end();
					this->threaded_modems.erase(it);
					if(this->threaded_modems.empty()) {
						logger::logger(__FUNCTION__, "Currently no modem threads running");
						break;
					}
					else {
						if(auto tmp_it = ++it; tmp_it == this->threaded_modems.end()) {
							logger::logger(__FUNCTION__, "Would break the next iteration, exiting loop");
							break;
						}
					}
					logger::logger(__FUNCTION__, to_string(this->threaded_modems.size()) + " currently threaded modems");
				}
			}
		}

		for(auto& modem : this->modemCollection) {
			if(this->threaded_modems.find(modem) == this->threaded_modems.end()) {
				this->threaded_modems.insert(make_pair(modem, std::thread(&Modem::start, modem)));
				logger::logger(__FUNCTION__, modem->getInfo() + " - Began thread...");
				this->threaded_modems[modem].detach();
			}
			else {
				logger::logger(__FUNCTION__, modem->getInfo() + " - Already threaded..." );
			}
		}
		helpers::sleep_thread( 5 );
	}
	
}
