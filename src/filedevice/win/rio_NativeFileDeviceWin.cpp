#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <filedevice/rio_NativeFileDevice.h>

#include <misc/win/rio_Windows.h>

#include <algorithm>

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
    : StdIOFileDevice("native", GetCWD())
{
}

NativeFileDevice::NativeFileDevice(const std::string& drive_name)
    : StdIOFileDevice(drive_name, GetCWD())
{
}

}

#endif // RIO_IS_WIN
