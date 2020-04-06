#include <libusb-1.0/libusb.h>

int main() {
	libusb_device** devices;
	libusb_context* context = NULL;

	size_t list_of_devices = libusb_get_device_list(NULL, &devices);
}
