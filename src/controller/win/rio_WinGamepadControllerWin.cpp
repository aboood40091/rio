#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <controller/win/rio_WinGamepadControllerWin.h>

namespace rio {

WinGamepadController::WinGamepadController(ControllerMgr* mgr, s32 channel)
    : Controller(mgr)
    , mIsConnected(true)
    , mChannel(channel)
    , mGamepadName(nullptr)
{
    RIO_ASSERT(channel <= GLFW_JOYSTICK_LAST);
    mId = ControllerDefine::CONTROLLER_WIN_GAMEPAD;
}

void WinGamepadController::calcImpl_()
{
    if (!glfwJoystickIsGamepad(mChannel) || !glfwGetGamepadState(mChannel, &mState))
    {
        mIsConnected = false;
        mGamepadName = nullptr;
        return;
    }

    mIsConnected = true;
    if (!mGamepadName)
        mGamepadName = glfwGetGamepadName(mChannel);

    mPadHold.makeAllZero();

    // XBOX-like Controller -> RIO Controller

    mPadHold.change(1 << Controller::PAD_IDX_A,      mState.buttons[GLFW_GAMEPAD_BUTTON_B]);
    mPadHold.change(1 << Controller::PAD_IDX_B,      mState.buttons[GLFW_GAMEPAD_BUTTON_A]);
    mPadHold.change(1 << Controller::PAD_IDX_X,      mState.buttons[GLFW_GAMEPAD_BUTTON_Y]);
    mPadHold.change(1 << Controller::PAD_IDX_Y,      mState.buttons[GLFW_GAMEPAD_BUTTON_X]);
    mPadHold.change(1 << Controller::PAD_IDX_PLUS |
                    1 << Controller::PAD_IDX_START,  mState.buttons[GLFW_GAMEPAD_BUTTON_START]);
    mPadHold.change(1 << Controller::PAD_IDX_MINUS |
                    1 << Controller::PAD_IDX_SELECT, mState.buttons[GLFW_GAMEPAD_BUTTON_BACK]);
    mPadHold.change(1 << Controller::PAD_IDX_L,      mState.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]);
    mPadHold.change(1 << Controller::PAD_IDX_R,      mState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]);
    mPadHold.change(1 << Controller::PAD_IDX_HOME,   mState.buttons[GLFW_GAMEPAD_BUTTON_GUIDE]);
    mPadHold.change(1 << Controller::PAD_IDX_UP,     mState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]);
    mPadHold.change(1 << Controller::PAD_IDX_DOWN,   mState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]);
    mPadHold.change(1 << Controller::PAD_IDX_LEFT,   mState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT]);
    mPadHold.change(1 << Controller::PAD_IDX_RIGHT,  mState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT]);
    mPadHold.change(1 << Controller::PAD_IDX_2,      mState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB]);
    mPadHold.change(1 << Controller::PAD_IDX_1,      mState.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB]);

    mPadHold.change(1 << Controller::PAD_IDX_ZL,     mState.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] >= 0.0f);
    mPadHold.change(1 << Controller::PAD_IDX_ZR,     mState.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] >= 0.0f);

    mLeftStick.set(mState.axes[GLFW_GAMEPAD_AXIS_LEFT_X], -mState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
    mRightStick.set(mState.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], -mState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);

    mLeftAnalogTrigger = 0.0f;
    mRightAnalogTrigger = 0.0f;

    setPointer_(false, false, { 0.0f, 0.0f });
}

}

#endif // RIO_IS_WIN
