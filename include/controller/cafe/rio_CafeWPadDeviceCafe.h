#ifndef RIO_CAFE_W_PAD_DEVICE_CAFE_H
#define RIO_CAFE_W_PAD_DEVICE_CAFE_H

#include <controller/rio_ControlDevice.h>
#include <misc/rio_MemUtil.h>

#include <padscore/kpad.h>

extern "C"
{

typedef struct KPADUnifiedWpadStatus
{
    u32 data[17];
} KPADUnifiedWpadStatus;

void KPADInitEx(KPADUnifiedWpadStatus[], u32);
u32 KPADGetMplsWorkSize();
void KPADSetMplsWorkarea(void*);

}

namespace rio {

class CafeWPadDevice : public ControlDevice
{
public:
    struct KPadInfo
    {
        KPadInfo()
            : last_read_length(0)
            , last_read_error(KPAD_ERROR_INVALID_CONTROLLER)
        {
            MemUtil::set(status, 0, sizeof(KPADStatus) * 16);
            for (u32 i = 0; i < 16; i++)
                status[i].extensionType = 253;
        }

        KPADStatus  status[16];
        s32         last_read_length;
        KPADError   last_read_error;
    };

public:
    CafeWPadDevice(ControllerMgr* mgr);
    virtual ~CafeWPadDevice();

    virtual void calc();

    const KPadInfo& getKPadInfo(s32 channel) const
    {
        RIO_ASSERT(channel < 4);
        return mKPadInfos[channel];
    }

private:
    KPADUnifiedWpadStatus   mUniRingBufs[4 * 16];
    KPadInfo                mKPadInfos[4];
};

}

#endif // RIO_CAFE_W_PAD_DEVICE_CAFE_H
