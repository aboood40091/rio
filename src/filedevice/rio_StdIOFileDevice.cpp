#include <filedevice/rio_StdIOFileDevice.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>

#include <sys/stat.h>

namespace rio {

StdIOFileDevice::StdIOFileDevice(const std::string& drive_name, const std::string& cwd)
    : FileDevice(drive_name)
    , mCWD(cwd)
{
}

FileDevice*
StdIOFileDevice::doOpen_(
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

    errno = 0;
    handle_inner->handle = (uintptr_t)std::fopen(file_path.c_str(), mode);
    if (handle_inner->handle)
    {
        mLastRawError = RAW_ERROR_OK;
        return this;
    }

    RIO_ASSERT(errno != 0);

    switch (errno)
    {
  //case ECANCELED:
  //    mLastRawError = RAW_ERROR_CANCELED;
  //    break;
  //case :
  //    mLastRawError = RAW_ERROR_ALREADY_OPEN;
  //    break;
    case ENOENT:
        mLastRawError = RAW_ERROR_NOT_FOUND;
        break;
    case EISDIR:
        mLastRawError = RAW_ERROR_NOT_FILE;
        break;
    case EINVAL:
    case ENOSPC:
    case EROFS:
    case ETXTBSY:
        mLastRawError = RAW_ERROR_ACCESS_ERROR;
        break;
    case EACCES:
        {
            auto prev_errno = errno;

            struct stat st;
            auto stat_ret = stat(file_path.c_str(), &st);

            errno = prev_errno;

            if (stat_ret == 0 && ((st.st_mode & S_IFDIR) || !(st.st_mode & S_IFREG)))
                mLastRawError = RAW_ERROR_NOT_FILE;
            else
                mLastRawError = RAW_ERROR_PERMISSION_ERROR;
        }
        break;
    default:
        mLastRawError = RAW_ERROR_FATAL_ERROR;
        RIO_LOG("StdIOFileDevice::doOpen_(): Unexpected error: %s\n", std::strerror(errno));
        RIO_ASSERT(false);
    }

    return nullptr;
}

bool
StdIOFileDevice::doClose_(FileHandle* handle)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    errno = 0;
    if (std::fclose((std::FILE*)handle_inner->handle) == 0)
    {
        mLastRawError = RAW_ERROR_OK;
        return true;
    }

    RIO_ASSERT(errno != 0);

  //if (errno == ECANCELED)
  //    mLastRawError = RAW_ERROR_CANCELED;
  //
  //else
    {
        mLastRawError = RAW_ERROR_FATAL_ERROR;
        RIO_LOG("StdIOFileDevice::doClose_(): Unexpected error: %s\n", std::strerror(errno));
        RIO_ASSERT(false);
    }

    return false;
}

bool
StdIOFileDevice::doRead_(
    u32* read_size, FileHandle* handle,
    u8* buf, u32 size
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    errno = 0;
    size_t result = std::fread(buf, sizeof(u8), size, (std::FILE*)handle_inner->handle);

    if (std::ferror((std::FILE*)handle_inner->handle))
    {
        RIO_ASSERT(errno != 0);

      //if (errno == ECANCELED)
      //    mLastRawError = RAW_ERROR_CANCELED;
      //
      //else
        {
            mLastRawError = RAW_ERROR_FATAL_ERROR;
            RIO_LOG("StdIOFileDevice::doRead_(): Unexpected error: %s\n", std::strerror(errno));
            RIO_ASSERT(false);
        }

        return false;
    }

    mLastRawError = RAW_ERROR_OK;

    if (read_size)
        *read_size = result;

    return true;
}

bool
StdIOFileDevice::doWrite_(
    u32* write_size, FileHandle* handle,
    const u8* buf, u32 size
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    errno = 0;
    size_t result = std::fwrite(buf, sizeof(u8), size, (std::FILE*)handle_inner->handle);

    if (std::ferror((std::FILE*)handle_inner->handle))
    {
        RIO_ASSERT(errno != 0);

        switch (errno)
        {
      //case ECANCELED:
      //    mLastRawError = RAW_ERROR_CANCELED;
      //    break;
        case EFBIG:
            mLastRawError = RAW_ERROR_FILE_TOO_BIG;
            break;
        case ENOSPC:
            mLastRawError = RAW_ERROR_STORAGE_FULL;
            break;
        default:
            mLastRawError = RAW_ERROR_FATAL_ERROR;
            RIO_LOG("StdIOFileDevice::doWrite_(): Unexpected error: %s\n", std::strerror(errno));
            RIO_ASSERT(false);
            break;
        }

        return false;
    }

    mLastRawError = RAW_ERROR_OK;

    if (write_size)
        *write_size = result;

    return true;
}

bool
StdIOFileDevice::doSeek_(
    FileHandle* handle, s32 offset, FileDevice::SeekOrigin origin
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    int std_origin;

    switch (origin)
    {
    case FileDevice::SEEK_ORIGIN_BEGIN:
        std_origin = SEEK_SET;
        break;
    case FileDevice::SEEK_ORIGIN_CURRENT:
        std_origin = SEEK_CUR;
        break;
    case FileDevice::SEEK_ORIGIN_END:
        std_origin = SEEK_END;
        break;
    default:
        return false;
    }

    errno = 0;
    if (std::fseek((std::FILE*)handle_inner->handle, offset, std_origin) == 0)
    {
        mLastRawError = RAW_ERROR_OK;
        return true;
    }

    RIO_ASSERT(errno != 0);

  //if (errno == ECANCELED)
  //    mLastRawError = RAW_ERROR_CANCELED;
  //
  //else
    {
        mLastRawError = RAW_ERROR_FATAL_ERROR;
        RIO_LOG("StdIOFileDevice::doSeek_(): Unexpected error: %s\n", std::strerror(errno));
        RIO_ASSERT(false);
    }

    return false;
}

bool
StdIOFileDevice::doGetCurrentSeekPos_(
    u32* pos, FileHandle* handle
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    errno = 0;
    long result = std::ftell((std::FILE*)handle_inner->handle);
    if (result == -1L)
    {
        RIO_ASSERT(errno != 0);

      //if (errno == ECANCELED)
      //    mLastRawError = RAW_ERROR_CANCELED;
      //
      //else
        {
            mLastRawError = RAW_ERROR_FATAL_ERROR;
            RIO_LOG("StdIOFileDevice::doSeek_(): Unexpected error: %s\n", std::strerror(errno));
            RIO_ASSERT(false);
        }

        return false;
    }

    mLastRawError = RAW_ERROR_OK;
    *pos = result;
    return true;
}

bool
StdIOFileDevice::doGetFileSize_(
    u32* size, const std::string& path
)
{
    std::string file_path = getNativePath(path);

    struct stat st;
    errno = 0;
    if (stat(file_path.c_str(), &st) == 0)
    {
        mLastRawError = RAW_ERROR_OK;
        *size = st.st_size;
        return true;
    }

    RIO_ASSERT(errno != 0);

  //if (errno == ECANCELED)
  //    mLastRawError = RAW_ERROR_CANCELED;
  //
  //else
    {
        mLastRawError = RAW_ERROR_FATAL_ERROR;
        RIO_LOG("StdIOFileDevice::doGetFileSize_(): Unexpected error: %s\n", std::strerror(errno));
        RIO_ASSERT(false);
    }

    return false;
}

bool
StdIOFileDevice::doGetFileSize_(
    u32* size, FileHandle* handle
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    int fd;
    struct stat st;

    if ((errno = 0, (fd = fileno((std::FILE*)handle_inner->handle)) == -1) ||
        (errno = 0, fstat(fd, &st)))
    {
        RIO_ASSERT(errno != 0);

      //if (errno == ECANCELED)
      //    mLastRawError = RAW_ERROR_CANCELED;
      //
      //else
        {
            mLastRawError = RAW_ERROR_FATAL_ERROR;
            RIO_LOG("StdIOFileDevice::doGetFileSize_(): Unexpected error: %s\n", std::strerror(errno));
            RIO_ASSERT(false);
        }

        return false;
    }

    mLastRawError = RAW_ERROR_OK;
    *size = st.st_size;
    return true;
}

bool
StdIOFileDevice::doIsExistFile_(
    bool* is_exist, const std::string& path
)
{
    std::string file_path = getNativePath(path);

    struct stat st;
    errno = 0;
    if (stat(file_path.c_str(), &st) == 0)
    {
        mLastRawError = RAW_ERROR_OK;
        *is_exist = !(st.st_mode & S_IFDIR) && (st.st_mode & S_IFREG);
        return true;
    }

    RIO_ASSERT(errno != 0);

    switch (errno)
    {
  //case ECANCELED:
  //    mLastRawError = RAW_ERROR_CANCELED;
  //    break;
    case ENOENT:
        mLastRawError = RAW_ERROR_NOT_FOUND;
        *is_exist = false;
        return true;
    case EACCES:
        mLastRawError = RAW_ERROR_PERMISSION_ERROR;
        break;
    default:
        mLastRawError = RAW_ERROR_FATAL_ERROR;
        RIO_LOG("StdIOFileDevice::doIsExistFile_(): Unexpected error: %s\n", std::strerror(errno));
        RIO_ASSERT(false);
        break;
    }

    return false;
}

RawErrorCode StdIOFileDevice::doGetLastRawError_() const
{
    return mLastRawError;
}

}
