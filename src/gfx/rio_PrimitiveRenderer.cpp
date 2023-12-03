#include <gfx/rio_Camera.h>
#include <gfx/rio_PrimitiveRenderer.h>
#include <gfx/rio_Projection.h>
#include <gpu/rio_Drawer.h>
#include <misc/rio_MemUtil.h>

namespace rio {

PrimitiveRenderer* PrimitiveRenderer::sInstance = nullptr;

bool PrimitiveRenderer::createSingleton(const char* shader_path)
{
    if (sInstance)
        return false;

    sInstance = new PrimitiveRenderer(shader_path);
    return true;
}

void PrimitiveRenderer::destroySingleton()
{
    if (!sInstance)
        return;

    delete sInstance;
    sInstance = nullptr;
}

PrimitiveRenderer::PrimitiveRenderer(const char* shader_path)
    : mModelMtx(Matrix34f::ident)
    , mCameraMtx(Matrix34f::ident)
    , mProjectionMtx(Matrix44f::ident)
    , mShader()
    , mVertexArray()
    , mVertexBuffer()
    , mPosStream()
    , mUVStream()
    , mColorStream()
    , mDrawQuadSampler()

    // These will be set anyway in initialize_()
    //, mParamWVPOffset(0xFFFFFFFF)
    //, mParamUserOffset(0xFFFFFFFF)
    //, mParamRateOffset(0xFFFFFFFF)
    //, mParamColor0Offset(0xFFFFFFFF)
    //, mParamColor1Offset(0xFFFFFFFF)
    //, mParamTexLocation(0xFFFFFFFF)
    //, mAttrVertexLocation(0xFFFFFFFF)
    //, mAttrTexCoord0Location(0xFFFFFFFF)
    //, mAttrColorRateLocation(0xFFFFFFFF)
    //, mQuadVertexBuf(nullptr)
    //, mQuadIndexBuf(nullptr)
    //, mBoxIndexBuf(nullptr)
    //, mLineVertexBuf(nullptr)
    //, mLineIndexBuf(nullptr)
    //, mCubeVertexBuf(nullptr)
    //, mCubeIndexBuf(nullptr)
    //, mWireCubeVertexBuf(nullptr)
    //, mWireCubeIndexBuf(nullptr)
    //, mSphereSVertexBuf(nullptr)
    //, mSphereSIndexBuf(nullptr)
    //, mSphereLVertexBuf(nullptr)
    //, mSphereLIndexBuf(nullptr)
    //, mDiskSVertexBuf(nullptr)
    //, mDiskSIndexBuf(nullptr)
    //, mDiskLVertexBuf(nullptr)
    //, mDiskLIndexBuf(nullptr)
    //, mCircleSIndexBuf(nullptr)
    //, mCircleLIndexBuf(nullptr)
    //, mCylinderSVertexBuf(nullptr)
    //, mCylinderSIndexBuf(nullptr)
    //, mCylinderLVertexBuf(nullptr)
    //, mCylinderLIndexBuf(nullptr)
{
    initialize_(shader_path);
}

PrimitiveRenderer::~PrimitiveRenderer()
{
    MemUtil::free(mQuadVertexBuf);
    MemUtil::free(mQuadIndexBuf);

    MemUtil::free(mBoxIndexBuf);

    MemUtil::free(mLineVertexBuf);
    MemUtil::free(mLineIndexBuf);

    MemUtil::free(mCubeVertexBuf);
    MemUtil::free(mCubeIndexBuf);

    MemUtil::free(mWireCubeVertexBuf);
    MemUtil::free(mWireCubeIndexBuf);

    MemUtil::free(mSphereSVertexBuf);
    MemUtil::free(mSphereSIndexBuf);

    MemUtil::free(mSphereLVertexBuf);
    MemUtil::free(mSphereLIndexBuf);

    MemUtil::free(mDiskSVertexBuf);
    MemUtil::free(mDiskSIndexBuf);

    MemUtil::free(mDiskLVertexBuf);
    MemUtil::free(mDiskLIndexBuf);

    MemUtil::free(mCircleSIndexBuf);

    MemUtil::free(mCircleLIndexBuf);

    MemUtil::free(mCylinderSVertexBuf);
    MemUtil::free(mCylinderSIndexBuf);

    MemUtil::free(mCylinderLVertexBuf);
    MemUtil::free(mCylinderLIndexBuf);
}

void PrimitiveRenderer::initialize_(const char* shader_path)
{
    mShader.load(shader_path, Shader::MODE_UNIFORM_REGISTER);

    mParamWVPOffset = mShader.getVertexUniformLocation("wvp");
    RIO_ASSERT(mParamWVPOffset != 0xFFFFFFFF);
    mParamUserOffset = mShader.getVertexUniformLocation("user");
    RIO_ASSERT(mParamUserOffset != 0xFFFFFFFF);
    mParamColor0Offset = mShader.getVertexUniformLocation("color0");
    RIO_ASSERT(mParamColor0Offset != 0xFFFFFFFF);
    mParamColor1Offset = mShader.getVertexUniformLocation("color1");
    RIO_ASSERT(mParamColor1Offset != 0xFFFFFFFF);

    mParamRateOffset = mShader.getFragmentUniformLocation("rate");
    RIO_ASSERT(mParamRateOffset != 0xFFFFFFFF);
    mParamTexLocation = mShader.getFragmentSamplerLocation("texture0");
    RIO_ASSERT(mParamTexLocation != 0xFFFFFFFF);

    mAttrVertexLocation = mShader.getVertexAttribLocation("Vertex");
    RIO_ASSERT(mAttrVertexLocation != 0xFFFFFFFF);
    mAttrTexCoord0Location = mShader.getVertexAttribLocation("TexCoord0");
    RIO_ASSERT(mAttrTexCoord0Location != 0xFFFFFFFF);
    mAttrColorRateLocation = mShader.getVertexAttribLocation("ColorRate");
    RIO_ASSERT(mAttrColorRateLocation != 0xFFFFFFFF);

    mPosStream  .setLayout(mAttrVertexLocation,    VertexStream::FORMAT_32_32_32_FLOAT,    offsetof(Vertex, pos));
    mUVStream   .setLayout(mAttrTexCoord0Location, VertexStream::FORMAT_32_32_FLOAT,       offsetof(Vertex, uv));
    mColorStream.setLayout(mAttrColorRateLocation, VertexStream::FORMAT_32_32_32_32_FLOAT, offsetof(Vertex, color));

    mVertexBuffer.setStride(sizeof(Vertex));

    mVertexArray.initialize();
    mVertexArray.addAttribute(mPosStream, mVertexBuffer);
    mVertexArray.addAttribute(mUVStream, mVertexBuffer);
    mVertexArray.addAttribute(mColorStream, mVertexBuffer);
    mVertexArray.process();

    {
        // Quad
        mQuadVertexBuf = static_cast<Vertex*>(MemUtil::alloc(4 * sizeof(Vertex), Drawer::cVtxAlignment));
        mQuadIndexBuf  = static_cast<   u16*>(MemUtil::alloc(6 * sizeof(   u16), Drawer::cIdxAlignment));

        getQuadVertex(mQuadVertexBuf, mQuadIndexBuf);

        VertexBuffer::invalidateCache(mQuadVertexBuf, 4 * sizeof(Vertex));
        VertexBuffer::invalidateCache(mQuadIndexBuf,  6 * sizeof(   u16));

        // Box
        mBoxIndexBuf   = static_cast<   u16*>(MemUtil::alloc(4 * sizeof(   u16), Drawer::cIdxAlignment));

        static const u16 idx[4] = { 0, 1, 3, 2 };
        MemUtil::copy(mBoxIndexBuf, idx, sizeof(idx));

        VertexBuffer::invalidateCache(mBoxIndexBuf,   4 * sizeof(   u16));
    }

    {
        // Line
        mLineVertexBuf = static_cast<Vertex*>(MemUtil::alloc(4 * sizeof(Vertex), Drawer::cVtxAlignment));
        mLineIndexBuf  = static_cast<   u16*>(MemUtil::alloc(6 * sizeof(   u16), Drawer::cIdxAlignment));

        getLineVertex(mLineVertexBuf, mLineIndexBuf);

        VertexBuffer::invalidateCache(mLineVertexBuf, 4 * sizeof(Vertex));
        VertexBuffer::invalidateCache(mLineIndexBuf,  6 * sizeof(   u16));
    }

    {
        // Cube
        mCubeVertexBuf = static_cast<Vertex*>(MemUtil::alloc(8  * sizeof(Vertex), Drawer::cVtxAlignment));
        mCubeIndexBuf  = static_cast<   u16*>(MemUtil::alloc(36 * sizeof(   u16), Drawer::cIdxAlignment));

        getCubeVertex(mCubeVertexBuf, mCubeIndexBuf);

        VertexBuffer::invalidateCache(mCubeVertexBuf, 8  * sizeof(Vertex));
        VertexBuffer::invalidateCache(mCubeIndexBuf,  36 * sizeof(   u16));
    }

    {
        // WireCube
        mWireCubeVertexBuf = static_cast<Vertex*>(MemUtil::alloc(8  * sizeof(Vertex), Drawer::cVtxAlignment));
        mWireCubeIndexBuf  = static_cast<   u16*>(MemUtil::alloc(17 * sizeof(   u16), Drawer::cIdxAlignment));

        getWireCubeVertex(mWireCubeVertexBuf, mWireCubeIndexBuf);

        VertexBuffer::invalidateCache(mWireCubeVertexBuf, 8  * sizeof(Vertex));
        VertexBuffer::invalidateCache(mWireCubeIndexBuf,  17 * sizeof(   u16));
    }

    {
        // SphereS
        mSphereSVertexBuf = static_cast<Vertex*>(MemUtil::alloc((4*8 + 2) * sizeof(Vertex), Drawer::cVtxAlignment));
        mSphereSIndexBuf  = static_cast<   u16*>(MemUtil::alloc((4*8 * 6) * sizeof(   u16), Drawer::cIdxAlignment));

        getSphereVertex(mSphereSVertexBuf, mSphereSIndexBuf, 8, 4);

        VertexBuffer::invalidateCache(mSphereSVertexBuf, (4*8 + 2) * sizeof(Vertex));
        VertexBuffer::invalidateCache(mSphereSIndexBuf,  (4*8 * 6) * sizeof(   u16));
    }

    {
        // SphereL
        mSphereLVertexBuf = static_cast<Vertex*>(MemUtil::alloc((8*16 + 2) * sizeof(Vertex), Drawer::cVtxAlignment));
        mSphereLIndexBuf  = static_cast<   u16*>(MemUtil::alloc((8*16 * 6) * sizeof(   u16), Drawer::cIdxAlignment));

        getSphereVertex(mSphereLVertexBuf, mSphereLIndexBuf, 16, 8);

        VertexBuffer::invalidateCache(mSphereLVertexBuf, (8*16 + 2) * sizeof(Vertex));
        VertexBuffer::invalidateCache(mSphereLIndexBuf,  (8*16 * 6) * sizeof(   u16));
    }

    {
        // DiskS
        mDiskSVertexBuf  = static_cast<Vertex*>(MemUtil::alloc((16 + 1) * sizeof(Vertex), Drawer::cVtxAlignment));
        mDiskSIndexBuf   = static_cast<   u16*>(MemUtil::alloc((16 * 3) * sizeof(   u16), Drawer::cIdxAlignment));

        getDiskVertex(mDiskSVertexBuf, mDiskSIndexBuf, 16);

        VertexBuffer::invalidateCache(mDiskSVertexBuf,  (16 + 1) * sizeof(Vertex));
        VertexBuffer::invalidateCache(mDiskSIndexBuf,   (16 * 3) * sizeof(   u16));

        // DiskL
        mDiskLVertexBuf  = static_cast<Vertex*>(MemUtil::alloc((32 + 1) * sizeof(Vertex), Drawer::cVtxAlignment));
        mDiskLIndexBuf   = static_cast<   u16*>(MemUtil::alloc((32 * 3) * sizeof(   u16), Drawer::cIdxAlignment));

        getDiskVertex(mDiskLVertexBuf, mDiskLIndexBuf, 32);

        VertexBuffer::invalidateCache(mDiskLVertexBuf,  (32 + 1) * sizeof(Vertex));
        VertexBuffer::invalidateCache(mDiskLIndexBuf,   (32 * 3) * sizeof(   u16));

        // CircleS
        mCircleSIndexBuf = static_cast<   u16*>(MemUtil::alloc(16       * sizeof(   u16), Drawer::cIdxAlignment));

        for (s32 i = 0; i < 16; i++)
            mCircleSIndexBuf[i] = i;

        VertexBuffer::invalidateCache(mCircleSIndexBuf, 16       * sizeof(   u16));

        // CircleL
        mCircleLIndexBuf = static_cast<   u16*>(MemUtil::alloc(32       * sizeof(   u16), Drawer::cIdxAlignment));

        for (s32 i = 0; i < 32; i++)
            mCircleLIndexBuf[i] = i;

        VertexBuffer::invalidateCache(mCircleLIndexBuf, 32       * sizeof(   u16));
    }

    {
        // CylinderS
        mCylinderSVertexBuf = static_cast<Vertex*>(MemUtil::alloc((16*2 +  2) * sizeof(Vertex), Drawer::cVtxAlignment));
        mCylinderSIndexBuf  = static_cast<   u16*>(MemUtil::alloc((16   * 12) * sizeof(   u16), Drawer::cIdxAlignment));

        getCylinderVertex(mCylinderSVertexBuf, mCylinderSIndexBuf, 16);

        VertexBuffer::invalidateCache(mCylinderSVertexBuf, (16*2 +  2) * sizeof(Vertex));
        VertexBuffer::invalidateCache(mCylinderSIndexBuf,  (16   * 12) * sizeof(   u16));
    }

    {
        // CylinderL
        mCylinderLVertexBuf = static_cast<Vertex*>(MemUtil::alloc((32*2 +  2) * sizeof(Vertex), Drawer::cVtxAlignment));
        mCylinderLIndexBuf  = static_cast<   u16*>(MemUtil::alloc((32   * 12) * sizeof(   u16), Drawer::cIdxAlignment));

        getCylinderVertex(mCylinderLVertexBuf, mCylinderLIndexBuf, 32);

        VertexBuffer::invalidateCache(mCylinderLVertexBuf, (32*2 +  2) * sizeof(Vertex));
        VertexBuffer::invalidateCache(mCylinderLIndexBuf,  (32   * 12) * sizeof(   u16));
    }

    mDrawQuadSampler.setWrap(TEX_WRAP_MODE_CLAMP,
                             TEX_WRAP_MODE_CLAMP,
                             TEX_WRAP_MODE_CLAMP);

    mDrawQuadSampler.setFilter(TEX_XY_FILTER_MODE_LINEAR,
                               TEX_XY_FILTER_MODE_LINEAR,
                               TEX_MIP_FILTER_MODE_NONE,
                               TEX_ANISO_1_TO_1);
}

void PrimitiveRenderer::setModelMatrix(const BaseMtx34f& model_matrix)
{
    mModelMtx = static_cast<const Matrix34f&>(model_matrix);
}

void PrimitiveRenderer::setViewMtx(const BaseMtx34f& view_mtx)
{
    mCameraMtx = static_cast<const Matrix34f&>(view_mtx);
}

void PrimitiveRenderer::setProjMtx(const BaseMtx44f& proj_mtx)
{
    mProjectionMtx = static_cast<const Matrix44f&>(proj_mtx);
}

void PrimitiveRenderer::setCamera(const Camera& camera)
{
    camera.getMatrix(&mCameraMtx);
}

void PrimitiveRenderer::setProjection(const Projection& projection)
{
    mProjectionMtx = static_cast<const Matrix44f&>(projection.getMatrix());
}

void PrimitiveRenderer::begin()
{
    Matrix44f viewProj;
    viewProj.setMul(mProjectionMtx, mCameraMtx);

    mShader.bind();
    mShader.setUniformArray(4, viewProj.v,         mParamWVPOffset,  0xFFFFFFFF);
    mShader.setUniformArray(3, Matrix34f::ident.v, mParamUserOffset, 0xFFFFFFFF);
}

void PrimitiveRenderer::end()
{
}

void PrimitiveRenderer::drawQuad(const QuadArg& arg)
{
    Matrix34f mtx;

    if (arg.isHorizontal())
    {
        mtx.makeSRT(
            { arg.getSize().y, arg.getSize().x, 1.0f },
            { 0.0f, 0.0f, Mathf::deg2rad(90) },
            arg.getCenter()
        );
    }
    else
    {
        mtx.makeST(
            { arg.getSize().x, arg.getSize().y, 1.0f },
            arg.getCenter()
        );
    }

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawQuad_(outMtx, arg.getColor0(), arg.getColor1());
}

void PrimitiveRenderer::drawQuad(const Texture2D& texture, const QuadArg& arg)
{
    Matrix34f mtx;

    if (arg.isHorizontal())
    {
        mtx.makeSRT(
            { arg.getSize().y, arg.getSize().x, 1.0f },
            { 0.0f, 0.0f, Mathf::deg2rad(90) },
            arg.getCenter()
        );
    }
    else
    {
        mtx.makeST(
            { arg.getSize().x, arg.getSize().y, 1.0f },
            arg.getCenter()
        );
    }

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawQuad_(outMtx, texture, arg.getColor0(), arg.getColor1());
}

void PrimitiveRenderer::drawBox(const QuadArg& arg)
{
    Matrix34f mtx;

    if (arg.isHorizontal())
    {
        mtx.makeSRT(
            { arg.getSize().y, arg.getSize().x, 1.0f },
            { 0.0f, 0.0f, Mathf::deg2rad(90) },
            arg.getCenter()
        );
    }
    else
    {
        mtx.makeST(
            { arg.getSize().x, arg.getSize().y, 1.0f },
            arg.getCenter()
        );
    }

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawBox_(outMtx, arg.getColor0(), arg.getColor1());
}

void PrimitiveRenderer::drawCube(const CubeArg& arg)
{
    Matrix34f mtx;
    mtx.makeST(arg.getSize(), arg.getCenter());

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawCube_(outMtx, arg.getColor0(), arg.getColor1());
}

void PrimitiveRenderer::drawWireCube(const CubeArg& arg)
{
    Matrix34f mtx;
    mtx.makeST(arg.getSize(), arg.getCenter());

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawWireCube_(outMtx, arg.getColor0(), arg.getColor1());
}

void PrimitiveRenderer::drawLine(const Vector3f& from, const Vector3f& to, const Color4f& c0, const Color4f& c1)
{
    Vector3f dir = to - from;

    Matrix34f mtxS;
    mtxS.makeS({ dir.length(), 1.0f, 1.0f });
    dir.normalize();

    Matrix34f mtxR;
    if (!mtxR.makeVectorRotation({ 1.0f, 0.0f, 0.0f }, dir))
        mtxR.makeR({ 0.0f, Mathf::pi(), 0.0f });

    Matrix34f mtx;
    mtx.setMul(mtxR, mtxS);

    dir.setScaleAdd(to - from, 0.5f, from);
    mtx.setTranslationWorld(dir);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawLine_(outMtx, c0, c1);
}

void PrimitiveRenderer::drawSphere4x8(const Vector3f& pos, f32 radius, const Color4f& north, const Color4f& south)
{
    f32 diameter = 2.0f * radius;
    Vector3f scale = { diameter, diameter, diameter };

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawSphere4x8_(outMtx, north, south);
}

void PrimitiveRenderer::drawSphere8x16(const Vector3f& pos, f32 radius, const Color4f& north, const Color4f& south)
{
    f32 diameter = 2.0f * radius;
    Vector3f scale = { diameter, diameter, diameter };

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawSphere8x16_(outMtx, north, south);
}

void PrimitiveRenderer::drawDisk16(const Vector3f& pos, f32 radius, const Color4f& center, const Color4f& edge)
{
    f32 diameter = 2.0f * radius;
    Vector3f scale = { diameter, diameter, diameter };

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawDisk16_(outMtx, center, edge);
}

void PrimitiveRenderer::drawDisk32(const Vector3f& pos, f32 radius, const Color4f& center, const Color4f& edge)
{
    f32 diameter = 2.0f * radius;
    Vector3f scale = { diameter, diameter, diameter };

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawDisk32_(outMtx, center, edge);
}

void PrimitiveRenderer::drawCircle16(const Vector3f& pos, f32 radius, const Color4f& color)
{
    f32 diameter = 2.0f * radius;
    Vector3f scale = { diameter, diameter, diameter };

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawCircle16_(outMtx, color);
}

void PrimitiveRenderer::drawCircle32(const Vector3f& pos, f32 radius, const Color4f& color)
{
    f32 diameter = 2.0f * radius;
    Vector3f scale = { diameter, diameter, diameter };

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawCircle32_(outMtx, color);
}

void PrimitiveRenderer::drawCylinder16(const Vector3f& pos, f32 radius, f32 height, const Color4f& top_color, const Color4f& btm_color)
{
    f32 diameter = 2.0f * radius;
    Vector3f scale = { diameter, height, diameter };

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawCylinder16_(outMtx, top_color, btm_color);
}

void PrimitiveRenderer::drawCylinder32(const Vector3f& pos, f32 radius, f32 height, const Color4f& top_color, const Color4f& btm_color)
{
    f32 diameter = 2.0f * radius;
    Vector3f scale = { diameter, height, diameter };

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    drawCylinder32_(outMtx, top_color, btm_color);
}

void PrimitiveRenderer::drawAxis(const Vector3f& pos, f32 scale)
{
    { Vector3f v = { scale, 0.0f, 0.0f }; v += pos;
      drawLine(pos, v, Color4f::cRed); }

    { Vector3f v = { 0.0f, scale, 0.0f }; v += pos;
      drawLine(pos, v, Color4f::cGreen); }

    { Vector3f v = { 0.0f, 0.0f, scale }; v += pos;
      drawLine(pos, v, Color4f::cBlue); }
}

void PrimitiveRenderer::drawQuad_(
    const BaseMtx34f& model_mtx, const Color4f& colorL, const Color4f& colorR
)
{
    drawTriangles_(model_mtx, colorL, colorR, mQuadVertexBuf, 4, mQuadIndexBuf, 6);
}

void PrimitiveRenderer::drawQuad_(
    const BaseMtx34f& model_mtx, const Texture2D& texture, const Color4f& colorL, const Color4f& colorR
)
{
    drawTriangles_(model_mtx, colorL, colorR, mQuadVertexBuf, 4, mQuadIndexBuf, 6, &texture);
}

void PrimitiveRenderer::drawBox_(
    const BaseMtx34f& model_mtx, const Color4f& colorL, const Color4f& colorR
)
{
    drawLines_(model_mtx, colorL, colorR, Drawer::LINE_LOOP, mQuadVertexBuf, 4, mBoxIndexBuf, 4);
}

void PrimitiveRenderer::drawCube_(
    const BaseMtx34f& model_mtx, const Color4f& c0, const Color4f& c1
)
{
    drawTriangles_(model_mtx, c0, c1, mCubeVertexBuf, 8, mCubeIndexBuf, 36);
}

void PrimitiveRenderer::drawWireCube_(
    const BaseMtx34f& model_mtx, const Color4f& c0, const Color4f& c1
)
{
    drawLines_(model_mtx, c0, c1, Drawer::LINE_LOOP, mWireCubeVertexBuf, 8, mWireCubeIndexBuf, 17);
}

void PrimitiveRenderer::drawLine_(
    const BaseMtx34f& model_mtx, const Color4f& c0, const Color4f& c1
)
{
    drawLines_(model_mtx, c0, c1, Drawer::LINES, mLineVertexBuf, 2, mLineIndexBuf, 2);
}

void PrimitiveRenderer::drawSphere4x8_(
    const BaseMtx34f& model_mtx, const Color4f& north, const Color4f& south
)
{
    drawTriangles_(model_mtx, north, south, mSphereSVertexBuf, 4*8 + 2, mSphereSIndexBuf, 4*8 * 6);
}

void PrimitiveRenderer::drawSphere8x16_(
    const BaseMtx34f& model_mtx, const Color4f& north, const Color4f& south
)
{
    drawTriangles_(model_mtx, north, south, mSphereLVertexBuf, 8*16 + 2, mSphereLIndexBuf, 8*16 * 6);
}

void PrimitiveRenderer::drawDisk16_(
    const BaseMtx34f& model_mtx, const Color4f& center, const Color4f& edge
)
{
    drawTriangles_(model_mtx, center, edge, mDiskSVertexBuf, 16 + 1, mDiskSIndexBuf, 16 * 3);
}

void PrimitiveRenderer::drawDisk32_(
    const BaseMtx34f& model_mtx, const Color4f& center, const Color4f& edge
)
{
    drawTriangles_(model_mtx, center, edge, mDiskLVertexBuf, 32 + 1, mDiskLIndexBuf, 32 * 3);
}

void PrimitiveRenderer::drawCircle16_(
    const BaseMtx34f& model_mtx, const Color4f& edge
)
{
    drawLines_(model_mtx, edge, edge, Drawer::LINE_LOOP, mDiskSVertexBuf, 16 + 1, mCircleSIndexBuf, 16);
}

void PrimitiveRenderer::drawCircle32_(
    const BaseMtx34f& model_mtx, const Color4f& edge
)
{
    drawLines_(model_mtx, edge, edge, Drawer::LINE_LOOP, mDiskLVertexBuf, 32 + 1, mCircleLIndexBuf, 32);
}

void PrimitiveRenderer::drawCylinder16_(
    const BaseMtx34f& model_mtx, const Color4f& top, const Color4f& btm
)
{
    drawTriangles_(model_mtx, top, btm, mCylinderSVertexBuf, 16*2 + 2, mCylinderSIndexBuf, 16 * 12);
}

void PrimitiveRenderer::drawCylinder32_(
    const BaseMtx34f& model_mtx, const Color4f& top, const Color4f& btm
)
{
    drawTriangles_(model_mtx, top, btm, mCylinderLVertexBuf, 32*2 + 2, mCylinderLIndexBuf, 32 * 12);
}

void PrimitiveRenderer::drawTriangles_(
    const BaseMtx34f& model_mtx, const Color4f& c0, const Color4f& c1,
    Vertex* vtx, u32 vtx_num, u16* idx, u32 idx_num,
    const Texture2D* texture
)
{
    mShader.setUniformArray(3, model_mtx.v, mParamUserOffset, 0xFFFFFFFF);

    mShader.setUniform(c0.v, mParamColor0Offset, 0xFFFFFFFF);
    mShader.setUniform(c1.v, mParamColor1Offset, 0xFFFFFFFF);

    if (texture)
    {
        mShader.setUniform(1.0f, 0xFFFFFFFF, mParamRateOffset);
        mDrawQuadSampler.linkTexture2D(texture);
        mDrawQuadSampler.bindFS(mParamTexLocation, 0);
    }
    else
    {
        mShader.setUniform(0.0f, 0xFFFFFFFF, mParamRateOffset);
    }

    mVertexBuffer.setData(vtx, vtx_num * sizeof(Vertex));
    mVertexArray.bind();

    Drawer::DrawElements(Drawer::TRIANGLES, idx_num, idx);
}

void PrimitiveRenderer::drawLines_(
    const BaseMtx34f& model_mtx, const Color4f& c0, const Color4f& c1,
    Drawer::PrimitiveMode mode,
    Vertex* vtx, u32 vtx_num, u16* idx, u32 idx_num
)
{
    mShader.setUniformArray(3, model_mtx.v, mParamUserOffset, 0xFFFFFFFF);

    mShader.setUniform(c0.v, mParamColor0Offset, 0xFFFFFFFF);
    mShader.setUniform(c1.v, mParamColor1Offset, 0xFFFFFFFF);

    mShader.setUniform(0.0f, 0xFFFFFFFF, mParamRateOffset);

    mVertexBuffer.setData(vtx, vtx_num * sizeof(Vertex));
    mVertexArray.bind();

    Drawer::DrawElements(mode, idx_num, idx);
}

void PrimitiveRenderer::getQuadVertex(Vertex* vtx, u16* idx)
{
    static const Vertex cVtx[4] = {
        { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } },
        { {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } },
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } },
        { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } }
    };

    static const u16 cIdx[6] = {
        0, 2, 1,
        1, 2, 3
    };

    if (vtx)
        MemUtil::copy(vtx, cVtx, sizeof(cVtx));

    if (idx)
        MemUtil::copy(idx, cIdx, sizeof(cIdx));
}

void PrimitiveRenderer::getLineVertex(Vertex* vtx, u16* idx)
{
    static const Vertex cVtx[2] = {
        { { -0.5f, 0.0f, 0.0f }, { 0.0f, 0.5f }, { 0.0f, 0.0f, 0.0f, 0.0f } },
        { {  0.5f, 0.0f, 0.0f }, { 1.0f, 0.5f }, { 1.0f, 0.0f, 0.0f, 0.0f } }
    };

    static const u16 cIdx[2] = {
        0, 1
    };

    if (vtx)
        MemUtil::copy(vtx, cVtx, sizeof(cVtx));

    if (idx)
        MemUtil::copy(idx, cIdx, sizeof(cIdx));
}

void PrimitiveRenderer::getCubeVertex(Vertex* vtx, u16* idx)
{
    static const Vertex cVtx[8] = {
        { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { 1.0f / 3.0f, 0.0f, 0.0f, 0.0f } },
        { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, {        0.0f, 0.0f, 0.0f, 0.0f } },
        { { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, { 1.0f / 3.0f, 0.0f, 0.0f, 0.0f } },
        { { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f }, { 2.0f / 3.0f, 0.0f, 0.0f, 0.0f } },
        { {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f }, {        1.0f, 0.0f, 0.0f, 0.0f } },
        { {  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f }, { 2.0f / 3.0f, 0.0f, 0.0f, 0.0f } },
        { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f }, { 1.0f / 3.0f, 0.0f, 0.0f, 0.0f } },
        { {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f }, { 2.0f / 3.0f, 0.0f, 0.0f, 0.0f } }
    };

    static const u16 cIdx[36] = {
        2, 1, 0,
        3, 2, 0,
        5, 2, 3,
        4, 5, 3,
        6, 5, 7,
        7, 5, 4,
        1, 6, 0,
        6, 7, 0,
        0, 7, 3,
        3, 7, 4,
        1, 2, 6,
        2, 5, 6
    };

    if (vtx)
        MemUtil::copy(vtx, cVtx, sizeof(cVtx));

    if (idx)
        MemUtil::copy(idx, cIdx, sizeof(cIdx));
}

void PrimitiveRenderer::getWireCubeVertex(Vertex* vtx, u16* idx)
{
    getCubeVertex(vtx, nullptr);

    static const u16 cIdx[17] = {
        0, 1, 2, 3, 0, 7, 6, 1,
        2, 5, 6, 7, 4, 5, 4, 3, 0
    };

    if (idx)
        MemUtil::copy(idx, cIdx, sizeof(cIdx));
}

void PrimitiveRenderer::getSphereVertex(Vertex* vtx, u16* idx, s32 sizeX, s32 sizeY)
{
    if (vtx)
    {
        for (s32 y = 0; y < sizeY; y++)
        {
            f32 angle_y = ((y + 1) / (sizeY + 1.0f) - 0.5f) * Mathf::deg2rad(180);

            f32 pos_y = std::sin(angle_y) * 0.5f;
            f32 radius = std::cos(angle_y) * 0.5f;

            for (s32 x = 0; x < sizeX; x++)
            {
                s32 pos = y * sizeX + x;
                f32 angle_x = Mathf::deg2rad(360) * x / sizeX;
                if (y % 2 == 0)
                    angle_x -= Mathf::deg2rad(360) / sizeX / 2;

                f32 pos_x = std::cos(angle_x) * radius;
                f32 pos_z = std::sin(angle_x) * radius;

                vtx[pos].pos.set(pos_x, pos_y, pos_z);
                vtx[pos].uv.set(pos_y + 0.5f, static_cast<f32>(x) / sizeX);
                vtx[pos].color.setf(0.5f - pos_y, 0.0f, 0.0f, 0.0f);
            }
        }

        {
            s32 pos = sizeX * sizeY;

            vtx[pos].pos.set(0.0f, -0.5f, 0.0f);
            vtx[pos].uv.set(0.0f, 0.5f);
            vtx[pos].color.setf(1.0f, 0.0f, 0.0f, 0.0f);
        }

        {
            s32 pos = sizeX * sizeY + 1;

            vtx[pos].pos.set(0.0f, 0.5f, 0.0f);
            vtx[pos].uv.set(1.0f, 0.5f);
            vtx[pos].color.setf(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    if (idx)
    {
        for (s32 i = 0; i < sizeX; i++)
        {
          idx[i * 3 + 0] = sizeX * sizeY;
          idx[i * 3 + 1] = i;
          idx[i * 3 + 2] = (i + 1) % sizeX;
        }

        for (s32 y = 0; y < sizeY - 1; y++)
        {
            for (s32 x = 0; x < sizeX; x++)
            {
                s32 offset = y % 2;
                s32 pos = (y * sizeX * 6) + x * 6 + sizeX * 3;

                idx[pos + 0] = y * sizeX + x;
                idx[pos + 1] = (y + 1) * sizeX + ((x + offset) % sizeX);
                idx[pos + 2] = y * sizeX + ((x + 1) % sizeX);
                idx[pos + 3] = (y + 1) * sizeX + ((x + offset) % sizeX);
                idx[pos + 4] = (y + 1) * sizeX + ((x + 1 + offset) % sizeX);
                idx[pos + 5] = y * sizeX + ((x + 1) % sizeX);
            }
        }

        for (s32 i = 0; i < sizeX; i++)
        {
            s32 posOffs = 3 * sizeX * (sizeY - 1) * 2 + sizeX * 3;

            idx[i * 3 + 0 + posOffs] = sizeX * sizeY + 1;
            idx[i * 3 + 1 + posOffs] = sizeX * (sizeY - 1) + ((i + 1) % sizeX);
            idx[i * 3 + 2 + posOffs] = sizeX * (sizeY - 1) + i;
        }
    }
}

void PrimitiveRenderer::getDiskVertex(Vertex* vtx, u16* idx, s32 divNum)
{
    if (vtx)
    {
        for (s32 i = 0; i < divNum; i++)
        {
            f32 angle = Mathf::deg2rad(360) * i / divNum;

            vtx[i].pos.x = std::cos(angle) * 0.5f;
            vtx[i].pos.y = std::sin(angle) * 0.5f;
            vtx[i].pos.z = 0.0f;
            vtx[i].uv.x = vtx[i].pos.x;
            vtx[i].uv.y = 1.0f - vtx[i].pos.y;
            vtx[i].color.setf(1.0f, 0.0f, 0.0f, 0.0f);
        }

        {
            s32 i = divNum;

            vtx[i].pos.set(0.0f, 0.0f, 0.0f);
            vtx[i].uv.set(0.5f, 0.5f);
            vtx[i].color.setf(1.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    if (idx)
    {
        for (s32 i = 0; i < divNum; i++)
        {
            idx[i * 3 + 0] = i;
            idx[i * 3 + 1] = (i + 1) % divNum;
            idx[i * 3 + 2] = divNum;
        }
    }
}

void PrimitiveRenderer::getCylinderVertex(Vertex* vtx, u16* idx, s32 divNum)
{
    if (vtx)
    {
        for (s32 i = 0; i < divNum; i++)
        {
            f32 angle = Mathf::deg2rad(360) * i / divNum;

            vtx[i].pos.x = std::cos(angle) * 0.5f;
            vtx[i].pos.z = -std::sin(angle) * 0.5f;
            vtx[i].pos.y = 0.5f;
            vtx[i].uv.x = vtx[i].pos.x;
            vtx[i].uv.y = 1.0f - vtx[i].pos.z;
            vtx[i].color.setf(0.0f, 0.0f, 0.0f, 0.0f);

            s32 pos = i + divNum + 1;

            vtx[pos].pos.x = std::cos(angle) * 0.5f;
            vtx[pos].pos.z = -std::sin(angle) * 0.5f;
            vtx[pos].pos.y = -0.5f;
            vtx[pos].uv.x = vtx[i].pos.x;
            vtx[pos].uv.y = 1.0f - vtx[i].pos.z;
            vtx[pos].color.setf(1.0f, 0.0f, 0.0f, 0.0f);
        }

        {
            s32 pos = divNum;

            vtx[pos].pos.set(0.0f, 0.5f, 0.0f);
            vtx[pos].uv.set(0.5f, 0.5f);
            vtx[pos].color.setf(0.0f, 0.0f, 0.0f, 0.0f);
        }

        {
            s32 pos = divNum + divNum + 1;

            vtx[pos].pos.set(0.0f, -0.5f, 0.0f);
            vtx[pos].uv.set(0.5f, 0.5f);
            vtx[pos].color.setf(1.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    if (idx)
    {
        for (s32 i = 0; i < divNum; i++)
        {
            idx[i * 3 + 0] = i;
            idx[i * 3 + 1] = (i + 1) - ((i + 1) % divNum);
            idx[i * 3 + 2] = divNum;

            s32 posOffs = divNum * 3;
            idx[i * 3 + 0 + posOffs] = i + (divNum + 1);
            idx[i * 3 + 1 + posOffs] = divNum + (divNum + 1);
            idx[i * 3 + 2 + posOffs] = ((i + 1) - ((i + 1) % divNum)) + (divNum + 1);
        }

        for (s32 i = 0; i < divNum; i++)
        {
            s32 posOffs = divNum * 6;

            idx[i * 6 + 0 + posOffs] = i;
            idx[i * 6 + 1 + posOffs] = i + (divNum + 1);
            idx[i * 6 + 2 + posOffs] = (i + 1) - ((i + 1) % divNum);
            idx[i * 6 + 3 + posOffs] = (i + 1) - ((i + 1) % divNum);
            idx[i * 6 + 4 + posOffs] = i + (divNum + 1);
            idx[i * 6 + 5 + posOffs] = ((i + 1) - ((i + 1) % divNum)) + (divNum + 1);
        }
    }
}

PrimitiveRenderer::QuadArg&
PrimitiveRenderer::QuadArg::setCornerAndSize(const Vector3f& p, const Vector2f& size)
{
    mCenter.set(size.x * 0.5f + p.x,
                size.y * 0.5f + p.y,
                                p.z);
    mSize = size;
    return *this;
}

PrimitiveRenderer::QuadArg&
PrimitiveRenderer::QuadArg::setColor(const Color4f& colorT, const Color4f& colorB)
{
    mHorizontal = false;
    mColor0 = colorT;
    mColor1 = colorB;
    return *this;
}

PrimitiveRenderer::QuadArg&
PrimitiveRenderer::QuadArg::setColorHorizontal(const Color4f& colorL, const Color4f& colorR)
{
    mHorizontal = true;
    mColor0 = colorL;
    mColor1 = colorR;
    return *this;
}

PrimitiveRenderer::CubeArg&
PrimitiveRenderer::CubeArg::setCornerAndSize(const Vector3f& p, const Vector3f& size)
{
    mCenter.setScaleAdd(size, 0.5f, p);
    mSize = size;
    return *this;
}

}
