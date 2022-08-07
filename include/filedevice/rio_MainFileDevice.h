#ifndef RIO_FILE_MAIN_DEVICE_H
#define RIO_FILE_MAIN_DEVICE_H

#include <misc/rio_Types.h>

#include <filedevice/rio_ContentFileDevice.h>

#if RIO_IS_CAFE && !defined(RIO_CAFE_MAIN_FILE_DEVICE_AS_CONTENT)
#include <filedevice/cafe/rio_CafeSDFileDeviceCafe.h>
#endif // RIO_IS_CAFE

namespace rio {

#if RIO_IS_CAFE && !defined(RIO_CAFE_MAIN_FILE_DEVICE_AS_CONTENT)
typedef CafeSDFileDevice    MainFileDevice;
#else
typedef ContentFileDevice   MainFileDevice;
#endif

}

#endif // RIO_FILE_MAIN_DEVICE_H
