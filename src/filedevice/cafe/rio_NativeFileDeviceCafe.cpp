#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <filedevice/rio_NativeFileDevice.h>
#include <filedevice/rio_FileDeviceMgr.h>

namespace rio {

NativeFileDevice::NativeFileDevice()
    : FileDevice("native")
    , mCWD(".")
{
}

NativeFileDevice::NativeFileDevice(const std::string& drive_name)
    : FileDevice(drive_name)
    , mCWD(".")
{
}

FileDevice*
NativeFileDevice::doOpen_(
    FileHandle* handle, const std::string& filename,
    FileDevice::FileOpenFlag flag
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    const char* mode = "r";
    switch (flag)
    {
    case FileDevice::FILE_OPEN_FLAG_READ:
        mode = "r";
        break;
    case FileDevice::FILE_OPEN_FLAG_WRITE:
        mode = "w";
        break;
    case FileDevice::FILE_OPEN_FLAG_READ_WRITE:
        mode = "r+";
        break;
    case FileDevice::FILE_OPEN_FLAG_CREATE:
        mode = "w+";
        break;
    default:
        RIO_ASSERT(false);
    }

    std::string file_path = getNativePath(filename);

    FSStatus status = FSOpenFile(client, &block, file_path.c_str(), mode, &handle_inner->handle, FS_ERROR_FLAG_NONE);
    handle_inner->position = 0;

    if (status != FS_STATUS_OK)
    {
        handle_inner->handle = 0;
        return nullptr;
    }

    return this;
}

bool
NativeFileDevice::doClose_(FileHandle* handle)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    FSStatus status = FSCloseFile(client, &block, handle_inner->handle, FS_ERROR_FLAG_NONE);
    if (status != FS_STATUS_OK)
        return false;

    return true;
}

bool
NativeFileDevice::doRead_(
    u32* read_size, FileHandle* handle,
    u8* buf, u32 size
)
{
    RIO_ASSERT(((uintptr_t)buf % 64u) == 0);

    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    s32 result = FSReadFile(client, &block, buf, sizeof(u8), size, handle_inner->handle, 0, FS_ERROR_FLAG_NONE);
    if (result >= FS_STATUS_OK)
    {
        handle_inner->position += result;

        if (read_size)
            *read_size = result;

        return true;
    }

    return false;
}

bool
NativeFileDevice::doWrite_(
    u32* write_size, FileHandle* handle,
    const u8* buf, u32 size
)
{
    RIO_ASSERT(((uintptr_t)buf % 64u) == 0);

    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    s32 result = FSWriteFile(client, &block, (u8*)buf, sizeof(u8), size, handle_inner->handle, 0, FS_ERROR_FLAG_NONE);
    if (result >= FS_STATUS_OK)
    {
        handle_inner->position += result;

        if (write_size)
            *write_size = result;

        return true;
    }

    return false;
}

bool
NativeFileDevice::doSeek_(
    FileHandle* handle, s32 offset, FileDevice::SeekOrigin origin
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    switch (origin)
    {
    case FileDevice::SEEK_ORIGIN_BEGIN:
        break;
    case FileDevice::SEEK_ORIGIN_CURRENT:
        offset += handle_inner->position;
        break;
    case FileDevice::SEEK_ORIGIN_END:
        RIO_ASSERT(offset <= 0);
        {
            u32 size = 0;
            if (doGetFileSize_(&size, handle))
                offset += size;

            else
                return false;
        }
        break;
    default:
        return false;
    }

    FSStatus status = FSSetPosFile(client, &block, handle_inner->handle, offset, FS_ERROR_FLAG_NONE);
    if (status == FS_STATUS_OK)
    {
        handle_inner->position = offset;
        return true;
    }

    return false;
}

bool
NativeFileDevice::doGetCurrentSeekPos_(
    u32* pos, FileHandle* handle
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);
    *pos = handle_inner->position;
    return true;
}

bool
NativeFileDevice::doGetFileSize_(
    u32* size, const std::string& path
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();

    std::string file_path = getNativePath(path);

    FSStat stat;
    FSStatus status = FSGetStat(client, &block, file_path.c_str(), &stat, FS_ERROR_FLAG_NONE);

    if (status != FS_STATUS_OK)
        return false;

    *size = stat.size;
    return true;
}

bool
NativeFileDevice::doGetFileSize_(
    u32* size, FileHandle* handle
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    FSStat stat;
    FSStatus status = FSGetStatFile(client, &block, handle_inner->handle, &stat, FS_ERROR_FLAG_NONE);

    if (status != FS_STATUS_OK)
        return false;

    *size = stat.size;
    return true;
}

FSClient*
NativeFileDevice::getUsableFSClient_() const
{
    if (!mFSClient)
        return FileDeviceMgr::instance()->getFSClient();

    return mFSClient;
}

}

#endif // RIO_IS_CAFE
