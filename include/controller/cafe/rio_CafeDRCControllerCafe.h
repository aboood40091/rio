#ifndef RIO_CAFE_DRC_CONTROLLER_CAFE_H
#define RIO_CAFE_DRC_CONTROLLER_CAFE_H

#include <controller/rio_Controller.h>

namespace rio {

class CafeDRCController : public Controller
{
public:
    CafeDRCController(ControllerMgr* mgr);
    virtual ~CafeDRCController() { }

    virtual bool isConnected() const { return mIsConnected; }

    void setTouchPanelSamplingUseNum(u32 num);

private:
    virtual void calcImpl_();

    struct TouchPanelInfo
    {
        TouchPanelInfo()
            : pointer(cInvalidPointer)
            , flags(0)
        {
        }

        Vector2f    pointer;
        BitFlag32   flags;
    };

    static void updateTouchPanelInfo_(TouchPanelInfo* tp_info, bool tp_touch_on, f32 x, f32 y);

private:
    bool            mIsConnected;
    u32             mTouchPanelSamplingUseNum;
    u32             mTimeout;
    u32             mCurrentTouchPanel;
    TouchPanelInfo  mTouchPanelInfos[3];
};

}

#endif // RIO_CAFE_DRC_CONTROLLER_CAFE_H
