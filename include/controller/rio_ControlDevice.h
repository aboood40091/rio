#ifndef RIO_CONTROL_DEVICE_H
#define RIO_CONTROL_DEVICE_H

#include <controller/rio_ControllerDefine.h>

namespace rio {

class ControllerMgr;

class ControlDevice
{
public:
    ControlDevice(ControllerMgr* mgr)
        : mId(ControllerDefine::DEVICE_NULL)
        , mMgr(mgr)
    {
    }

    virtual ~ControlDevice()
    {
    }

    virtual void calc() = 0;

    ControllerDefine::DeviceId getId() const { return mId; }

protected:
    ControllerDefine::DeviceId  mId;
    ControllerMgr*              mMgr;

    friend class ControllerMgr;
};

}

#endif // RIO_CONTROL_DEVICE_H
