#include <filedevice/rio_FileDevice.h>
#include <filedevice/rio_FileDeviceMgr.h>
#include <misc/rio_MemUtil.h>

namespace {

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

u8* FileDevice::tryLoad(FileDevice::LoadArg& arg)
{
    return doLoad_(arg);
}

FileDevice* FileDevice::tryOpen(FileHandle* handle, const std::string& filename, FileDevice::FileOpenFlag flag)
{
    RIO_ASSERT(handle);

    FileDevice* device = doOpen_(handle, filename, flag);
    handle->mDevice = device;
    if (device)
        handle->mOriginalDevice = this;

    return device;
}

bool FileDevice::tryClose(FileHandle* handle)
{
    RIO_ASSERT(handle);
    RIO_ASSERT(handle->mDevice == this);

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
    RIO_ASSERT(handle);
    RIO_ASSERT(handle->mDevice == this);
    RIO_ASSERT(buf);

    bool success = doRead_(read_size, handle, buf, size);
    RIO_ASSERT(!read_size || *read_size <= size);
    return success;
}

bool FileDevice::tryWrite(u32* write_size, FileHandle* handle, const u8* buf, u32 size)
{
    RIO_ASSERT(handle);
    RIO_ASSERT(handle->mDevice == this);
    RIO_ASSERT(buf);

    bool success = doWrite_(write_size, handle, buf, size);
    RIO_ASSERT(!write_size || *write_size <= size);
    return success;
}

bool FileDevice::trySeek(FileHandle* handle, s32 offset, FileDevice::SeekOrigin origin)
{
    RIO_ASSERT(handle);
    RIO_ASSERT(handle->mDevice == this);

    return doSeek_(handle, offset, origin);
}

bool FileDevice::tryGetCurrentSeekPos(u32* pos, FileHandle* handle)
{
    RIO_ASSERT(handle);
    RIO_ASSERT(handle->mDevice == this);
    RIO_ASSERT(pos);

    return doGetCurrentSeekPos_(pos, handle);
}

bool FileDevice::tryGetFileSize(u32* size, const std::string& path)
{
    RIO_ASSERT(size);

    return doGetFileSize_(size, path);
}

bool FileDevice::tryGetFileSize(u32* size, FileHandle* handle)
{
    RIO_ASSERT(handle);
    RIO_ASSERT(handle->mDevice == this);
    RIO_ASSERT(size);

    return doGetFileSize_(size, handle);
}

u8* FileDevice::doLoad_(FileDevice::LoadArg& arg)
{
    RIO_ASSERT(!arg.buffer || arg.buffer_size);
    RIO_ASSERT(arg.alignment);

    FileHandle handle;
    if (!tryOpen(&handle, arg.path, FileDevice::FILE_OPEN_FLAG_READ))
        return nullptr;

    u32 file_size = 0;
    if (!tryGetFileSize(&file_size, &handle))
        return nullptr;

    RIO_ASSERT(file_size != 0);

    u32 buffer_size = arg.buffer_size;
    if (buffer_size == 0)
        buffer_size = align(file_size, FileDevice::cBufferMinAlignment);

    else
    {
        RIO_ASSERT(buffer_size >= file_size);
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

bool FileHandle::close()
{
    RIO_ASSERT(isOpen());
    return mDevice->close(this);
}

bool FileHandle::tryClose()
{
    RIO_ASSERT(isOpen());
    return mDevice->tryClose(this);
}

u32 FileHandle::read(u8* buf, u32 size)
{
    RIO_ASSERT(isOpen());
    return mDevice->read(this, buf, size);
}

bool FileHandle::tryRead(u32* read_size, u8* buf, u32 size)
{
    RIO_ASSERT(isOpen());
    return mDevice->tryRead(read_size, this, buf, size);
}

u32 FileHandle::write(const u8* buf, u32 size)
{
    RIO_ASSERT(isOpen());
    return mDevice->write(this, buf, size);
}

bool FileHandle::tryWrite(u32* write_size, const u8* buf, u32 size)
{
    RIO_ASSERT(isOpen());
    return mDevice->tryWrite(write_size, this, buf, size);
}

bool FileHandle::seek(s32 offset, FileDevice::SeekOrigin origin)
{
    RIO_ASSERT(isOpen());
    return mDevice->seek(this, offset, origin);
}

bool FileHandle::trySeek(s32 offset, FileDevice::SeekOrigin origin)
{
    RIO_ASSERT(isOpen());
    return mDevice->trySeek(this, offset, origin);
}

u32 FileHandle::getCurrentSeekPos()
{
    RIO_ASSERT(isOpen());
    return mDevice->getCurrentSeekPos(this);
}

bool FileHandle::tryGetCurrentSeekPos(u32* pos)
{
    RIO_ASSERT(isOpen());
    return mDevice->tryGetCurrentSeekPos(pos, this);
}

u32 FileHandle::getFileSize()
{
    RIO_ASSERT(isOpen());
    return mDevice->getFileSize(this);
}

bool FileHandle::tryGetFileSize(u32* size)
{
    RIO_ASSERT(isOpen());
    return mDevice->tryGetFileSize(size, this);
}

}
