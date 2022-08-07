#ifndef RIO_CAFE_V_PAD_DEVICE_CAFE_H
#define RIO_CAFE_V_PAD_DEVICE_CAFE_H

#include <controller/rio_ControlDevice.h>

#include <vpad/input.h>

namespace rio {

class CafeVPadDevice : public ControlDevice
{
public:
    struct VPadInfo
    {
        VPadInfo();

        VPADStatus      status[16];
        s32             last_read_length;
        VPADReadError   last_read_error;
    };

public:
    CafeVPadDevice(ControllerMgr* mgr);
    virtual ~CafeVPadDevice();

    virtual void calc();

    const VPadInfo& getVPadInfo() const
    {
        return mVPadInfo;
    }

private:
    VPadInfo mVPadInfo;
};

}

#endif // RIO_CAFE_V_PAD_DEVICE_CAFE_H
