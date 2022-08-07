#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <controller/win/rio_KeyboardMouseDeviceWin.h>
#include <gfx/rio_Window.h>

#include <GLFW/glfw3.h>

#include <windows.h>

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
    glfwGetCursorPos((GLFWwindow*)Window::getWindowInner(), &pos_x, &pos_y);
    mCursorPos.set(pos_x, pos_y);
}

}

#endif // RIO_IS_WIN
