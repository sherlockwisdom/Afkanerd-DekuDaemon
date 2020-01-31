#include <map>
using namespace std;

class Modems {
	public:
		Modems(){}
		map<string,string> getAll();
		
		vector<string> getAllIndexes();
		vector<string> getAllISP();
		vector<string> getAllIMEI();
		~Modems(){}
};
