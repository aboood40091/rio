#ifndef RIO_FILE_DEVICE_MANAGER_H
#define RIO_FILE_DEVICE_MANAGER_H

#include <filedevice/rio_MainFileDevice.h>
#include <filedevice/rio_NativeFileDevice.h>

#if RIO_IS_CAFE
#include <coreinit/filesystem.h>
#endif // RIO_IS_CAFE

namespace rio {

class FileDeviceMgr
{
private:
    typedef TList<FileDevice*> DeviceList;

public:
    static bool createSingleton();
    static void destroySingleton();
    static FileDeviceMgr* instance() { return sInstance; }

private:
    static FileDeviceMgr* sInstance;

    FileDeviceMgr();
    ~FileDeviceMgr();

    FileDeviceMgr(const FileDeviceMgr&);
    FileDeviceMgr& operator=(const FileDeviceMgr&);

public:
    FileDevice* findDeviceFromPath(const std::string& path, std::string* no_drive_path) const;

    FileDevice* open(FileHandle* handle, const std::string& filename, FileDevice::FileOpenFlag flag)
    {
        FileDevice* device = tryOpen(handle, filename, flag);
        RIO_ASSERT(device);
        return device;
    }

    FileDevice* tryOpen(FileHandle* handle, const std::string& filename, FileDevice::FileOpenFlag flag);

    u8* load(FileDevice::LoadArg& arg)
    {
        u8* ret = tryLoad(arg);
        RIO_ASSERT(ret);
        return ret;
    }

    u8* tryLoad(FileDevice::LoadArg& arg);

    void unload(u8* data)
    {
        RIO_ASSERT(data);
        delete[] data;
    }

    void mount(FileDevice* device, const std::string& drive_name = "");
    void unmount(const std::string& drive);
    void unmount(FileDevice* device);

    void setDefaultFileDevice(FileDevice* device)
    {
        mDefaultFileDevice = device;
    }

    FileDevice* getDefaultFileDevice() const
    {
        return mDefaultFileDevice;
    }

    MainFileDevice* getMainFileDevice() const
    {
        return mMainFileDevice;
    }

    NativeFileDevice* getNativeFileDevice() const
    {
        return mNativeFileDevice;
    }

    FileDevice* findDevice(const std::string& drive) const;

#if RIO_IS_CAFE
    FSClient* getFSClient() { return &mFSClient; }
    const FSClient* getFSClient() const { return &mFSClient; }

    ContentFileDevice* getCafeContentFileDevice() const
    {
        return mCafeContentFileDevice;
    }
#endif // RIO_IS_CAFE

private:
    DeviceList          mDeviceList;
    FileDevice*         mDefaultFileDevice;
    MainFileDevice*     mMainFileDevice;
    NativeFileDevice*   mNativeFileDevice;

#if RIO_IS_CAFE
    FSClient            mFSClient;
    ContentFileDevice*  mCafeContentFileDevice;
#endif // RIO_IS_CAFE
};

}

#endif // RIO_FILE_DEVICE_MANAGER_H
