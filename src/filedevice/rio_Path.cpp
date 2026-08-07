#include <filedevice/rio_Path.h>

namespace rio {

bool Path::isValidDriveName(const std::string& drive_name)
{
    if (drive_name.empty())
        return false;

    // Windows drive letters are reserved for the OS file device
    if (isWindowsDriveLetter(drive_name))
        return false;

    // Drive name cannot contain a colon
    if (drive_name.find(':') != std::string::npos)
        return false;

    // Drive name cannot contain a forward slash
    if (drive_name.find('/') != std::string::npos)
        return false;

    // Drive name cannot contain a backslash
    if (drive_name.find('\\') != std::string::npos)
        return false;

    return true;
}

bool Path::getDriveName(std::string* dst, const std::string& src)
{
    RIO_ASSERT(dst);

    size_t index = src.find(':');
    if (index == std::string::npos)
        return false;

    *dst = src.substr(0, index);
    return true;
}

bool Path::getDriveNameEx(std::string* dst, const std::string& src)
{
    size_t index = src.find("://");
    if (index == std::string::npos)
        return false;

    std::string drive_name = src.substr(0, index);
    if (!isValidDriveName(drive_name))
        return false;

    if (dst)
        *dst = drive_name;

    return true;
}

void Path::getPathExceptDrive(std::string* dst, const std::string& src)
{
    RIO_ASSERT(dst);

    size_t index = src.find("://");
    if (index != std::string::npos)
        *dst = src.substr(index + 3);

    else
        *dst = src;
}

bool Path::isWindowsDriveLetter(const std::string& drive_name)
{
    return drive_name.length() == 1 && ((drive_name[0] >= 'A' && drive_name[0] <= 'Z') || (drive_name[0] >= 'a' && drive_name[0] <= 'z'));
}

}
