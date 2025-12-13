#include <misc/rio_Types.h>

#if RIO_IS_DESKTOP

#include <filedevice/rio_NativeFileDevice.h>

#include <filesystem>

namespace rio {

NativeFileDevice::NativeFileDevice()
    : StdIOFileDevice("native", std::filesystem::current_path().generic_string())
{
}

NativeFileDevice::NativeFileDevice(const std::string& drive_name)
    : StdIOFileDevice(drive_name, std::filesystem::current_path().generic_string())
{
}

}

#endif // RIO_IS_DESKTOP
