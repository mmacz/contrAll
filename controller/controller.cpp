#include <array>
#include <algorithm>
#include <cassert>
#include "controller.hpp"
extern "C" {
 #include "libusb.h"
}

static constexpr uint32_t SONY_CORP_VENDOR_ID  = 0x054c;
static constexpr uint32_t SONY_CORP_PRODUCT_ID = 0x0ce6;

struct VenProdId {
    uint32_t vendorId;
    uint32_t productId;
    VenProdId(uint32_t vid, uint32_t pid) : vendorId{vid}, productId{pid} {};
};

const std::array<VenProdId, 1> SUPPORTED_MANUFACTURERS = {
    VenProdId{SONY_CORP_VENDOR_ID, SONY_CORP_PRODUCT_ID}
};

static ControllerDesc MakeDesc(const libusb_device_descriptor& desc) {
    ControllerDesc d;
    memcpy(&d, &desc, sizeof(desc));
    return d;
}

Controllers::~Controllers() {
    libusb_exit((libusb_context*)mpUsbCtx);
}

const std::vector<ControllerDesc> Controllers::get_devices() {
    std::vector<ControllerDesc> devices;

    libusb_context* usbCtx = (libusb_context*)mpUsbCtx;
    auto rc = libusb_init(&usbCtx);
    assert(rc == LIBUSB_SUCCESS);

    libusb_device **ppDevices;
    auto numDevices = libusb_get_device_list(usbCtx, &ppDevices);

    for (int devIdx = 0; devIdx < numDevices; ++devIdx) {
        const auto *pDev = ppDevices[devIdx];
        assert(pDev != nullptr);
        libusb_device_descriptor dd;
        auto err = libusb_get_device_descriptor(const_cast<libusb_device*>(pDev), &dd);
        assert(err == LIBUSB_SUCCESS);
        if (std::find_if(SUPPORTED_MANUFACTURERS.begin()
                        ,SUPPORTED_MANUFACTURERS.end(),
                        [dd](const VenProdId& vpi) {return dd.idProduct == vpi.productId && dd.idVendor == vpi.vendorId;})
            != SUPPORTED_MANUFACTURERS.end()
           )
        {
            devices.push_back(MakeDesc(dd));
        }
    }

    libusb_free_device_list(ppDevices, 1);

    return devices;
}

