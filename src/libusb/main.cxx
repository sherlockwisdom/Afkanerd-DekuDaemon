#include <iostream>
#include <libusb-1.0/libusb.h>

int main() {
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
		std::cout << "Found " << number_of_devices << std::endl;

	}

	for(size_t i=0;i<number_of_devices;++i) {
		auto dev = devices[i];
		libusb_device_descriptor dev_descriptor;
		auto desc_state = libusb_get_device_descriptor(dev, &dev_descriptor);
		if( desc_state != 0 ) {
			std::cerr << "Failed to get device descriptor" << std::endl;
			libusb_free_device_list(devices, 1);
			libusb_exit(context);
			return 1;
		}
		libusb_device_handle* dev_handle;

		auto open_state = libusb_open(dev, &dev_handle);
		if( open_state != 0) {
			std::cerr << "Failed to open device " << std::endl;
			std::cerr << libusb_error_name( open_state ) << std::endl;
			libusb_free_device_list(devices, 1);
			libusb_exit(context);
			return 1;
		}

		// http://libusb.sourceforge.net/api-1.0/structlibusb__device__descriptor.html
		auto manf_desc = dev_descriptor.bDeviceClass;
		unsigned char* pnt_str_output = NULL;

		// http://libusb.sourceforge.net/api-1.0/group__libusb__desc.html#ga5e9ab08d490a7704cf3a9b0439f16f00
		int get_desc_state = libusb_get_string_descriptor_ascii(dev_handle, manf_desc, pnt_str_output, dev_descriptor.bLength);
		std::cout << get_desc_state << " Bytes returned" << std::endl;
		if( get_desc_state < 0 ) {
			std::cerr << "Failed to get descriptor details" << std::endl;
			std::cerr << libusb_error_name( get_desc_state ) << std::endl;
			libusb_free_device_list(devices, 1);
			libusb_exit(context);
			return 1;
		}

		std::cout << "Manf_desc: " << pnt_str_output << std::endl;
	}

	libusb_free_device_list(devices, 1);
	libusb_exit(context);

	return 0;
}
