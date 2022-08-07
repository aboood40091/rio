#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <controller/cafe/rio_CafeDRCControllerCafe.h>
#include <controller/cafe/rio_CafeVPadDeviceCafe.h>
#include <controller/rio_ControllerMgr.h>

namespace rio {

CafeDRCController::CafeDRCController(ControllerMgr* mgr)
    : Controller(mgr)
    , mIsConnected(false)
    , mTouchPanelSamplingUseNum(3)
    , mTimeout(0)
    , mCurrentTouchPanel(0)
{
    mId = ControllerDefine::CONTROLLER_CAFE_DRC;
}

void CafeDRCController::setTouchPanelSamplingUseNum(u32 num)
{
    if (num)
    {
        mTouchPanelSamplingUseNum = num;
    }
    else
    {
        RIO_ASSERT(false);
    }
}

void CafeDRCController::calcImpl_()
{
    CafeVPadDevice* device = static_cast<CafeVPadDevice*>(ControllerMgr::instance()->getControlDevice(ControllerDefine::DEVICE_CAFE_V_PAD));
    if (!device)
        return;

    const CafeVPadDevice::VPadInfo& v_pad_info = device->getVPadInfo();

    if (v_pad_info.last_read_length > 0 || (v_pad_info.last_read_error == VPAD_READ_NO_SAMPLES && mIsConnected && mTimeout < 180))
    {
        const VPADStatus& status = v_pad_info.status[0];
        u32 hold = status.hold;

        mPadHold.makeAllZero();
        mPadHold.change(1 << Controller::PAD_IDX_A,      hold & VPAD_BUTTON_A);
        mPadHold.change(1 << Controller::PAD_IDX_B,      hold & VPAD_BUTTON_B);
        mPadHold.change(1 << Controller::PAD_IDX_X,      hold & VPAD_BUTTON_X);
        mPadHold.change(1 << Controller::PAD_IDX_Y,      hold & VPAD_BUTTON_Y);
        mPadHold.change(1 << Controller::PAD_IDX_PLUS |
                        1 << Controller::PAD_IDX_START,  hold & VPAD_BUTTON_PLUS);
        mPadHold.change(1 << Controller::PAD_IDX_MINUS |
                        1 << Controller::PAD_IDX_SELECT, hold & VPAD_BUTTON_MINUS);
        mPadHold.change(1 << Controller::PAD_IDX_L,      hold & VPAD_BUTTON_L);
        mPadHold.change(1 << Controller::PAD_IDX_R,      hold & VPAD_BUTTON_R);
        mPadHold.change(1 << Controller::PAD_IDX_ZL,     hold & VPAD_BUTTON_ZL);
        mPadHold.change(1 << Controller::PAD_IDX_ZR,     hold & VPAD_BUTTON_ZR);
        mPadHold.change(1 << Controller::PAD_IDX_HOME,   hold & VPAD_BUTTON_HOME);
        mPadHold.change(1 << Controller::PAD_IDX_UP,     hold & VPAD_BUTTON_UP);
        mPadHold.change(1 << Controller::PAD_IDX_DOWN,   hold & VPAD_BUTTON_DOWN);
        mPadHold.change(1 << Controller::PAD_IDX_LEFT,   hold & VPAD_BUTTON_LEFT);
        mPadHold.change(1 << Controller::PAD_IDX_RIGHT,  hold & VPAD_BUTTON_RIGHT);
        mPadHold.change(1 << Controller::PAD_IDX_2,      hold & VPAD_BUTTON_STICK_R);
        mPadHold.change(1 << Controller::PAD_IDX_1,      hold & VPAD_BUTTON_STICK_L);

        mLeftStick.set(status.leftStick.x, status.leftStick.y);
        mRightStick.set(status.rightStick.x, status.rightStick.y);

        mLeftAnalogTrigger = 0.0f;
        mRightAnalogTrigger = 0.0f;

        bool tp_valid_x = false;
        bool tp_valid_y = false;

        u32 sample_num;
        if (v_pad_info.last_read_length < 1)
            sample_num = 1;
        else if ((u32)v_pad_info.last_read_length > mTouchPanelSamplingUseNum)
            sample_num = mTouchPanelSamplingUseNum;
        else
            sample_num = v_pad_info.last_read_length;

        VPADTouchData tp_data;

        for (u32 i = 0; i < sample_num; i++)
        {
            const VPADTouchData& tp_sample_data = v_pad_info.status[i].tpNormal;
            if (tp_sample_data.validity == (VPAD_INVALID_X | VPAD_INVALID_Y))
                continue;

            VPADTouchData tp_calibrated_data;
            VPADGetTPCalibratedPoint(VPAD_CHAN_0, &tp_calibrated_data, &tp_sample_data);

            if (!(tp_sample_data.validity & VPAD_INVALID_X))
            {
                tp_data.x = tp_calibrated_data.x;
                tp_valid_x = true;
            }

            if (!(tp_sample_data.validity & VPAD_INVALID_Y))
            {
                tp_data.y = tp_calibrated_data.y;
                tp_valid_y = true;
            }

            if (tp_valid_x && tp_valid_y)
                break;
        }

        updateTouchPanelInfo_(&mTouchPanelInfos[0], tp_valid_x && tp_valid_y, tp_data.x, tp_data.y);

        VPADGetTPCalibratedPoint(VPAD_CHAN_0, &tp_data, &v_pad_info.status[0].tpFiltered1);
        updateTouchPanelInfo_(&mTouchPanelInfos[1], tp_data.touched == 1, tp_data.x, tp_data.y);

        VPADGetTPCalibratedPoint(VPAD_CHAN_0, &tp_data, &v_pad_info.status[0].tpFiltered2);
        updateTouchPanelInfo_(&mTouchPanelInfos[2], tp_data.touched == 1, tp_data.x, tp_data.y);

        const TouchPanelInfo& tp_info = mTouchPanelInfos[mCurrentTouchPanel];
        setPointer_(tp_info.flags.isOn(POINTER_ON), true, tp_info.pointer);

        if (v_pad_info.last_read_error == VPAD_READ_NO_SAMPLES)
            mTimeout++;
        else
            mTimeout = 0;

        mIsConnected = true;
    }
    else
    {
        mIsConnected = false;
        mTimeout = 0;
    }
}

void CafeDRCController::updateTouchPanelInfo_(TouchPanelInfo* tp_info, bool tp_touch_on, f32 x, f32 y)
{
    bool prev_pointer_on = tp_info->flags.isOn(POINTER_ON);

    tp_info->flags.change(POINTER_ON, tp_touch_on);

    tp_info->flags.change(POINTER_ON_NOW, !prev_pointer_on && tp_touch_on);
    tp_info->flags.change(POINTER_OFF_NOW, prev_pointer_on && !tp_touch_on);

    if (tp_touch_on)
        tp_info->pointer.set(x, y);
}

}

#endif // RIO_IS_CAFE
