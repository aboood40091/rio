#ifndef RIO_FILE_PATH_H
#define RIO_FILE_PATH_H

#include <misc/rio_Types.h>

#include <string>

namespace rio {

class Path
{
public:
    // Isolates the drive name in the path specified in "src" and stores it in "dst".
    static bool getDriveName(std::string* dst, const std::string& src);
    // Removes the drive name from the path specified in "src" and stores it in "dst".
    static void getPathExceptDrive(std::string* dst, const std::string& src);
};

}

#endif // RIO_FILE_PATH_H
