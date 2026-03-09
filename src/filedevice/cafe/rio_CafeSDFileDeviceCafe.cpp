#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <filedevice/cafe/rio_CafeSDFileDeviceCafe.h>

#include <whb/sdcard.h>

#ifndef RIO_CAFE_SD_BASE_PATH
#define RIO_CAFE_SD_BASE_PATH "rio"
#endif // RIO_CAFE_SD_BASE_PATH

namespace rio {

u32 CafeSDCardMounter::sSDMountCount = 0;

CafeSDCardMounter::CafeSDCardMounter()
{
    if (sSDMountCount++ == 0)
    {
        [[maybe_unused]] BOOL mounted = WHBMountSdCard();
        RIO_ASSERT(mounted);
    }
}

CafeSDCardMounter::~CafeSDCardMounter()
{
    if (--sSDMountCount == 0)
    {
        [[maybe_unused]] BOOL unmounted = WHBUnmountSdCard();
        RIO_ASSERT(unmounted);
    }
}

const char* CafeSDCardMounter::getSDCardMountPath()
{
    RIO_ASSERT(sSDMountCount > 0);
    return WHBGetSdCardMountPath();
}

CafeSDFileDevice::CafeSDFileDevice()
    : CafeSDCardMounter()
    , StdIOFileDevice("sd", std::string(CafeSDCardMounter::getSDCardMountPath()) + "/" RIO_CAFE_SD_BASE_PATH)
{
    RIO_LOG("CafeSDFileDevice.mCWD: %s\n", mCWD.c_str());
}

}

#endif // RIO_IS_CAFE
