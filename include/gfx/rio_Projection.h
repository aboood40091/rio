#ifndef RIO_GFX_PROJECTION_H
#define RIO_GFX_PROJECTION_H

#include <math/rio_Vector.h>

namespace rio {

class Projection
{
public:
    const BaseMtx44f& getMatrix() const
    {
        if (mDirty)
        {
            updateMatrix_();
            mDirty = false;
        }

        return mMatrix;
    }

protected:
    virtual void updateMatrix_() const = 0;

protected:
    mutable bool        mDirty;
    mutable BaseMtx44f  mMatrix;
};

class PerspectiveProjection : public Projection
{
public:
    PerspectiveProjection();
    PerspectiveProjection(f32 near, f32 far, f32 fovy, f32 aspect);

    void set(f32 near, f32 far, f32 fovy, f32 aspect);

    f32 getNear() const { return mNear; }
    f32 getFar() const { return mFar; }
    f32 fovy() const { return mAngle; }
    f32 aspect() const { return mAspect; }
    const Vector2f& offset() const { return mOffset; }

    void setNear(f32 near)
    {
        mNear = near;
        mDirty = true;
    }

    void setFar(f32 far)
    {
        mFar = far;
        mDirty = true;
    }

    void setFovx(f32 fovx);

    void setFovy(f32 fovy)
    {
        setFovy_(fovy);
        mDirty = true;
    }

    void setAspect(f32 aspect)
    {
        mAspect = aspect;
        mDirty = true;
    }

    void setOffset(const Vector2f& offset)
    {
        mOffset.set(offset.x, offset.y);
        mDirty = true;
    }

    void setOffset(f32 offsetx, f32 offsety)
    {
        mOffset.set(offsetx, offsety);
        mDirty = true;
    }

    f32 getTop() const;
    f32 getBottom() const;
    f32 getLeft() const;
    f32 getRight() const;

private:
    void setFovy_(f32 fovy);
    virtual void updateMatrix_() const;

    f32 calcNearClipHeight_() const
    {
        return mNear * 2 * mFovyTan;
    }

    f32 calcNearClipWidth_() const
    {
        return calcNearClipHeight_() * mAspect;
    }

private:
    f32         mNear;
    f32         mFar;
    f32         mAngle;
    f32         mFovySin;
    f32         mFovyCos;
    f32         mFovyTan;
    f32         mAspect;
    Vector2f    mOffset;
};

class OrthoProjection : public Projection
{
public:
    OrthoProjection();
    OrthoProjection(f32 near, f32 far, f32 top, f32 bottom, f32 left, f32 right);

    void set(f32 near, f32 far, f32 top, f32 bottom, f32 left, f32 right);
    void setFromWindow(f32 near = -1000.f, f32 far = 1000.f);

    f32 getNear() const { return mNear; }
    f32 getFar() const { return mFar; }
    f32 top() const { return mTop; }
    f32 bottom() const { return mBottom; }
    f32 left() const { return mLeft; }
    f32 right() const { return mRight; }

    void setNear(f32 near)
    {
        mNear = near;
        mDirty = true;
    }

    void setFar(f32 far)
    {
        mFar = far;
        mDirty = true;
    }

    void setTBLR(f32 top, f32 bottom, f32 left, f32 right)
    {
        mTop = top;
        mBottom = bottom;
        mLeft = left;
        mRight = right;
        mDirty = true;
    }

private:
    virtual void updateMatrix_() const;

private:
    f32 mNear;
    f32 mFar;
    f32 mTop;
    f32 mBottom;
    f32 mLeft;
    f32 mRight;
};

}

#endif // RIO_GFX_PROJECTION_H
