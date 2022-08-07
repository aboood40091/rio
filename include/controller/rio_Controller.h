#ifndef RIO_CONTROLLER_H
#define RIO_CONTROLLER_H

#include <controller/rio_ControllerBase.h>
#include <controller/rio_ControllerDefine.h>

namespace rio {

class ControllerMgr;

class Controller : public ControllerBase
{
public:
    enum PadIdx
    {
        PAD_IDX_A                   = 0,
        PAD_IDX_B                   = 1,
        PAD_IDX_C                   = 2,
        PAD_IDX_X                   = 3,
        PAD_IDX_Y                   = 4,
        PAD_IDX_Z                   = 5,
        PAD_IDX_2                   = 6, // Also Right-Stick Click
        PAD_IDX_1                   = 7, // Also Left-Stick Click
        PAD_IDX_HOME                = 8,
        PAD_IDX_MINUS               = 9,
        PAD_IDX_PLUS                = 10,
        PAD_IDX_START               = 11,
        PAD_IDX_SELECT              = 12,
        PAD_IDX_ZL                  = PAD_IDX_C,
        PAD_IDX_ZR                  = PAD_IDX_Z,
        PAD_IDX_L                   = 13,
        PAD_IDX_R                   = 14,
        PAD_IDX_TOUCH               = 15,
        PAD_IDX_UP                  = 16,
        PAD_IDX_DOWN                = 17,
        PAD_IDX_LEFT                = 18,
        PAD_IDX_RIGHT               = 19,
        PAD_IDX_LEFT_STICK_UP       = 20,
        PAD_IDX_LEFT_STICK_DOWN     = 21,
        PAD_IDX_LEFT_STICK_LEFT     = 22,
        PAD_IDX_LEFT_STICK_RIGHT    = 23,
        PAD_IDX_RIGHT_STICK_UP      = 24,
        PAD_IDX_RIGHT_STICK_DOWN    = 25,
        PAD_IDX_RIGHT_STICK_LEFT    = 26,
        PAD_IDX_RIGHT_STICK_RIGHT   = 27,
        PAD_IDX_MAX                 = 28
    };

public:
    Controller(ControllerMgr* mgr);
    virtual ~Controller() { }

    virtual void calc();
    virtual bool isConnected() const { return true; }

protected:
    virtual void calcImpl_() = 0;
    virtual bool isIdle_();
    virtual void setIdle_();

public:
    ControllerDefine::ControllerId getId() const { return mId; }

protected:
    ControllerDefine::ControllerId  mId;
    ControllerMgr*                  mMgr;

    friend class ControllerMgr;
};

}

#endif // RIO_CONTROLLER_H
