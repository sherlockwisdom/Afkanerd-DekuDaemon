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

	// /*
	GError* error;
	GBusType bus_type = G_BUS_TYPE_SYSTEM;
	GCancellable* cancellable = g_cancellable_new( );

	GIOStream* stream;
	const gchar* guid = NULL;
	GDBusConnectionFlags flags = G_DBUS_CONNECTION_FLAGS_NONE;
	GDBusAuthObserver *observer = NULL;
	GDBusConnection* connection = g_dbus_connection_new_sync( stream, guid, flags, observer, cancellable, &error);
	if ( error  ) {
		cerr << "Error getting bus sync..." << endl;
		cerr << error->message << endl;
		g_clear_error( &error );
		return 1;
	}
	else {
		cout << "GDBusConnection acquired..." << endl;
	}
	GDBusObjectManagerClientFlags gd_object_flags = G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE;
	// GDBusObjectManagerClientFlags flags = G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_DO_NOT_AUTO_START;
	// */

	// /*
	MMManager* mm_manager = mm_manager_new_sync( connection, gd_object_flags, NULL, &error);
	if ( error != NULL ) {
		cerr << error->message << endl;
		g_clear_error( &error );
		return 1;
	}
	// auto GD_Bus_Proxy = mm_manager_peek_proxy( mm_manager );
	
	// cout << "Version: " << mm_manager_version << endl;
	gboolean scan_state = mm_manager_scan_devices_sync( mm_manager, NULL, &error );
	const gchar* mm_manager_version = mm_manager_get_version( mm_manager );

	if( scan_state ) {
		cout << "Scan successful...." << endl;
	}
	else {
		cout << "Scan failed..." << endl;
		cout << error->message << endl;
	}
	// */

	return 0;
}
