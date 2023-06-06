#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <controller/win/rio_KeyboardMouseDeviceWin.h>
#include <gfx/rio_Window.h>

#include <misc/gl/rio_GL.h>
#include <misc/win/rio_Windows.h>

namespace rio {

KeyboardMouseDevice::KeyboardMouseDevice(ControllerMgr* mgr)
    : ControlDevice(mgr)
    , mIsLastReadSuccess(false)
{
    mId = ControllerDefine::DEVICE_KEYBOARD_MOUSE;
}

KeyboardMouseDevice::~KeyboardMouseDevice()
{
}

void KeyboardMouseDevice::calc()
{
    mIsLastReadSuccess = GetKeyboardState(mKeyState);

    f64 pos_x, pos_y;
    glfwGetCursorPos(Window::instance()->getNativeWindow().handle, &pos_x, &pos_y);
    mCursorPos.set(pos_x, pos_y);
}

}

#endif // RIO_IS_WIN
