#ifndef RIO_CAFE_REMOTE_CONTROLLER_CAFE_H
#define RIO_CAFE_REMOTE_CONTROLLER_CAFE_H

#include <controller/rio_Controller.h>

namespace rio {

class CafeRemoteController : public Controller
{
public:
    CafeRemoteController(ControllerMgr* mgr, s32 channel);
    virtual ~CafeRemoteController() { }

    virtual bool isConnected() const { return mIsConnected; }

    s32 getChannel() const { return mChannel; }

    u8 getInternalDeviceType() const { return mDeviceTypeInternal; }
    u8 getDeviceType() const { return mDeviceType; }

    bool isEnableCL() const { return mIsEnableCL; }
    void setEnableCL(bool enable) { mIsEnableCL = enable; }

private:
    virtual void calcImpl_();

    s32     mChannel;
    u8      mDeviceTypeInternal;
    u8      mDeviceType;
    bool    mIsConnected;
    bool    mIsEnableCL;
};

}

#endif // RIO_CAFE_REMOTE_CONTROLLER_CAFE_H
