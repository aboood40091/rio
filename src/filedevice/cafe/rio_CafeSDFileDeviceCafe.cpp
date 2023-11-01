#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <filedevice/cafe/rio_CafeSDFileDeviceCafe.h>

#include <whb/sdcard.h>

#ifndef RIO_CAFE_SD_BASE_PATH
#define RIO_CAFE_SD_BASE_PATH "rio"
#endif // RIO_CAFE_SD_BASE_PATH

namespace {

static u32 sSDMountCount = 0;

}

namespace rio {

CafeSDFileDevice::SDCardMounter::SDCardMounter()
{
    if (sSDMountCount++ == 0)
    {
        [[maybe_unused]] BOOL mounted = WHBMountSdCard();
        RIO_ASSERT(mounted);
    }
}

CafeSDFileDevice::SDCardMounter::~SDCardMounter()
{
    if (--sSDMountCount == 0)
    {
        [[maybe_unused]] BOOL unmounted = WHBUnmountSdCard();
        RIO_ASSERT(unmounted);
    }
}

CafeSDFileDevice::CafeSDFileDevice()
    : StdIOFileDevice("sd", std::string(WHBGetSdCardMountPath()) + "/" RIO_CAFE_SD_BASE_PATH)
    , mSDMounter()
{
    RIO_LOG("CafeSDFileDevice.mCWD: %s\n", mCWD.c_str());
}

}

#endif // RIO_IS_CAFE
