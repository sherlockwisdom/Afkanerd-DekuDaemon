#ifndef ISP_DETERMINER_H_INCLUDE_
#define ISP_DETERMINER_H_INCLUDE_
using namespace std;

namespace isp_determiner {
	//TODO: Create dynamic version of get_isp - should be able to deduce phonenumbers of anytime based surely on regex
	//TODO: Add parsers for country code values
	string get_isp( string number ) {
		size_t start_counter = number.find("+237") != string::npos ? number.find("+237") : 0;
		if(number[start_counter] == '6') {
			switch(number[start_counter+1]) {
				case '5':
					switch(number[start_counter+2]) {
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
							return "MTN";
						break;

						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							return "ORANGE";
						break;

					}
				break;

				case '6': return "NEXTTEL";

				case '7': return "MTN";
				break;

				case '8':
					  switch(number[2]) {
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
							return "MTN";
						break;
					  }
				break;

				case '9': return "ORANGE";
				break;
			}
		}
		return "UNKNOWN";
	}
}


#endif
