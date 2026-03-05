#ifndef RIO_FILE_CAFE_SD_DEVICE_H
#define RIO_FILE_CAFE_SD_DEVICE_H

#include <filedevice/rio_StdIOFileDevice.h>

namespace rio {

class CafeSDCardMounter
{
public:
    CafeSDCardMounter();
    ~CafeSDCardMounter();

    CafeSDCardMounter(const CafeSDCardMounter&) = delete;
    CafeSDCardMounter& operator=(const CafeSDCardMounter&) = delete;

    CafeSDCardMounter(CafeSDCardMounter&&) = delete;
    CafeSDCardMounter& operator=(CafeSDCardMounter&&) = delete;

    const char* getSDCardMountPath();

private:
    static u32 sSDMountCount;
};

class CafeSDFileDevice : public CafeSDCardMounter, public StdIOFileDevice
{
public:
    CafeSDFileDevice();
    virtual ~CafeSDFileDevice() {}

    virtual std::string getNativePath(const std::string& path) const
    {
        return mCWD + '/' + path;
    }
};

}

#endif // RIO_FILE_CAFE_SD_DEVICE_H
