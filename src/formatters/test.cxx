#include "helpers.hpp"

using namespace std;

int main() {
	string sample_string = "Hello\\nworld\\n";
	cout << sample_string << endl << "----------" << endl;
	cout << helpers::find_and_replace( "\\n", "\n", sample_string) << endl;

	return 0;
}
