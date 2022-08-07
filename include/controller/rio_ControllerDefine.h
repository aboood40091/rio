#ifndef RIO_CONTROLLER_DEFINE_H
#define RIO_CONTROLLER_DEFINE_H

#include <misc/rio_Types.h>

namespace rio {

class ControllerDefine
{
public:
    enum ControllerId
    {
        CONTROLLER_NULL,
        CONTROLLER_WIN,
        CONTROLLER_WIN_GAMEPAD,
        CONTROLLER_CAFE_REMOTE,
        CONTROLLER_CAFE_DRC,
        CONTROLLER_USER_DEFINE
    };

    enum DeviceId
    {
        DEVICE_NULL,
        DEVICE_KEYBOARD_MOUSE,
        DEVICE_CAFE_W_PAD,
        DEVICE_CAFE_V_PAD,
        DEVICE_USER_DEFINE
    };
};

}

#endif // RIO_CONTROLLER_DEFINE_H
