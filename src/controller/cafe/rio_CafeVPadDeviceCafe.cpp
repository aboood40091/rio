#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <controller/cafe/rio_CafeVPadDeviceCafe.h>
#include <misc/rio_MemUtil.h>

namespace rio {

CafeVPadDevice::VPadInfo::VPadInfo()
    : last_read_length(0)
    , last_read_error(VPAD_READ_INVALID_CONTROLLER)
{
    MemUtil::set(status, 0, sizeof(VPADStatus) * 16);
}

CafeVPadDevice::CafeVPadDevice(ControllerMgr* mgr)
    : ControlDevice(mgr)
{
    mId = ControllerDefine::DEVICE_CAFE_V_PAD;
}

CafeVPadDevice::~CafeVPadDevice()
{
}

void CafeVPadDevice::calc()
{
    mVPadInfo.last_read_length = VPADRead(VPAD_CHAN_0, mVPadInfo.status, 16, &mVPadInfo.last_read_error);
}

}

#endif // RIO_IS_CAFE
