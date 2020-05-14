#include <iostream>
#include <libusb-1.0/libusb.h>
#include "../formatters/helpers.hpp"

using namespace std;

extern string logger::show_state = "TESTING";

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

		//1. Open device
		cout << "+ Opening devices...";
		struct libusb_device_handle* dev_handle;
		int ret = libusb_open(device, &dev_handle);
		if( ret != 0) {
			//cerr << errno << endl;
			logger::logger_errno( errno );
			continue;
		}
		cout << " done" << endl;

		//2. Get device descriptor
		/*
		cout << "+ Getting device descriptors...";
		struct libusb_device_descriptor* device_descriptor;
		ret = libusb_get_device_descriptor( device, device_descriptor );
		if( ret < 0 ) {
			// cerr << errno << endl;
			logger::logger_errno( errno );
			continue;
		}
		cout << " done" << endl;
		uint8_t num_dev_configs = device_descriptor->bNumConfigurations;
		uint8_t dev_serial_num = device_descriptor->iSerialNumber;
		cout << "# configs: " << (unsigned int)num_dev_configs << endl;
		cout << "Index of serial #: " << (unsigned int)dev_serial_num << endl;
		*/

		//3. Get descriptor
		unsigned char* data;
		// static int descriptor = libusb_get_descriptor(dev_handle, LIBUSB_DT_DEVICE, dev_serial_num, data, 1024);
		int descriptor = libusb_get_string_descriptor_ascii(dev_handle, 1, data, 255);
		if( descriptor < 0 ) {
			// cerr << errno << endl;
			logger::logger_errno( errno );
			continue;
		}
		cout << "> Data: " << data << endl;


		//4. Reset device
		ret = libusb_reset_device(dev_handle);
		if( ret != 0 ) {
			logger::logger_errno( errno );
		}


		/// Cleaning here
		libusb_close( dev_handle );
		std::cout << "DONE" << std::endl;
	}

	libusb_free_device_list(devices, 1);
	libusb_exit(context);

	return 0;
}
