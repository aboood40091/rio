#include <controller/rio_ControllerBase.h>

namespace rio {

const f32 ControllerBase::cStickHoldThresholdDefault    = 0.5f;
const f32 ControllerBase::cStickReleaseThresholdDefault = 0.25f;

const Vector2f ControllerBase::cInvalidPointer{ Mathf::min(), Mathf::min() };
const Vector2i ControllerBase::cInvalidPointerS32{ Mathi::min(), Mathi::min() };

ControllerBase::ControllerBase(s32 padBitMax, s32 leftStickCrossStartBit, s32 rightStickCrossStartBit, s32 touchKeyBit)
    : mPadTrig()
    , mPadRelease()
    , mPadRepeat()
    , mPointerFlag()
    , mPointerS32(cInvalidPointerS32)
    , mLeftStickHoldThreshold(0.5f)
    , mRightStickHoldThreshold(0.5f)
    , mLeftStickReleaseThreshold(0.25f)
    , mRightStickReleaseThreshold(0.25f)
    , mPadBitMax(padBitMax)
    , mLeftStickCrossStartBit(leftStickCrossStartBit)
    , mRightStickCrossStartBit(rightStickCrossStartBit)
    , mTouchKeyBit(touchKeyBit)
    , mIdleFrame(0)
    , mPadHold()
    , mPointer(cInvalidPointer)
    , mLeftStick{ 0.0f, 0.0f }
    , mRightStick{ 0.0f, 0.0f }
    , mLeftAnalogTrigger(0.0f)
    , mRightAnalogTrigger(0.0f)
{
    if (PAD_IDX_MAX_BASE < padBitMax)
    {
        RIO_ASSERT(false);
        mPadBitMax = PAD_IDX_MAX_BASE;
    }

    for (u32 i = 0; i < PAD_IDX_MAX_BASE; i++)
    {
        mPadRepeatDelays[i] = 30;
        mPadRepeatPulses[i] = 1;
        mPadHoldCounts[i] = 0;
    }
}

void ControllerBase::setPointer_(bool is_on, bool touchkey_hold, const Vector2f& pos)
{
    if (is_on)
    {
        mPointer.x = pos.x;
        mPointer.y = pos.y;
    }

    mPointerFlag.change(POINTER_ON, is_on);

    if (mTouchKeyBit >= 0)
    {
        mPadHold.changeBit(mTouchKeyBit, is_on && touchkey_hold);
    }
}

void ControllerBase::updateDerivativeParams_(u32 prev_hold, bool prev_pointer_on)
{
    u32 stick_hold = 0;

    if (mLeftStickCrossStartBit >= 0)
    {
        stick_hold |= getStickHold_(prev_hold, mLeftStick, mLeftStickHoldThreshold, mLeftStickReleaseThreshold, mLeftStickCrossStartBit);
    }

    if (mRightStickCrossStartBit >= 0)
    {
        stick_hold |= getStickHold_(prev_hold, mRightStick, mRightStickHoldThreshold, mRightStickReleaseThreshold, mRightStickCrossStartBit);
    }

    mPadHold.setDirect((mPadHold.getDirect() & ~createStickCrossMask_()) | stick_hold);

    mPadTrig.setDirect(~prev_hold & mPadHold.getDirect());
    mPadRelease.setDirect(prev_hold & ~mPadHold.getDirect());
    mPadRepeat.setDirect(0);

    for (s32 i = 0; i < mPadBitMax; i++)
    {
        if (mPadHold.isOnBit(i))
        {
            if (mPadRepeatPulses[i])
            {
                if (mPadHoldCounts[i] == mPadRepeatDelays[i])
                    mPadRepeat.setBit(i);

                else if (mPadRepeatDelays[i] < mPadHoldCounts[i] &&
                         (mPadHoldCounts[i] - mPadRepeatDelays[i]) % mPadRepeatPulses[i] == 0)
                {
                    mPadRepeat.setBit(i);
                }
            }

            mPadHoldCounts[i]++;
        }
        else
        {
            mPadHoldCounts[i] = 0;
        }
    }

    mPointerFlag.change(POINTER_ON_NOW, !prev_pointer_on && mPointerFlag.isOn(POINTER_ON));
    mPointerFlag.change(POINTER_OFF_NOW, prev_pointer_on && mPointerFlag.isOff(POINTER_ON));

    mPointerS32.x = (s32)mPointer.x;
    mPointerS32.y = (s32)mPointer.y;
}

u32 ControllerBase::getPadHoldCount(s32 bit) const
{
    RIO_ASSERT(bit < mPadBitMax);
    return mPadHoldCounts[bit];
}

void ControllerBase::setPadRepeat(u32 mask, u8 delay_frame, u8 pulse_frame)
{
    BitFlag32 pad_to_set(mask);

    for (s32 i = 0; i < mPadBitMax; i++)
    {
        if (pad_to_set.isOnBit(i))
        {
            mPadRepeatDelays[i] = delay_frame;
            mPadRepeatPulses[i] = pulse_frame;
        }
    }
}

void ControllerBase::setLeftStickCrossThreshold(f32 hold, f32 release)
{
    if (hold >= release)
    {
        mLeftStickHoldThreshold = hold;
        mLeftStickReleaseThreshold = release;
    }
    else
    {
        RIO_ASSERT(false);
    }
}

void ControllerBase::setRightStickCrossThreshold(f32 hold, f32 release)
{
    if (hold >= release)
    {
        mRightStickHoldThreshold = hold;
        mRightStickReleaseThreshold = release;
    }
    else
    {
        RIO_ASSERT(false);
    }
}

u32 ControllerBase::getStickHold_(u32 prev_hold, const Vector2f& stick, f32 hold_threshold, f32 release_threshold, s32 start_bit)
{
    f32 length = stick.length();

    if (length < release_threshold || (length < hold_threshold &&
                                       (prev_hold & (1 << (start_bit + CROSS_UP   ) |
                                                     1 << (start_bit + CROSS_DOWN ) |
                                                     1 << (start_bit + CROSS_LEFT ) |
                                                     1 << (start_bit + CROSS_RIGHT))) == 0))
    {
        return 0;
    }
    else
    {
        u32 angle = std::atan2(stick.y, stick.x) / Mathf::pi() * 0x80000000;

        if (angle < 0x10000000)
        {
            return 1 << (start_bit + CROSS_RIGHT);
        }
        else if (angle < 0x30000000)
        {
            return 1 << (start_bit + CROSS_RIGHT) |
                   1 << (start_bit + CROSS_UP   );
        }
        else if (angle < 0x50000000)
        {
            return 1 << (start_bit + CROSS_UP);
        }
        else if (angle < 0x70000000)
        {
            return 1 << (start_bit + CROSS_LEFT) |
                   1 << (start_bit + CROSS_UP  );
        }
        else if (angle < 0x90000000)
        {
            return 1 << (start_bit + CROSS_LEFT);
        }
        else if (angle < 0xb0000000)
        {
            return 1 << (start_bit + CROSS_LEFT) |
                   1 << (start_bit + CROSS_DOWN);
        }
        else if (angle < 0xd0000000)
        {
            return 1 << (start_bit + CROSS_DOWN);
        }
        else if (angle < 0xf0000000)
        {
            return 1 << (start_bit + CROSS_RIGHT) |
                   1 << (start_bit + CROSS_DOWN );
        }
        else
        {
            return 1 << (start_bit + CROSS_RIGHT);
        }
    }
}

bool ControllerBase::isIdleBase_()
{
    return getHoldMask() == 0 &&
           mLeftStick == Vector2f{ 0.0f, 0.0f } &&
           mRightStick == Vector2f{ 0.0f, 0.0f } &&
           mLeftAnalogTrigger == 0.0f &&
           mRightAnalogTrigger == 0.0f;
}

void ControllerBase::setIdleBase_()
{
    mPadHold.makeAllZero();
    mPadTrig.makeAllZero();
    mPadRelease.makeAllZero();
    mPadRepeat.makeAllZero();
    mPointerFlag.makeAllZero();

    for (s32 i = 0; i < mPadBitMax; i++)
        mPadHoldCounts[i] = 0;

    mPointer.set(cInvalidPointer.x, cInvalidPointer.y);
    mPointerS32.set(cInvalidPointerS32.x, cInvalidPointerS32.y);
    mLeftStick.set(0.0f, 0.0f);
    mRightStick.set(0.0f, 0.0f);
    mLeftAnalogTrigger = 0.0f;
    mRightAnalogTrigger = 0.0f;
}

u32 ControllerBase::createStickCrossMask_()
{
    BitFlag32 mask;

    if (mLeftStickCrossStartBit >= 0)
    {
        mask.setBit(mLeftStickCrossStartBit + CROSS_UP);
        mask.setBit(mLeftStickCrossStartBit + CROSS_DOWN);
        mask.setBit(mLeftStickCrossStartBit + CROSS_LEFT);
        mask.setBit(mLeftStickCrossStartBit + CROSS_RIGHT);
    }

    if (mRightStickCrossStartBit >= 0)
    {
        mask.setBit(mRightStickCrossStartBit + CROSS_UP);
        mask.setBit(mRightStickCrossStartBit + CROSS_DOWN);
        mask.setBit(mRightStickCrossStartBit + CROSS_LEFT);
        mask.setBit(mRightStickCrossStartBit + CROSS_RIGHT);
    }

    return mask.getDirect();
}

}
