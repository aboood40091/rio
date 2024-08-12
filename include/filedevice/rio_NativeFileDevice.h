#ifndef RIO_FILE_NATIVE_DEVICE_H
#define RIO_FILE_NATIVE_DEVICE_H

#include <misc/rio_Types.h>

#if RIO_IS_DESKTOP
#include <filedevice/rio_StdIOFileDevice.h>
#else
#include <filedevice/rio_FileDevice.h>
#endif // RIO_IS_DESKTOP

namespace rio {

#if RIO_IS_DESKTOP
class NativeFileDevice : public StdIOFileDevice
#else
class NativeFileDevice : public FileDevice
#endif // RIO_IS_DESKTOP
{
public:
    NativeFileDevice();
    virtual ~NativeFileDevice() {}

    virtual std::string getNativePath(const std::string& path) const
    {
        return path;
    }

protected:
    NativeFileDevice(const std::string& drive_name);

#if !RIO_IS_DESKTOP
private:
    virtual FileDevice* doOpen_(FileHandle* handle, const std::string& filename, FileOpenFlag flag);
    virtual bool doClose_(FileHandle* handle);
    virtual bool doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size);
    virtual bool doWrite_(u32* write_size, FileHandle* handle, const u8* buf, u32 size);
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin);
    virtual bool doGetCurrentSeekPos_(u32* pos, FileHandle* handle);
    virtual bool doGetFileSize_(u32* size, const std::string& path);
    virtual bool doGetFileSize_(u32* size, FileHandle* handle);
    virtual bool doIsExistFile_(bool* is_exist, const std::string& path);
    virtual RawErrorCode doGetLastRawError_() const;

public:
    const std::string& getCWD() const
    {
        return mCWD;
    }

private:
    std::string     mCWD;
    RawErrorCode    mLastRawError;
#endif
};

}

#endif // RIO_FILE_NATIVE_DEVICE_H
