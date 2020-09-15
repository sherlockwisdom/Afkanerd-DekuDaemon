// https://www.freedesktop.org/software/ModemManager/libmm-glib/1.10.0/
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

	/*
	GError* error;
	GBusType bus_type = G_BUS_TYPE_SYSTEM;
	GCancellable* cancellable = g_cancellable_new( );
	GDBusConnection* connection = g_bus_get_sync( bus_type, cancellable, &error);
	if ( error != NULL ) {
		cerr << "Error getting bus sync..." << endl;
		cerr << error->message << endl;
		g_clear_error( &error );
	}
	GDBusObjectManagerClientFlags flags = G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE;
	// GDBusObjectManagerClientFlags flags = G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_DO_NOT_AUTO_START;
	*/

	/*
	MMManager* mm_manager = mm_manager_new_sync( connection, flags, NULL, &error);
	if ( error != NULL ) {
		cerr << error->message << endl;
		g_clear_error( &error );
		return 1;
	}
	// auto mm_version = mm_manager_get_version( mm_manager );
	// cout << "Version: " << mm_manager_get_version( mm_manager ) << endl;
	gboolean scan_state = mm_manager_scan_devices_sync( mm_manager, NULL, &error );

	if( scan_state ) {
		cout << "Scan successful...." << endl;
	}
	else {
		cout << "Scan failed..." << endl;
		cout << error->message << endl;
	}
	*/

	MMObject* mm_object;
	// MMModem* modem = mm_object_get_modem( mm_object );

	return 0;
}
