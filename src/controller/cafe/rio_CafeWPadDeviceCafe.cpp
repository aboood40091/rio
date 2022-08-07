#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <controller/cafe/rio_CafeWPadDeviceCafe.h>

namespace rio {

CafeWPadDevice::CafeWPadDevice(ControllerMgr* mgr)
    : ControlDevice(mgr)
{
    mId = ControllerDefine::DEVICE_CAFE_W_PAD;

    KPADInitEx(mUniRingBufs, 4 * 16);
    KPADSetMplsWorkarea(new u8[KPADGetMplsWorkSize()]);
}

CafeWPadDevice::~CafeWPadDevice()
{
    KPADShutdown();
}

void CafeWPadDevice::calc()
{
    for (u32 i = 0; i < 4; i++)
    {
        mKPadInfos[i].last_read_length = KPADReadEx(
            (KPADChan)i,
            mKPadInfos[i].status,
            16,
            &mKPadInfos[i].last_read_error
        );
    }
}

}

#endif // RIO_IS_CAFE
