#include <iostream>
#include <libusb-1.0/libusb.h>

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

		libusb_device_handle* dev_handle;
		auto open_state = libusb_open(device, &dev_handle);
		if( open_state != 0) {
			std::cerr << "Failed to open device " << std::endl;
			std::cerr << libusb_error_name( open_state ) << std::endl;
			//libusb_free_device_list(devices, 1);
			//libusb_exit(context);
			//return 1;

			
			continue;
		}

		/*libusb_device_descriptor dev_desc;
		if( libusb_get_device_descriptor(device, &dev_desc) < 0) {
			std::cout << "Failed to get descriptor" << std::endl;
			continue;
		}

		//https://github.com/libusb/libusb/blob/master/examples/testlibusb.c
		int dev_man = dev_desc.bNumConfigurations;
		*/
		if( true ) {
			//std::cout << dev_man << std::endl;
			unsigned char* data;
			int device_string_descriptor = libusb_get_string_descriptor_ascii(dev_handle, 0x02, data, 1024);
			if( device_string_descriptor > 0) 
				std::cout << "Man: " << std::endl;
				//std::cout << "Manufacturer: " << data << std::endl;

		}
		std::cout << "DONE" << std::endl;
		/// Cleaning here
		libusb_close( dev_handle );
	}

	libusb_free_device_list(devices, 1);
	libusb_exit(context);

	return 0;
}
