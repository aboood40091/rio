#include <controller/rio_Controller.h>
#include <controller/rio_ControllerMgr.h>

namespace rio {

Controller::Controller(ControllerMgr* mgr)
    : ControllerBase(PAD_IDX_MAX, PAD_IDX_LEFT_STICK_UP, PAD_IDX_RIGHT_STICK_UP, PAD_IDX_TOUCH)
    , mId(ControllerDefine::CONTROLLER_NULL)
    , mMgr(mgr)
{
}

void Controller::calc()
{
    u32 prev_hold = getHoldMask();
    bool prev_pointer_on = isPointerOn();

    calcImpl_();

    updateDerivativeParams_(prev_hold, prev_pointer_on);

    bool is_idle = true;

    if (is_idle && isIdle_())
        mIdleFrame++;
    else
        mIdleFrame = 0;
}

bool Controller::isIdle_()
{
    return isIdleBase_();
}

void Controller::setIdle_()
{
    setIdleBase_();
}

}
