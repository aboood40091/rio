#ifndef RIO_FILE_NATIVE_DEVICE_H
#define RIO_FILE_NATIVE_DEVICE_H

#include <filedevice/rio_FileDevice.h>

#if RIO_IS_CAFE
typedef struct FSClient FSClient;
#endif // RIO_IS_CAFE

namespace rio {

class NativeFileDevice : public FileDevice
{
public:
    NativeFileDevice();
    virtual ~NativeFileDevice() {}

protected:
    NativeFileDevice(const std::string& drive_name);

private:
    virtual FileDevice* doOpen_(FileHandle* handle, const std::string& filename, FileOpenFlag flag);
    virtual bool doClose_(FileHandle* handle);
    virtual bool doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size);
    virtual bool doWrite_(u32* write_size, FileHandle* handle, const u8* buf, u32 size);
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin);
    virtual bool doGetCurrentSeekPos_(u32* pos, FileHandle* handle);
    virtual bool doGetFileSize_(u32* size, const std::string& path);
    virtual bool doGetFileSize_(u32* size, FileHandle* handle);

public:
    virtual std::string getNativePath(const std::string& path) const
    {
        return path;
    }

    const std::string& getCWD() const
    {
        return mCWD;
    }

private:
    std::string mCWD;

#if RIO_IS_CAFE
    FSClient* getUsableFSClient_() const;

    FSClient* mFSClient;
#endif // RIO_IS_CAFE
};

}

#endif // RIO_FILE_NATIVE_DEVICE_H
