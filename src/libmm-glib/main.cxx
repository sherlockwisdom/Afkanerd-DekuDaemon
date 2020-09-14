#include <iostream>

#include <libmm-glib.h>

using namespace std;

int main() {
	MMObject* mmobject;

	auto mm_path = mm_object_get_path( mmobject );

	cout << mm_path << endl;

	return 0;
}
