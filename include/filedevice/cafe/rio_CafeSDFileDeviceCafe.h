#ifndef RIO_FILE_CAFE_SD_DEVICE_H
#define RIO_FILE_CAFE_SD_DEVICE_H

#include <filedevice/rio_StdIOFileDevice.h>

namespace rio {

class CafeSDFileDevice : public StdIOFileDevice
{
public:
    CafeSDFileDevice();
    virtual ~CafeSDFileDevice() {}

    virtual std::string getNativePath(const std::string& path) const
    {
        return mCWD + '/' + path;
    }

private:
    class SDCardMounter
    {
    public:
        SDCardMounter();
        ~SDCardMounter();
    };

    SDCardMounter   mSDMounter;
};

}

#endif // RIO_FILE_CAFE_SD_DEVICE_H
