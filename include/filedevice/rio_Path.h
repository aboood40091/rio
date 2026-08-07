#ifndef RIO_FILE_PATH_H
#define RIO_FILE_PATH_H

#include <misc/rio_Types.h>

#include <string>

namespace rio {

class Path
{
public:
    // Checks if the drive name is valid according to the rules of RIO file devices.
    static bool isValidDriveName(const std::string& drive_name);

    // Isolates the drive name in the path specified in "src" and stores it in "dst".
    static bool getDriveName(std::string* dst, const std::string& src);
    // Checks if the path specified in "src" has a drive name, and isolates it into "dst" if it's not null.
    static bool getDriveNameEx(std::string* dst, const std::string& src);
    // Removes the drive name from the path specified in "src" and stores it in "dst".
    static void getPathExceptDrive(std::string* dst, const std::string& src);

    // Returns true if the drive name is a single letter (A-Z or a-z).
    static bool isWindowsDriveLetter(const std::string& drive_name);
};

}

#endif // RIO_FILE_PATH_H
