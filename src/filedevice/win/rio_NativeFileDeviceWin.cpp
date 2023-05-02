#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <filedevice/rio_NativeFileDevice.h>

#include <misc/win/rio_Windows.h>

#include <algorithm>
#include <cstdio>

#include <sys/stat.h>

namespace {

static inline std::string GetCWD()
{
    std::string path = std::string(MAX_PATH, '\0');
    GetModuleFileNameA(nullptr, &path[0], path.length());
    std::replace(path.begin(), path.end(), '\\', '/');
    return path.substr(0, path.find_last_of('/'));
}

}

namespace rio {

NativeFileDevice::NativeFileDevice()
    : FileDevice("native")
    , mCWD(GetCWD())
{
}

NativeFileDevice::NativeFileDevice(const std::string& drive_name)
    : FileDevice(drive_name)
    , mCWD(GetCWD())
{
}

FileDevice*
NativeFileDevice::doOpen_(
    FileHandle* handle, const std::string& filename,
    FileDevice::FileOpenFlag flag
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    const char* mode = "rb";
    switch (flag)
    {
    case FileDevice::FILE_OPEN_FLAG_READ:
        mode = "rb";
        break;
    case FileDevice::FILE_OPEN_FLAG_WRITE:
        mode = "wb";
        break;
    case FileDevice::FILE_OPEN_FLAG_READ_WRITE:
        mode = "rb+";
        break;
    case FileDevice::FILE_OPEN_FLAG_CREATE:
        mode = "wb+";
        break;
    default:
        RIO_ASSERT(false);
    }

    std::string file_path = getNativePath(filename);

    handle_inner->handle = (uintptr_t)std::fopen(file_path.c_str(), mode);
    return handle_inner->handle ? this : nullptr;
}

bool
NativeFileDevice::doClose_(FileHandle* handle)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);
    return std::fclose((std::FILE*)handle_inner->handle) == 0;
}

bool
NativeFileDevice::doRead_(
    u32* read_size, FileHandle* handle,
    u8* buf, u32 size
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);
    size_t result = std::fread(buf, sizeof(u8), size, (std::FILE*)handle_inner->handle);

    if (std::ferror((std::FILE*)handle_inner->handle))
        return false;

    if (read_size)
        *read_size = result;

    return true;
}

bool
NativeFileDevice::doWrite_(
    u32* write_size, FileHandle* handle,
    const u8* buf, u32 size
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);
    size_t result = std::fwrite(buf, sizeof(u8), size, (std::FILE*)handle_inner->handle);

    if (std::ferror((std::FILE*)handle_inner->handle))
        return false;

    if (write_size)
        *write_size = result;

    return true;
}

bool
NativeFileDevice::doSeek_(
    FileHandle* handle, s32 offset, FileDevice::SeekOrigin origin
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    switch (origin)
    {
    case FileDevice::SEEK_ORIGIN_BEGIN:
        return std::fseek((std::FILE*)handle_inner->handle, offset, SEEK_SET) == 0;
    case FileDevice::SEEK_ORIGIN_CURRENT:
        return std::fseek((std::FILE*)handle_inner->handle, offset, SEEK_CUR) == 0;
    case FileDevice::SEEK_ORIGIN_END:
        return std::fseek((std::FILE*)handle_inner->handle, offset, SEEK_END) == 0;
    default:
        return false;
    }
}

bool
NativeFileDevice::doGetCurrentSeekPos_(
    u32* pos, FileHandle* handle
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);
    long result = std::ftell((std::FILE*)handle_inner->handle);
    if (result == -1L)
        return false;

    *pos = result;
    return true;
}

bool
NativeFileDevice::doGetFileSize_(
    u32* size, const std::string& path
)
{
    std::string file_path = getNativePath(path);

    struct stat st;
    if (stat(file_path.c_str(), &st))
        return false;

    *size = st.st_size;
    return true;
}

bool
NativeFileDevice::doGetFileSize_(
    u32* size, FileHandle* handle
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);
    int fd = fileno((std::FILE*)handle_inner->handle);
    if (fd == -1)
        return false;


    struct stat st;
    if (fstat(fd, &st))
        return false;

    *size = st.st_size;
    return true;
}

}

#endif // RIO_IS_WIN
