#pragma once
#include <string>
#include <format>
#include <vector>
#include <cstdint>
#include <sstream>

/* Adapter for the libusb descriptor */
struct ControllerDesc {
    uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t bcdUSB;
	uint8_t  bDeviceClass;
	uint8_t  bDeviceSubClass;
	uint8_t  bDeviceProtocol;
	uint8_t  bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t  iManufacturer;
	uint8_t  iProduct;
	uint8_t  iSerialNumber;
	uint8_t  bNumConfigurations;

    std::string toString() const {
        std::stringstream ss;
        ss << std::format("bLength: {}\n", bLength);
        ss << std::format("bDescriptorType: {}\n", bDescriptorType);
        ss << std::format("bcdUSB: 0x{:04x}\n", bcdUSB);
        ss << std::format("bDeviceClass: {}\n", bDeviceClass);
        ss << std::format("bDeviceSubClass: {}\n", bDeviceSubClass);
        ss << std::format("bDeviceProtocol: {}\n", bDeviceProtocol);
        ss << std::format("bMaxPacketSize0: {}\n", bMaxPacketSize0);
        ss << std::format("idVendor: 0x{:04x}\n", idVendor);
        ss << std::format("idProduct: 0x{:04x}\n", idProduct);
        ss << std::format("bcdDevice: 0x{:04x}\n", bcdDevice);
        ss << std::format("iManufacturer: {}\n", iManufacturer);
        ss << std::format("iProduct: {}\n", iProduct);
        ss << std::format("iSerialNumber: {}\n", iSerialNumber);
        ss << std::format("bNumConfigurations: {}\n", bNumConfigurations);
        return ss.str();
    }
};

class ControllersPrv;
class Controllers {
    public:
        Controllers();
        ~Controllers();
        const std::vector<ControllerDesc> get_connected() const;
    private:
        std::unique_ptr<ControllersPrv> mImpl;
};

