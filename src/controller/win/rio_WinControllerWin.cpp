#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <controller/win/rio_WinControllerWin.h>
#include <controller/win/rio_KeyboardMouseDeviceWin.h>
#include <controller/rio_ControllerMgr.h>
#include <misc/rio_MemUtil.h>

namespace rio {

const u8 WinController::sDefaultPadConfig[Controller::PAD_IDX_MAX] = {
    0x41, // A       -> A
    0x53, // S       -> B
    0x51, // Q       -> ZL
    0x5A, // Z       -> X
    0x58, // X       -> Y
    0x57, // W       -> ZR
    0x00, // (None)  -> R-Stick Click
    0x00, // (None)  -> L-Stick Click
    0x24, // Home    -> Home
    0x6D, // -       -> -
    0x6B, // +       -> +
    0x6B, // +       -> Start
    0x6D, // -       -> Select
    0x31, // 1       -> L
    0x32, // 2       -> R
    0x01, // L-Click -> Touch
    0x26, // Up      -> Up
    0x28, // Down    -> Down
    0x25, // Left    -> Left
    0x27, // Right   -> Right
    0x00, // (None)  -> L-Stick Up
    0x00, // (None)  -> L-Stick Down
    0x00, // (None)  -> L-Stick Left
    0x00, // (None)  -> L-Stick Right
    0x00, // (None)  -> R-Stick Up
    0x00, // (None)  -> R-Stick Down
    0x00, // (None)  -> R-Stick Left
    0x00  // (None)  -> R-Stick Right
};

WinController::WinController(ControllerMgr* mgr)
    : Controller(mgr)
{
    mId = ControllerDefine::CONTROLLER_WIN;
    setPadConfig(sDefaultPadConfig);
}

void WinController::setPadConfig(const u8* pad_config)
{
    MemUtil::copy(mPadConfig, pad_config, PAD_IDX_MAX);
}

void WinController::calcImpl_()
{
    KeyboardMouseDevice* device = static_cast<KeyboardMouseDevice*>(ControllerMgr::instance()->getControlDevice(ControllerDefine::DEVICE_KEYBOARD_MOUSE));
    if (!device)
        return;

    const u8* key_state = device->getKeyState();
    if (!key_state)
        return;

    const Vec2f& cursor_pos = device->getCursorPos();

    mPadHold.makeAllZero();

    for (u32 i = 0; i < PAD_IDX_MAX; i++)
    {
        u8 key = mPadConfig[i];
        if (key == 0)
            continue;

        if (key_state[key] & 0x80)
            mPadHold.setBit(i);
    }

    mLeftStick.set(0.0f, 0.0f);
    mRightStick.set(0.0f, 0.0f);

    mLeftAnalogTrigger = 0.0f;
    mRightAnalogTrigger = 0.0f;

    setPointer_(true, mPadHold.isOnBit(PAD_IDX_TOUCH), cursor_pos);
}

}

#endif // RIO_IS_WIN
