#ifndef RIO_FILE_CONTENT_DEVICE_H
#define RIO_FILE_CONTENT_DEVICE_H

#include <filedevice/rio_NativeFileDevice.h>

namespace rio {

class ContentFileDevice : public NativeFileDevice
{
public:
    ContentFileDevice();
    virtual ~ContentFileDevice() {}

    virtual std::string getNativePath(const std::string& path) const
    {
        return mContentPath + '/' + path;
    }

    const std::string& getContentNativePath() const
    {
        return mContentPath;
    }

private:
    std::string mContentPath;
};

}

#endif // RIO_FILE_CONTENT_DEVICE_H
