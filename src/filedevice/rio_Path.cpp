#include <filedevice/rio_Path.h>

namespace rio {

bool Path::getDriveName(std::string* dst, const std::string& src)
{
    RIO_ASSERT(dst);

    size_t index = src.find(':');
    if (index == std::string::npos)
        return false;

    *dst = src.substr(0, index);
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

}
