#include <array>
#include <algorithm>
#include <cassert>
#include <memory>
#include <stdexcept>
#include <iostream>

extern "C" {
    #include "libusb.h"
}

#include "controller.hpp"

static constexpr uint32_t SONY_CORP_VENDOR_ID  = 0x054c;
static constexpr uint32_t SONY_CORP_PRODUCT_ID = 0x0ce6;
static constexpr uint32_t MCSFT_CORP_VENDOR_ID = 0x045e;
static constexpr uint32_t MCSFT_CORP_PRODUCT_ID = 0x02d1;

struct VenProdId {
    uint32_t vendorId;
    uint32_t productId;
    VenProdId(uint32_t vid, uint32_t pid) : vendorId{vid}, productId{pid} {};
};

const std::array<VenProdId, 2> SUPPORTED_MANUFACTURERS = {
    VenProdId{SONY_CORP_VENDOR_ID, SONY_CORP_PRODUCT_ID},
    VenProdId{MCSFT_CORP_VENDOR_ID, MCSFT_CORP_PRODUCT_ID}
};

class ControllersPrv {
    public:
        ControllersPrv();
        ~ControllersPrv();
        const std::vector<ControllerDesc> get_connected() const;
        void open_device(const ControllerDesc& desc);
        void close_device();
        const ControllerData& read_from_device();
    private:
        libusb_context *mpCtx;
        libusb_device_handle *mhDev;
        ControllerData mData;
        std::array<uint8_t, 1024> mDataBuff;
};

Controllers::Controllers() {
    mImpl.reset(new ControllersPrv());
}

Controllers::~Controllers() {
}

ControllersPrv::ControllersPrv() 
    : mpCtx{nullptr} {
    auto rc = libusb_init(&mpCtx);
    assert(rc == LIBUSB_SUCCESS);
}

ControllersPrv::~ControllersPrv() {
    libusb_exit(mpCtx);
    mpCtx = nullptr;
}

static ControllerDesc MakeDesc(const libusb_device_descriptor& desc) {
    ControllerDesc d;
    memcpy(&d, &desc, sizeof(desc));
    return d;
}

const std::vector<ControllerDesc> Controllers::get_connected() const {
    if (!mImpl) {
        throw std::runtime_error("Not initialized");
    }
    return mImpl->get_connected();
}

const std::vector<ControllerDesc> ControllersPrv::get_connected() const {
    std::vector<ControllerDesc> devices;

    libusb_device **ppDevices;
    auto numDevices = libusb_get_device_list(mpCtx, &ppDevices);

    for (int devIdx = 0; devIdx < numDevices; ++devIdx) {
        const auto *pDev = ppDevices[devIdx];
        if (!pDev) {
            throw std::runtime_error("Cannot retrieve USB device");
        }

        libusb_device_descriptor dd;
        auto err = libusb_get_device_descriptor(const_cast<libusb_device*>(pDev), &dd);
        if (err) {
            throw std::runtime_error("Cannot retrieve descriptor for device");
        }

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

void Controllers::open_device(const ControllerDesc& desc) {
    if (!mImpl) {
        throw std::runtime_error("Not initialized");
    }

    mImpl->open_device(desc);
}

void ControllersPrv::open_device(const ControllerDesc& desc) {
    /* Temporarily solution */
    mhDev = libusb_open_device_with_vid_pid(mpCtx, desc.idVendor, desc.idProduct);
    if (!mhDev) {
        throw std::runtime_error("Cannot get device handle");
    }
}

void Controllers::close_device() {
    if (!mImpl) {
        throw std::runtime_error("Not initialized");
    }
    mImpl->close_device();
}

void ControllersPrv::close_device() {
    libusb_close(mhDev);
    mhDev = nullptr;
}

const ControllerData& Controllers::read_from_device() {
    if (!mImpl) {
        throw std::runtime_error("Not initialized");
    }
    return mImpl->read_from_device();
}

const ControllerData& ControllersPrv::read_from_device() {
    int32_t readDataSize {0};
    int rc = libusb_bulk_transfer(mhDev, LIBUSB_ENDPOINT_IN, mDataBuff.data(), mDataBuff.size(), &readDataSize, 100);
    std::cout << std::format("Transfer rc: {}\n", rc);
    mData.pData = mDataBuff.data();
    mData.size = readDataSize;
    return mData;
}

