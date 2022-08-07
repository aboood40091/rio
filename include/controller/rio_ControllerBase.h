#ifndef RIO_CONTROLLER_BASE_H
#define RIO_CONTROLLER_BASE_H

#include <math/rio_Vector.h>
#include <misc/rio_BitFlag.h>

namespace rio {

class ControllerBase
{
public:
    static const f32 cStickHoldThresholdDefault;
    static const f32 cStickReleaseThresholdDefault;
    static const Vector2f cInvalidPointer;
    static const Vector2i cInvalidPointerS32;

public:
    ControllerBase(s32 padBitMax, s32 leftStickCrossStartBit, s32 rightStickCrossStartBit, s32 touchKeyBit);

    u32 getHoldMask() const { return mPadHold.getDirect(); }
    u32 getTrigMask() const { return mPadTrig.getDirect(); }
    u32 getReleaseMask() const { return mPadRelease.getDirect(); }
    u32 getRepeatMask() const { return mPadRepeat.getDirect(); }

    u32 getPadHoldCount(s32 bit) const;

    const Vector2f& getLeftStick() const { return mLeftStick; }
    const Vector2f& getRightStick() const { return mRightStick; }
    f32 getLeftAnalogTrigger() const { return mLeftAnalogTrigger; }
    f32 getRightAnalogTrigger() const { return mRightAnalogTrigger; }
    const Vector2f& getPointer() const { return mPointer; }
    const Vector2i& getPointerPrev() const { return mPointerS32; }
    bool isPointerOn() const { return mPointerFlag.isOn(POINTER_ON); }
    bool isPointerOnNow() const { return mPointerFlag.isOn(POINTER_ON_NOW); }
    bool isPointerOffNow() const { return mPointerFlag.isOn(POINTER_OFF_NOW); }

    bool isHold(u32 mask) const { return mask & getHoldMask(); }
    bool isTrig(u32 mask) const { return mask & getTrigMask(); }
    bool isHoldAll(u32 mask) const { return (mask & getHoldMask()) == mask; }
    bool isRelease(u32 mask) const { return mask & getReleaseMask(); }
    bool isRepeat(u32 mask) const { return mask & getRepeatMask(); }

    bool isTrigWithRepeat(u32 mask) const
    {
        u32 trig = getTrigMask();
        u32 repeat = getRepeatMask();
        return (repeat | trig) & mask;
    }

    void setPadRepeat(u32 mask, u8 delay_frame, u8 pulse_frame);

    void setLeftStickCrossThreshold(f32 hold, f32 release);
    void setRightStickCrossThreshold(f32 hold, f32 release);

protected:
    bool isIdleBase_();
    void setIdleBase_();
    void setPointer_(bool is_on, bool touchkey_hold, const Vector2f& pos);
    void updateDerivativeParams_(u32 prev_hold, bool prev_pointer_on);
    u32 getStickHold_(u32 prev_hold, const Vector2f& stick, f32 hold_threshold, f32 release_threshold, s32 start_bit);
    u32 createStickCrossMask_();

    enum
    {
        PAD_IDX_MAX_BASE = 32
    };

    enum PointerFlagMask
    {
        POINTER_ON      = 1 << 0,
        POINTER_ON_NOW  = 1 << 1,
        POINTER_OFF_NOW = 1 << 2
    };

    enum
    {
        CROSS_UP,
        CROSS_DOWN,
        CROSS_LEFT,
        CROSS_RIGHT
    };

    BitFlag32   mPadTrig;
    BitFlag32   mPadRelease;
    BitFlag32   mPadRepeat;
    BitFlag32   mPointerFlag;
    Vector2i    mPointerS32;
    u32         mPadHoldCounts[PAD_IDX_MAX_BASE];
    u8          mPadRepeatDelays[PAD_IDX_MAX_BASE];
    u8          mPadRepeatPulses[PAD_IDX_MAX_BASE];
    f32         mLeftStickHoldThreshold;
    f32         mRightStickHoldThreshold;
    f32         mLeftStickReleaseThreshold;
    f32         mRightStickReleaseThreshold;
    s32         mPadBitMax;
    s32         mLeftStickCrossStartBit;
    s32         mRightStickCrossStartBit;
    s32         mTouchKeyBit;
    s32         mIdleFrame;
    BitFlag32   mPadHold;
    Vector2f    mPointer;
    Vector2f    mLeftStick;
    Vector2f    mRightStick;
    f32         mLeftAnalogTrigger;
    f32         mRightAnalogTrigger;
};

}

#endif // RIO_CONTROLLER_BASE_H
