#ifndef RIO_GFX_PRIMITIVE_RENDERER_H
#define RIO_GFX_PRIMITIVE_RENDERER_H

#include <gfx/rio_Color.h>
#include <gpu/rio_Drawer.h>
#include <gpu/rio_Shader.h>
#include <gpu/rio_TextureSampler.h>
#include <gpu/rio_VertexArray.h>
#include <math/rio_Matrix.h>
#include <math/rio_Vector.h>

namespace rio {

class Camera;
class Projection;

class PrimitiveRenderer
{
public:
    class QuadArg
    {
    public:
        QuadArg()
            : mCenter{ 0.0f, 0.0f, 0.0f }
            , mSize{ 1.0f, 1.0f }
            , mColor0(Color4f::cWhite)
            , mColor1(Color4f::cWhite)
            , mHorizontal(false)
        {
        }

        QuadArg& setCenter(const Vector3f& p) { mCenter = p; return *this; }
        QuadArg& setSize(const Vector2f& size) { mSize = size; return *this; }
        QuadArg& setCornerAndSize(const Vector3f& p, const Vector2f& size);
        QuadArg& setColor(const Color4f& colorT, const Color4f& colorB);
        QuadArg& setColor(const Color4f& color) { return setColor(color, color); }
        QuadArg& setColorHorizontal(const Color4f& colorL, const Color4f& colorR);

        const Vector3f& getCenter() const { return mCenter; }
        const Vector2f& getSize() const { return mSize; }
        const Color4f& getColor0() const { return mColor0; }
        const Color4f& getColor1() const { return mColor1; }
        bool isHorizontal() const { return mHorizontal; }

    private:
        Vector3f mCenter;
        Vector2f mSize;
        Color4f mColor0;
        Color4f mColor1;
        bool mHorizontal;
    };

    class CubeArg
    {
    public:
        CubeArg()
            : mCenter{ 0.0f, 0.0f, 0.0f }
            , mSize{ 1.0f, 1.0f, 1.0f }
            , mColor0(Color4f::cWhite)
            , mColor1(Color4f::cWhite)
        {
        }

        CubeArg& setCenter(const Vector3f& p) { mCenter = p; return *this; }
        CubeArg& setSize(const Vector3f& size) { mSize = size; return *this; }
        CubeArg& setCornerAndSize(const Vector3f& p, const Vector3f& size);
        CubeArg& setColor(const Color4f& c0, const Color4f& c1) { mColor0 = c0; mColor1 = c1; return *this; }
        CubeArg& setColor(const Color4f& color) { return setColor(color, color); }

        const Vector3f& getCenter() const { return mCenter; }
        const Vector3f& getSize() const { return mSize; }
        const Color4f& getColor0() const { return mColor0; }
        const Color4f& getColor1() const { return mColor1; }

    private:
        Vector3f mCenter;
        Vector3f mSize;
        Color4f mColor0;
        Color4f mColor1;
    };

public:
    static bool createSingleton(const char* shader_path = "primitive_renderer");
    static void destroySingleton();
    static PrimitiveRenderer* instance() { return sInstance; }

private:
    static PrimitiveRenderer* sInstance;

    PrimitiveRenderer(const char* shader_path);
    ~PrimitiveRenderer();

    PrimitiveRenderer(const PrimitiveRenderer&);
    PrimitiveRenderer& operator=(const PrimitiveRenderer&);

private:
    void initialize_(const char* shader_path);

public:
    void setModelMatrix(const BaseMtx34f& model_matrix);
    const Matrix34f& getModelMatrix() const { return mModelMtx; }

    void setViewMtx(const BaseMtx34f& view_mtx);
    void setProjMtx(const BaseMtx44f& proj_mtx);

    void setCamera(const Camera& camera);
    void setProjection(const Projection& projection);

    void begin();
    void end();

    void drawQuad(const QuadArg& arg);
    void drawQuad(const Texture2D& texture, const QuadArg& arg);
    void drawBox(const QuadArg& arg);
    void drawCube(const CubeArg& arg);
    void drawWireCube(const CubeArg& arg);
    void drawLine(const Vector3f& from, const Vector3f& to, const Color4f& c0, const Color4f& c1);
    void drawLine(const Vector3f& from, const Vector3f& to, const Color4f& color) { drawLine(from, to, color, color); }
    void drawSphere4x8(const Vector3f& pos, f32 radius, const Color4f& north, const Color4f& south);
    void drawSphere4x8(const Vector3f& pos, f32 radius, const Color4f& color) { drawSphere4x8(pos, radius, color, color); }
    void drawSphere8x16(const Vector3f& pos, f32 radius, const Color4f& north, const Color4f& south);
    void drawSphere8x16(const Vector3f& pos, f32 radius, const Color4f& color) { drawSphere8x16(pos, radius, color, color); }
    void drawDisk16(const Vector3f& pos, f32 radius, const Color4f& center, const Color4f& edge);
    void drawDisk16(const Vector3f& pos, f32 radius, const Color4f& color) { drawDisk16(pos, radius, color, color); }
    void drawDisk32(const Vector3f& pos, f32 radius, const Color4f& center, const Color4f& edge);
    void drawDisk32(const Vector3f& pos, f32 radius, const Color4f& color) { drawDisk32(pos, radius, color, color); }
    void drawCircle16(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawCircle32(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawCylinder16(const Vector3f& pos, f32 radius, f32 height, const Color4f& top_color, const Color4f& btm_color);
    void drawCylinder16(const Vector3f& pos, f32 radius, f32 height, const Color4f& color) { drawCylinder16(pos, radius, height, color, color); }
    void drawCylinder32(const Vector3f& pos, f32 radius, f32 height, const Color4f& top_color, const Color4f& btm_color);
    void drawCylinder32(const Vector3f& pos, f32 radius, f32 height, const Color4f& color) { drawCylinder32(pos, radius, height, color, color); }

    void drawAxis(const Vector3f& pos, f32 scale);

private:
    void drawQuad_(const BaseMtx34f& model_mtx, const Color4f& colorL, const Color4f& colorR);
    void drawQuad_(const BaseMtx34f& model_mtx, const Texture2D& texture, const Color4f& colorL, const Color4f& colorR);
    void drawBox_(const BaseMtx34f& model_mtx, const Color4f& colorL, const Color4f& colorR);
    void drawCube_(const BaseMtx34f& model_mtx, const Color4f& c0, const Color4f& c1);
    void drawWireCube_(const BaseMtx34f& model_mtx, const Color4f& c0, const Color4f& c1);
    void drawLine_(const BaseMtx34f& model_mtx, const Color4f& c0, const Color4f& c1);
    void drawSphere4x8_(const BaseMtx34f& model_mtx, const Color4f& north, const Color4f& south);
    void drawSphere8x16_(const BaseMtx34f& model_mtx, const Color4f& north, const Color4f& south);
    void drawDisk16_(const BaseMtx34f& model_mtx, const Color4f& center, const Color4f& edge);
    void drawDisk32_(const BaseMtx34f& model_mtx, const Color4f& center, const Color4f& edge);
    void drawCircle16_(const BaseMtx34f& model_mtx, const Color4f& edge);
    void drawCircle32_(const BaseMtx34f& model_mtx, const Color4f& edge);
    void drawCylinder16_(const BaseMtx34f& model_mtx, const Color4f& top, const Color4f& btm);
    void drawCylinder32_(const BaseMtx34f& model_mtx, const Color4f& top, const Color4f& btm);

    struct Vertex
    {
        Vector3f pos;
        Vector2f uv;
        Color4f color;
    };
    static_assert(sizeof(Vertex) == 0x24, "Vertex size mismatch");

    void drawTriangles_(const BaseMtx34f& model_mtx, const Color4f& c0, const Color4f& c1, Vertex* vtx, u32 vtx_num, u16* idx, u32 idx_num, const Texture2D* texture = nullptr);
    void drawLines_(const BaseMtx34f& model_mtx, const Color4f& c0, const Color4f& c1, Drawer::PrimitiveMode mode, Vertex* vtx, u32 vtx_num, u16* idx, u32 idx_num);

    static inline void getQuadVertex(Vertex* vtx, u16* idx);
    static inline void getLineVertex(Vertex* vtx, u16* idx);
    static inline void getCubeVertex(Vertex* vtx, u16* idx);
    static inline void getWireCubeVertex(Vertex* vtx, u16* idx);
    static inline void getSphereVertex(Vertex* vtx, u16* idx, s32 sizeX, s32 sizeY);
    static inline void getDiskVertex(Vertex* vtx, u16* idx, s32 divNum);
    static inline void getCylinderVertex(Vertex* vtx, u16* idx, s32 divNum);

private:
    // MVP
    Matrix34f           mModelMtx;
    Matrix34f           mCameraMtx;
    Matrix44f           mProjectionMtx;

    // Shader
    Shader              mShader;
    u32                 mParamWVPOffset;
    u32                 mParamUserOffset;
    u32                 mParamRateOffset;
    u32                 mParamColor0Offset;
    u32                 mParamColor1Offset;
    u32                 mParamTexLocation;
    u32                 mAttrVertexLocation;
    u32                 mAttrTexCoord0Location;
    u32                 mAttrColorRateLocation;

    // Vertex Array Object
    VertexArray         mVertexArray;

    // Vertex Buffer Object
    VertexBuffer        mVertexBuffer;

    // Vertex Stream layouts
    VertexStream        mPosStream;
    VertexStream        mUVStream;
    VertexStream        mColorStream;

    // Quad Texture Sampler
    TextureSampler2D    mDrawQuadSampler;

    // Quad, Box
    Vertex*             mQuadVertexBuf;
    u16*                mQuadIndexBuf;
    u16*                mBoxIndexBuf;

    // Line
    Vertex*             mLineVertexBuf;
    u16*                mLineIndexBuf;

    // Cube
    Vertex*             mCubeVertexBuf;
    u16*                mCubeIndexBuf;

    // WireCube
    Vertex*             mWireCubeVertexBuf;
    u16*                mWireCubeIndexBuf;

    // SphereS
    Vertex*             mSphereSVertexBuf;
    u16*                mSphereSIndexBuf;

    // SphereL
    Vertex*             mSphereLVertexBuf;
    u16*                mSphereLIndexBuf;

    // DiskS, DiskL, CircleS, CircleL
    Vertex*             mDiskSVertexBuf;
    u16*                mDiskSIndexBuf;
    Vertex*             mDiskLVertexBuf;
    u16*                mDiskLIndexBuf;
    u16*                mCircleSIndexBuf;
    u16*                mCircleLIndexBuf;

    // CylinderS
    Vertex*             mCylinderSVertexBuf;
    u16*                mCylinderSIndexBuf;

    // CylinderL
    Vertex*             mCylinderLVertexBuf;
    u16*                mCylinderLIndexBuf;
};

}

#endif // RIO_GFX_PRIMITIVE_RENDERER_H
