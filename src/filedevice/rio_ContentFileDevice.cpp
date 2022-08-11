#include <filedevice/rio_ContentFileDevice.h>

namespace rio {

ContentFileDevice::ContentFileDevice()
    : NativeFileDevice("content")
#if RIO_IS_CAFE
    , mContentPath("/vol/content")
#else
    , mContentPath(NativeFileDevice::getCWD() + "/fs/content")
#endif
{
    RIO_LOG("ContentFileDevice.mCWD: %s\n", mContentPath.c_str());
}

}
