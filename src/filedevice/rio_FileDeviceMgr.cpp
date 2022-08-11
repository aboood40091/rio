#include <filedevice/rio_FileDeviceMgr.h>
#include <filedevice/rio_Path.h>

#if RIO_IS_CAFE
extern "C" {
int SAVEInit();
void SAVEShutdown();
}
#endif // RIO_IS_CAFE

namespace rio {

FileDeviceMgr* FileDeviceMgr::sInstance = nullptr;

bool FileDeviceMgr::createSingleton()
{
    if (sInstance)
        return false;

    sInstance = new FileDeviceMgr();
    return true;
}

void FileDeviceMgr::destroySingleton()
{
    if (!sInstance)
        return;

    delete sInstance;
    sInstance = nullptr;
}

FileDeviceMgr::FileDeviceMgr()
    : mDeviceList()
{
#if RIO_IS_CAFE
    FSInit();
    FSAddClient(&mFSClient, FS_ERROR_FLAG_NONE);
    SAVEInit();
#endif // RIO_IS_CAFE

    mMainFileDevice = new MainFileDevice();
    mount(mMainFileDevice);

    mNativeFileDevice = new NativeFileDevice();
    mount(mNativeFileDevice);

    mDefaultFileDevice = mMainFileDevice;

#if RIO_IS_CAFE
#ifdef RIO_CAFE_MAIN_FILE_DEVICE_AS_CONTENT
    mCafeContentFileDevice = mMainFileDevice;
#else
    mCafeContentFileDevice = new ContentFileDevice();
    mount(mCafeContentFileDevice);
#endif // RIO_CAFE_MAIN_FILE_DEVICE_AS_CONTENT
#endif // RIO_IS_CAFE
}

FileDeviceMgr::~FileDeviceMgr()
{
    if (mMainFileDevice)
    {
        delete mMainFileDevice;
        mMainFileDevice = nullptr;
    }

    if (mNativeFileDevice)
    {
        delete mNativeFileDevice;
        mNativeFileDevice = nullptr;
    }

#if RIO_IS_CAFE
#ifdef RIO_CAFE_MAIN_FILE_DEVICE_AS_CONTENT
    mCafeContentFileDevice = nullptr;
#else
    if (mCafeContentFileDevice)
    {
        delete mCafeContentFileDevice;
        mCafeContentFileDevice = nullptr;
    }
#endif // RIO_CAFE_MAIN_FILE_DEVICE_AS_CONTENT

    FSDelClient(&mFSClient, FS_ERROR_FLAG_NONE);
    SAVEShutdown();
    FSShutdown();
#endif // RIO_IS_CAFE
}

void FileDeviceMgr::mount(FileDevice* device, const std::string& drive_name)
{
    RIO_ASSERT(device);

    if (!drive_name.empty())
        device->mDriveName = drive_name;

    mDeviceList.pushBack(device);
}

void FileDeviceMgr::unmount(FileDevice* device)
{
    RIO_ASSERT(device);

    if (device->mList)
        mDeviceList.erase(device);

    if (device == mDefaultFileDevice)
        mDefaultFileDevice = nullptr;
}

FileDevice*
FileDeviceMgr::findDeviceFromPath(
    const std::string& path, std::string* no_drive_path
) const
{
    std::string drive;
    FileDevice* device;

    device = Path::getDriveName(&drive, path) ? findDevice(drive)
                                              : mDefaultFileDevice;
    if (!device)
        return nullptr;

    if (no_drive_path)
        Path::getPathExceptDrive(no_drive_path, path);

    return device;
}

FileDevice*
FileDeviceMgr::findDevice(const std::string& drive) const
{
    for (FileDeviceMgr::DeviceList::iterator it = mDeviceList.begin(); it != mDeviceList.end(); ++it)
        if ((*it)->mDriveName == drive)
            return (*it);

    return nullptr;
}

FileDevice* FileDeviceMgr::tryOpen(FileHandle* handle, const std::string& filename, FileDevice::FileOpenFlag flag)
{
    std::string no_drive_path;
    FileDevice* device = findDeviceFromPath(filename, &no_drive_path);
    if (!device)
        return nullptr;

    return device->tryOpen(handle, no_drive_path, flag);
}

u8* FileDeviceMgr::tryLoad(FileDevice::LoadArg& arg)
{
    RIO_ASSERT(!arg.path.empty());

    std::string no_drive_path;
    FileDevice* device = findDeviceFromPath(arg.path, &no_drive_path);
    if (!device)
        return nullptr;

    FileDevice::LoadArg arg_(arg);
    arg_.path = no_drive_path;

    u8* ret = device->tryLoad(arg_);

    arg.read_size = arg_.read_size;
    arg.roundup_size = arg_.roundup_size;
    arg.need_unload = arg_.need_unload;

    return ret;
}

}
