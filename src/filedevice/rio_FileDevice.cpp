#include <filedevice/rio_FileDevice.h>
#include <filedevice/rio_FileDeviceMgr.h>
#include <misc/rio_MemUtil.h>

namespace {

static inline u32 max(u32 x, u32 y)
{
    if (x >= y)
        return x;

    return y;
}

static inline u32 align(u32 x, u32 y)
{
    RIO_ASSERT(((y - 1) & y) == 0);
    return (x + y - 1) & -y;
}

}

namespace rio {

FileDevice::~FileDevice()
{
    if (FileDeviceMgr::instance())
        FileDeviceMgr::instance()->unmount(this);
}

RawErrorCode FileDevice::getLastRawError() const
{
    return doGetLastRawError_();
}

u8* FileDevice::tryLoad(FileDevice::LoadArg& arg)
{
    return doLoad_(arg);
}

FileDevice* FileDevice::tryOpen(FileHandle* handle, const std::string& filename, FileDevice::FileOpenFlag flag)
{
    if (handle == nullptr)
    {
        RIO_LOG("FileDevice::tryOpen(): handle is null.\n");
        RIO_ASSERT(false);
        return nullptr;
    }

    FileDevice* device = doOpen_(handle, filename, flag);
    handle->mDevice = device;
    if (device)
        handle->mOriginalDevice = this;

    return device;
}

bool FileDevice::tryClose(FileHandle* handle)
{
    if (handle == nullptr)
    {
        RIO_LOG("FileDevice::tryClose(): handle is null.\n");
        RIO_ASSERT(false);
        return false;
    }

    if (handle->mDevice != this)
    {
        RIO_LOG("FileDevice::tryClose(): handle device miss match.\n");
        RIO_ASSERT(false);
        return false;
    }

    bool success = doClose_(handle);
    if (success)
    {
        handle->mDevice = nullptr;
        handle->mOriginalDevice = nullptr;
    }

    return success;
}

bool FileDevice::tryRead(u32* read_size, FileHandle* handle, u8* buf, u32 size)
{
    if (handle == nullptr)
    {
        RIO_LOG("FileDevice::tryRead(): handle is null.\n");
        RIO_ASSERT(false);
        return false;
    }

    if (handle->mDevice != this)
    {
        RIO_LOG("FileDevice::tryRead(): handle device miss match.\n");
        RIO_ASSERT(false);
        return false;
    }

    if (buf == nullptr)
    {
        RIO_LOG("FileDevice::tryRead(): buf is null.\n");
        RIO_ASSERT(false);
        return false;
    }

    bool success = doRead_(read_size, handle, buf, size);
    RIO_ASSERT(!read_size || *read_size <= size);
    return success;
}

bool FileDevice::tryWrite(u32* write_size, FileHandle* handle, const u8* buf, u32 size)
{
    if (handle == nullptr)
    {
        RIO_LOG("FileDevice::tryWrite(): handle is null.\n");
        RIO_ASSERT(false);
        return false;
    }

    if (handle->mDevice != this)
    {
        RIO_LOG("FileDevice::tryWrite(): handle device miss match.\n");
        RIO_ASSERT(false);
        return false;
    }

    if (buf == nullptr)
    {
        RIO_LOG("FileDevice::tryWrite(): buf is null.\n");
        RIO_ASSERT(false);
        return false;
    }

    bool success = doWrite_(write_size, handle, buf, size);
    RIO_ASSERT(!write_size || *write_size <= size);
    return success;
}

bool FileDevice::trySeek(FileHandle* handle, s32 offset, FileDevice::SeekOrigin origin)
{
    if (handle == nullptr)
    {
        RIO_LOG("FileDevice::trySeek(): handle is null.\n");
        RIO_ASSERT(false);
        return false;
    }

    if (handle->mDevice != this)
    {
        RIO_LOG("FileDevice::trySeek(): handle device miss match.\n");
        RIO_ASSERT(false);
        return false;
    }

    return doSeek_(handle, offset, origin);
}

bool FileDevice::tryGetCurrentSeekPos(u32* pos, FileHandle* handle)
{
    if (handle == nullptr)
    {
        RIO_LOG("FileDevice::tryGetCurrentSeekPos(): handle is null.\n");
        RIO_ASSERT(false);
        return false;
    }

    if (handle->mDevice != this)
    {
        RIO_LOG("FileDevice::tryGetCurrentSeekPos(): handle device miss match.\n");
        RIO_ASSERT(false);
        return false;
    }

    if (pos == nullptr)
    {
        RIO_LOG("FileDevice::tryGetCurrentSeekPos(): pos is null.\n");
        RIO_ASSERT(false);
        return false;
    }

    return doGetCurrentSeekPos_(pos, handle);
}

bool FileDevice::tryGetFileSize(u32* size, const std::string& path)
{
    if (size == nullptr)
    {
        RIO_LOG("FileDevice::tryGetFileSize(): size is null.\n");
        RIO_ASSERT(false);
        return false;
    }

    return doGetFileSize_(size, path);
}

bool FileDevice::tryGetFileSize(u32* size, FileHandle* handle)
{
    if (handle == nullptr)
    {
        RIO_LOG("FileDevice::tryGetFileSize(): handle is null.\n");
        RIO_ASSERT(false);
        return false;
    }

    if (handle->mDevice != this)
    {
        RIO_LOG("FileDevice::tryGetFileSize(): handle device miss match.\n");
        RIO_ASSERT(false);
        return false;
    }

    if (size == nullptr)
    {
        RIO_LOG("FileDevice::tryGetFileSize(): size is null.\n");
        RIO_ASSERT(false);
        return false;
    }

    return doGetFileSize_(size, handle);
}

bool FileDevice::tryIsExistFile(bool* is_exist, const std::string& path)
{
    if (is_exist == nullptr)
    {
        RIO_LOG("FileDevice::tryIsExistFile(): is_exist is null.\n");
        RIO_ASSERT(false);
        return false;
    }

    return doIsExistFile_(is_exist, path);
}

u8* FileDevice::doLoad_(FileDevice::LoadArg& arg)
{
    if (arg.buffer && arg.buffer_size == 0)
    {
        RIO_LOG("FileDevice::doLoad_(): arg.buffer is specified, but arg.buffer_size is zero.\n");
        return nullptr;
    }

    arg.alignment = max(arg.alignment, 1);

    FileHandle handle;
    if (!tryOpen(&handle, arg.path, FileDevice::FILE_OPEN_FLAG_READ))
        return nullptr;

    u32 file_size = 0;
    if (!tryGetFileSize(&file_size, &handle))
        return nullptr;

    if (file_size == 0)
    {
        RIO_ASSERT(false);
        return nullptr;
    }

    u32 buffer_size = arg.buffer_size;
    if (buffer_size == 0)
        buffer_size = align(file_size, FileDevice::cBufferMinAlignment);

    else if (buffer_size < file_size)
    {
        RIO_LOG("FileDevice::doLoad_(): arg.buffer_size[%u] is smaller than file size[%u].\n", buffer_size, file_size);
        return nullptr;
    }

    u8* buffer = arg.buffer;
    bool need_unload = false;

    if (!buffer)
    {
        buffer = (u8*)MemUtil::alloc(buffer_size, align(arg.alignment, FileDevice::cBufferMinAlignment));
        need_unload = true;
    }

    u32 read_size = 0;
    if (!tryRead(&read_size, &handle, buffer, buffer_size))
    {
        if (need_unload)
            MemUtil::free(buffer);

        return nullptr;
    }

    if (!tryClose(&handle))
    {
        if (need_unload)
            MemUtil::free(buffer);

        return nullptr;
    }

    arg.read_size = read_size;
    arg.roundup_size = buffer_size;
    arg.need_unload = need_unload;

    return buffer;
}

void FileHandle::close()
{
    if (!isOpen())
    {
        RIO_LOG("FileHandle::close(): Handle not opened.\n");
        RIO_ASSERT(false);
        return;
    }

    mDevice->close(this);
}

bool FileHandle::tryClose()
{
    if (!isOpen())
    {
        RIO_LOG("FileHandle::tryClose(): Handle not opened.\n");
        RIO_ASSERT(false);
        return false;
    }

    return mDevice->tryClose(this);
}

u32 FileHandle::read(u8* buf, u32 size)
{
    if (!isOpen())
    {
        RIO_LOG("FileHandle::read(): Handle not opened.\n");
        RIO_ASSERT(false);
        return 0;
    }

    return mDevice->read(this, buf, size);
}

bool FileHandle::tryRead(u32* read_size, u8* buf, u32 size)
{
    if (!isOpen())
    {
        RIO_LOG("FileHandle::tryRead(): Handle not opened.\n");
        RIO_ASSERT(false);
        return false;
    }

    return mDevice->tryRead(read_size, this, buf, size);
}

u32 FileHandle::write(const u8* buf, u32 size)
{
    if (!isOpen())
    {
        RIO_LOG("FileHandle::write(): Handle not opened.\n");
        RIO_ASSERT(false);
        return 0;
    }

    return mDevice->write(this, buf, size);
}

bool FileHandle::tryWrite(u32* write_size, const u8* buf, u32 size)
{
    if (!isOpen())
    {
        RIO_LOG("FileHandle::tryWrite(): Handle not opened.\n");
        RIO_ASSERT(false);
        return false;
    }

    return mDevice->tryWrite(write_size, this, buf, size);
}

void FileHandle::seek(s32 offset, FileDevice::SeekOrigin origin)
{
    RIO_ASSERT(isOpen());
    mDevice->seek(this, offset, origin);
}

bool FileHandle::trySeek(s32 offset, FileDevice::SeekOrigin origin)
{
    if (!isOpen())
    {
        RIO_LOG("FileHandle::trySeek(): Handle not opened.\n");
        RIO_ASSERT(false);
        return false;
    }

    return mDevice->trySeek(this, offset, origin);
}

u32 FileHandle::getCurrentSeekPos()
{
    if (!isOpen())
    {
        RIO_LOG("FileHandle::getCurrentSeekPos(): Handle not opened.\n");
        RIO_ASSERT(false);
        return 0;
    }

    return mDevice->getCurrentSeekPos(this);
}

bool FileHandle::tryGetCurrentSeekPos(u32* pos)
{
    if (!isOpen())
    {
        RIO_LOG("FileHandle::tryGetCurrentSeekPos(): Handle not opened.\n");
        RIO_ASSERT(false);
        return false;
    }

    return mDevice->tryGetCurrentSeekPos(pos, this);
}

u32 FileHandle::getFileSize()
{
    if (!isOpen())
    {
        RIO_LOG("FileHandle::getFileSize(): Handle not opened.\n");
        RIO_ASSERT(false);
        return 0;
    }

    return mDevice->getFileSize(this);
}

bool FileHandle::tryGetFileSize(u32* size)
{
    if (!isOpen())
    {
        RIO_LOG("FileHandle::tryGetFileSize(): Handle not opened.\n");
        RIO_ASSERT(false);
        return false;
    }

    return mDevice->tryGetFileSize(size, this);
}

}
