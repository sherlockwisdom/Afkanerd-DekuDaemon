#include <iostream>

#include <libmm-glib.h>

using namespace std;

void init_callback(auto source_object, auto res, auto user_data ) {
	cout << "In callback..." << endl;

	GError **error;
	MMManager* mm_manager = mm_manager_new_finish( res, error);

	// mm_manager_scan_devices( mm_manager, cancellable, callback, user_data);
}

int main() {

	GDBusConnection *connection;
	GDBusObjectManagerClientFlags flags;
	GCancellable *cancellable;
	// GAsyncReadyCallback callback = init_callback;
	gpointer user_data;
	GError** error;


	MMManager* mm_manager = mm_manager_new_sync( connection, flags, cancellable, error);
	gboolean scan_state = mm_manager_scan_devices_sync( mm_manager, cancellable, error );

	cout << "scan_state: " << scan_state << endl;
	return 0;
}
