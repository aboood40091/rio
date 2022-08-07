#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <controller/cafe/rio_CafeRemoteControllerCafe.h>
#include <controller/cafe/rio_CafeWPadDeviceCafe.h>
#include <controller/rio_ControllerMgr.h>

namespace rio {

CafeRemoteController::CafeRemoteController(ControllerMgr* mgr, s32 channel)
    : Controller(mgr)
    , mChannel(channel)
    , mDeviceTypeInternal(253)
    , mDeviceType(253)
    , mIsConnected(false)
    , mIsEnableCL(false)
{
    mId = ControllerDefine::CONTROLLER_CAFE_REMOTE;
}

void CafeRemoteController::calcImpl_()
{
    CafeWPadDevice* device = static_cast<CafeWPadDevice*>(ControllerMgr::instance()->getControlDevice(ControllerDefine::DEVICE_CAFE_W_PAD));
    if (!device)
        return;

    const CafeWPadDevice::KPadInfo& k_pad_info = device->getKPadInfo(mChannel);

    if (k_pad_info.last_read_length > 0 || k_pad_info.last_read_error == KPAD_ERROR_NO_SAMPLES)
    {
        const KPADStatus& status = k_pad_info.status[0];
        if (status.error == -4)
            return;

        if (status.extensionType != 253)
        {
            mPadHold.makeAllZero();

            if (status.extensionType == WPAD_EXT_PRO_CONTROLLER)
            {
                u32 hold = status.pro.hold;

                mPadHold.change(1 << Controller::PAD_IDX_A,      hold & WPAD_PRO_BUTTON_A);
                mPadHold.change(1 << Controller::PAD_IDX_B,      hold & WPAD_PRO_BUTTON_B);
                mPadHold.change(1 << Controller::PAD_IDX_X,      hold & WPAD_PRO_BUTTON_X);
                mPadHold.change(1 << Controller::PAD_IDX_Y,      hold & WPAD_PRO_BUTTON_Y);
                mPadHold.change(1 << Controller::PAD_IDX_L,      hold & WPAD_PRO_TRIGGER_L);
                mPadHold.change(1 << Controller::PAD_IDX_R,      hold & WPAD_PRO_TRIGGER_R);
                mPadHold.change(1 << Controller::PAD_IDX_C,      hold & WPAD_PRO_TRIGGER_ZL);
                mPadHold.change(1 << Controller::PAD_IDX_Z,      hold & WPAD_PRO_TRIGGER_ZR);
                mPadHold.change(1 << Controller::PAD_IDX_PLUS |
                                1 << Controller::PAD_IDX_START,  hold & WPAD_PRO_BUTTON_PLUS);
                mPadHold.change(1 << Controller::PAD_IDX_MINUS |
                                1 << Controller::PAD_IDX_SELECT, hold & WPAD_PRO_BUTTON_MINUS);
                mPadHold.change(1 << Controller::PAD_IDX_HOME,   hold & WPAD_PRO_BUTTON_HOME);
                mPadHold.change(1 << Controller::PAD_IDX_RIGHT,  hold & WPAD_PRO_BUTTON_RIGHT);
                mPadHold.change(1 << Controller::PAD_IDX_LEFT,   hold & WPAD_PRO_BUTTON_LEFT);
                mPadHold.change(1 << Controller::PAD_IDX_UP,     hold & WPAD_PRO_BUTTON_UP);
                mPadHold.change(1 << Controller::PAD_IDX_DOWN,   hold & WPAD_PRO_BUTTON_DOWN);
                mPadHold.change(1 << Controller::PAD_IDX_1,      hold & WPAD_PRO_BUTTON_STICK_L);
                mPadHold.change(1 << Controller::PAD_IDX_2,      hold & WPAD_PRO_BUTTON_STICK_R);

                mLeftStick.set(status.pro.leftStick.x, status.pro.leftStick.y);
                mRightStick.set(status.pro.rightStick.x, status.pro.rightStick.y);

                setPointer_(false, false, { 0.0f, 0.0f });
            }
            else
            {
                u32 hold = status.hold;

                mPadHold.change(1 << Controller::PAD_IDX_A,      hold & WPAD_BUTTON_A);
                mPadHold.change(1 << Controller::PAD_IDX_B,      hold & WPAD_BUTTON_B);
                mPadHold.change(1 << Controller::PAD_IDX_1,      hold & WPAD_BUTTON_1);
                mPadHold.change(1 << Controller::PAD_IDX_2,      hold & WPAD_BUTTON_2);
                mPadHold.change(1 << Controller::PAD_IDX_Z,      hold & WPAD_BUTTON_Z);
                mPadHold.change(1 << Controller::PAD_IDX_C,      hold & WPAD_BUTTON_C);
                mPadHold.change(1 << Controller::PAD_IDX_PLUS |
                                1 << Controller::PAD_IDX_START,  hold & WPAD_BUTTON_PLUS);
                mPadHold.change(1 << Controller::PAD_IDX_MINUS |
                                1 << Controller::PAD_IDX_SELECT, hold & WPAD_BUTTON_MINUS);
                mPadHold.change(1 << Controller::PAD_IDX_HOME,   hold & WPAD_BUTTON_HOME);
                mPadHold.change(1 << Controller::PAD_IDX_RIGHT,  hold & WPAD_BUTTON_RIGHT);
                mPadHold.change(1 << Controller::PAD_IDX_LEFT,   hold & WPAD_BUTTON_LEFT);
                mPadHold.change(1 << Controller::PAD_IDX_UP,     hold & WPAD_BUTTON_UP);
                mPadHold.change(1 << Controller::PAD_IDX_DOWN,   hold & WPAD_BUTTON_DOWN);

                if (status.extensionType == WPAD_EXT_NUNCHUK || status.extensionType == WPAD_EXT_MPLUS_NUNCHUK)
                    mLeftStick.set(status.nunchuck.stick.x, status.nunchuck.stick.y);
                else
                    mLeftStick.set(0.0f, 0.0f);

                mRightStick.set(0.0f, 0.0f);

                setPointer_(status.posValid > 0, hold & WPAD_BUTTON_A, Vector2f{ status.pos.x, status.pos.y });

                if (mIsEnableCL && (status.extensionType == WPAD_EXT_CLASSIC || status.extensionType == WPAD_EXT_MPLUS_CLASSIC))
                {
                    u32 cl_hold = status.classic.hold;

                    if (cl_hold & WPAD_CLASSIC_BUTTON_A)     mPadHold.set(1 << Controller::PAD_IDX_A);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_B)     mPadHold.set(1 << Controller::PAD_IDX_B);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_X)     mPadHold.set(1 << Controller::PAD_IDX_X);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_Y)     mPadHold.set(1 << Controller::PAD_IDX_Y);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_PLUS)  mPadHold.set(1 << Controller::PAD_IDX_PLUS |
                                                                          1 << Controller::PAD_IDX_START);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_MINUS) mPadHold.set(1 << Controller::PAD_IDX_MINUS |
                                                                          1 << Controller::PAD_IDX_SELECT);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_L)     mPadHold.set(1 << Controller::PAD_IDX_L);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_R)     mPadHold.set(1 << Controller::PAD_IDX_R);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_ZL)    mPadHold.set(1 << Controller::PAD_IDX_C);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_ZR)    mPadHold.set(1 << Controller::PAD_IDX_Z);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_HOME)  mPadHold.set(1 << Controller::PAD_IDX_HOME);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_UP)    mPadHold.set(1 << Controller::PAD_IDX_UP);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_DOWN)  mPadHold.set(1 << Controller::PAD_IDX_DOWN);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_LEFT)  mPadHold.set(1 << Controller::PAD_IDX_LEFT);
                    if (cl_hold & WPAD_CLASSIC_BUTTON_RIGHT) mPadHold.set(1 << Controller::PAD_IDX_RIGHT);

                    mLeftStick.set(status.classic.leftStick.x, status.classic.leftStick.y);
                    mRightStick.set(status.classic.rightStick.x, status.classic.rightStick.y);
                }
            }

            mLeftAnalogTrigger = 0.0f;
            mRightAnalogTrigger = 0.0f;
            mIsConnected = true;
            mDeviceTypeInternal = status.extensionType;
        }
        else
        {
            mIsConnected = false;
            mDeviceTypeInternal = 253;
        }
    }
    else
    {
        mIsConnected = false;
        mDeviceTypeInternal = 253;
    }

    if (mDeviceTypeInternal < 252)
        mDeviceType = mDeviceTypeInternal;
}

}

#endif // RIO_IS_CAFE
