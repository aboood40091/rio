#ifndef RIO_KEYBOARD_MOUSE_DEVICE_WIN_H
#define RIO_KEYBOARD_MOUSE_DEVICE_WIN_H

#include <controller/rio_ControlDevice.h>
#include <math/rio_Vector.h>

namespace rio {

class KeyboardMouseDevice : public ControlDevice
{
public:
    KeyboardMouseDevice(ControllerMgr* mgr);
    virtual ~KeyboardMouseDevice();

    virtual void calc();

    const u8* getKeyState() const
    {
        if (mIsLastReadSuccess)
            return mKeyState;

        return nullptr;
    }

    const Vec2f& getCursorPos() const
    {
        return mCursorPos;
    }

private:
    bool    mIsLastReadSuccess;
    u8      mKeyState[256];
    Vec2f   mCursorPos;
};

}

#endif // RIO_KEYBOARD_MOUSE_DEVICE_WIN_H
