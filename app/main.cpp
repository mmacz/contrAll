#include <iostream>
#include <cassert>
#include <format>
#include <string_view>

extern "C" {
    #include "libusb.h"
}

void usbCallback(libusb_context *pCtx, enum libusb_log_level level, const char *str) {
    std::cout << "Logging: " << level << " " << str << std::endl;
}

int main(int argc, char **argv) {
    libusb_context *pUsbCtx = nullptr;
    auto rc = libusb_init(&pUsbCtx);
#if defined DEBUG
    libusb_set_log_cb(pUsbCtx, usbCallback, LIBUSB_LOG_LEVEL_DEBUG);
#endif

    libusb_device **pDevices = nullptr;
    auto numDevices = libusb_get_device_list(pUsbCtx, &pDevices);

    if (numDevices) {
        std::cout << "devices found: " << numDevices << std::endl;
        for (int i = 0 ; i < numDevices; ++i) {
            auto *pDev = pDevices[i];
            if (pDev) {
                struct libusb_device_descriptor desc;
                libusb_get_device_descriptor(pDev, &desc);
                std::cout << std::format("vendor id: {}\n", desc.idVendor);
                libusb_device_handle *pHdl;
                libusb_open(pDev, &pHdl);
                if (pHdl) {
                    char data[32];
                    memset(data, 0, sizeof(data));
                    libusb_get_string_descriptor_ascii(pHdl, desc.iSerialNumber, (unsigned char*)data, 31);
                    std::string_view sv{data};
                    std::cout << std::format("SN: {}\n", sv);

                    memset(data, 0, sizeof(data));
                    libusb_get_string_descriptor_ascii(pHdl, desc.iManufacturer, (unsigned char*)data, 31);
                    sv = std::string_view(data);
                    std::cout << std::format("manufacturer: {}\n", sv);
                }
                libusb_close(pHdl);
            }
        }
    }
    libusb_free_device_list(pDevices, 1);
    libusb_exit(pUsbCtx);
    return 0;
}

