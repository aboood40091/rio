#ifndef RIO_GFX_CAMERA_H
#define RIO_GFX_CAMERA_H

#include <math/rio_Vector.h>

namespace rio {

class Camera
{
public:
    virtual void getMatrix(BaseMtx34f* dst) const = 0;
};

class LookAtCamera : public Camera
{
public:
    LookAtCamera()
        : mPos({ 0.0f, 0.0f,  0.0f })
        , mAt ({ 0.0f, 0.0f, -1.0f })
        , mUp ({ 0.0f, 1.0f,  0.0f })
    {
    }

    LookAtCamera(const Vector3f& pos, const Vector3f& at, const Vector3f& up)
        : mPos(pos)
        , mAt (at)
    {
        setUp(up);
    }

    Vector3f& pos() { return mPos; }
    const Vector3f& pos() const { return mPos; }

    Vector3f& at() { return mAt; }
    const Vector3f& at() const { return mAt; }

    void setUp(const Vector3f& up) { mUp.setNormalized(up); }
    const Vector3f& getUp() const { return mUp; }

    virtual void getMatrix(BaseMtx34f* dst) const;

private:
    Vector3f    mPos;
    Vector3f    mAt;
    Vector3f    mUp;
};

class OrthoCamera : public Camera
{
public:
    OrthoCamera()
        : mPos{ 0.0f, 0.0f }
        , mZoomScale(1.0f)
    {
    }

    Vector2f& pos() { return mPos; }
    const Vector2f& pos() const { return mPos; }

    f32 getZoomScale() const { return mZoomScale; }
    void setZoomScale(f32 zoom_scale) { mZoomScale = zoom_scale; }

    void getMatrix(BaseMtx34f* dst) const override;

private:
    Vector2f    mPos;
    f32         mZoomScale;
};

}

#endif // RIO_GFX_CAMERA_H
