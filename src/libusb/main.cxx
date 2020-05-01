#include <iostream>
#include <libusb-1.0/libusb.h>

using namespace std;

int main(int argc, char** argv) {
	libusb_device** devices;
	libusb_context* context = NULL;
	
	libusb_init(&context);

	size_t number_of_devices = libusb_get_device_list(context, &devices);
	if( number_of_devices < 1 ) {
		std::cerr << libusb_error_name( number_of_devices ) << std::endl;
		libusb_free_device_list(devices, 1);
		libusb_exit(context);
		return 1;
	}
	else {
		std::cout << "Found " << number_of_devices << std::endl << "------" << std::endl;

	}

	for(size_t i=0;i<number_of_devices;++i, std::cout<<std::endl) {
		std::cout << "> Device " << i+1 << "/" << number_of_devices << std::endl;
		libusb_device* device = devices[i];

		struct libusb_device_descriptor* device_descriptor;

		//1. Get device descriptor
		int ret = libusb_get_device_descriptor( device, device_descriptor );
		if( ret < 0 ) {
			cerr << errno << endl;
			continue;
		}

		uint8_t num_dev_configs = device_descriptor->bNumConfigurations;
		cout << "# configs: " << (unsigned int)num_dev_configs << endl;
		
		/// Cleaning here
		//libusb_close( dev_handle );
		std::cout << "DONE" << std::endl;
	}

	libusb_free_device_list(devices, 1);
	libusb_exit(context);

	return 0;
}
