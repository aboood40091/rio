#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gpu/rio_TextureSampler.h>

namespace rio {

TextureSampler2D::TextureSampler2D()
{
    init_();

    glGenSamplers(1, &mSamplerInner);
    RIO_ASSERT(mSamplerInner != GL_NONE);
}

TextureSampler2D::~TextureSampler2D()
{
    if (mSamplerInner != GL_NONE)
    {
        glDeleteSamplers(1, &mSamplerInner);
        mSamplerInner = GL_NONE;
    }
}

void TextureSampler2D::updateFilter_() const
{
    switch (mMinFilter)
    {
    case TEX_XY_FILTER_MODE_POINT:
        switch (mMipFilter)
        {
        case TEX_MIP_FILTER_MODE_NONE:
            glSamplerParameteri(mSamplerInner, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
        case TEX_MIP_FILTER_MODE_POINT:
            glSamplerParameteri(mSamplerInner, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            break;
        case TEX_MIP_FILTER_MODE_LINEAR:
            glSamplerParameteri(mSamplerInner, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            break;
        }
        break;
    case TEX_XY_FILTER_MODE_LINEAR:
        switch (mMipFilter)
        {
        case TEX_MIP_FILTER_MODE_NONE:
            glSamplerParameteri(mSamplerInner, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        case TEX_MIP_FILTER_MODE_POINT:
            glSamplerParameteri(mSamplerInner, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            break;
        case TEX_MIP_FILTER_MODE_LINEAR:
            glSamplerParameteri(mSamplerInner, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        }
        break;
    }

    switch (mMagFilter)
    {
    case TEX_XY_FILTER_MODE_POINT:
        switch (mMipFilter)
        {
        case TEX_MIP_FILTER_MODE_NONE:
            glSamplerParameteri(mSamplerInner, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case TEX_MIP_FILTER_MODE_POINT:
            glSamplerParameteri(mSamplerInner, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            break;
        case TEX_MIP_FILTER_MODE_LINEAR:
            glSamplerParameteri(mSamplerInner, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            break;
        }
        break;
    case TEX_XY_FILTER_MODE_LINEAR:
        switch (mMipFilter)
        {
        case TEX_MIP_FILTER_MODE_NONE:
            glSamplerParameteri(mSamplerInner, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case TEX_MIP_FILTER_MODE_POINT:
            glSamplerParameteri(mSamplerInner, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            break;
        case TEX_MIP_FILTER_MODE_LINEAR:
            glSamplerParameteri(mSamplerInner, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        }
        break;
    }

    switch (mMaxAniso)
    {
    case TEX_ANISO_1_TO_1:
        glSamplerParameterf(mSamplerInner, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
        break;
    case TEX_ANISO_2_TO_1:
        glSamplerParameterf(mSamplerInner, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.0f);
        break;
    case TEX_ANISO_4_TO_1:
        glSamplerParameterf(mSamplerInner, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
        break;
    case TEX_ANISO_8_TO_1:
        glSamplerParameterf(mSamplerInner, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
        break;
    case TEX_ANISO_16_TO_1:
        glSamplerParameterf(mSamplerInner, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
        break;
    }
}

void TextureSampler2D::updateWrap_() const
{
    switch (mWrapX)
    {
    case TEX_WRAP_MODE_REPEAT:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_S, GL_REPEAT);
        break;
    case TEX_WRAP_MODE_MIRROR:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        break;
    case TEX_WRAP_MODE_CLAMP:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        break;
    case TEX_WRAP_MODE_MIRROR_ONCE:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE_EXT);
        break;
    case TEX_WRAP_MODE_CLAMP_HALF_BORDER:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_S, GL_CLAMP);
        break;
    case TEX_WRAP_MODE_MIRROR_ONCE_HALF_BORDER:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_EXT);
        break;
    case TEX_WRAP_MODE_CLAMP_BORDER:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        break;
    case TEX_WRAP_MODE_MIRROR_ONCE_BORDER:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_BORDER_EXT);
        break;
    }

    switch (mWrapY)
    {
    case TEX_WRAP_MODE_REPEAT:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    case TEX_WRAP_MODE_MIRROR:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        break;
    case TEX_WRAP_MODE_CLAMP:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;
    case TEX_WRAP_MODE_MIRROR_ONCE:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE_EXT);
        break;
    case TEX_WRAP_MODE_CLAMP_HALF_BORDER:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_T, GL_CLAMP);
        break;
    case TEX_WRAP_MODE_MIRROR_ONCE_HALF_BORDER:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_EXT);
        break;
    case TEX_WRAP_MODE_CLAMP_BORDER:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        break;
    case TEX_WRAP_MODE_MIRROR_ONCE_BORDER:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_BORDER_EXT);
        break;
    }

    switch (mWrapZ)
    {
    case TEX_WRAP_MODE_REPEAT:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_R, GL_REPEAT);
        break;
    case TEX_WRAP_MODE_MIRROR:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
        break;
    case TEX_WRAP_MODE_CLAMP:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        break;
    case TEX_WRAP_MODE_MIRROR_ONCE:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_R, GL_MIRROR_CLAMP_TO_EDGE_EXT);
        break;
    case TEX_WRAP_MODE_CLAMP_HALF_BORDER:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_R, GL_CLAMP);
        break;
    case TEX_WRAP_MODE_MIRROR_ONCE_HALF_BORDER:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_R, GL_MIRROR_CLAMP_EXT);
        break;
    case TEX_WRAP_MODE_CLAMP_BORDER:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        break;
    case TEX_WRAP_MODE_MIRROR_ONCE_BORDER:
        glSamplerParameteri(mSamplerInner, GL_TEXTURE_WRAP_R, GL_MIRROR_CLAMP_TO_BORDER_EXT);
        break;
    }
}

void TextureSampler2D::updateBorderColor_() const
{
    glSamplerParameterfv(mSamplerInner, GL_TEXTURE_BORDER_COLOR, mBorderColor);
}

void TextureSampler2D::updateLOD_() const
{
    glSamplerParameterf(mSamplerInner, GL_TEXTURE_MIN_LOD, mMinLOD);
    glSamplerParameterf(mSamplerInner, GL_TEXTURE_MAX_LOD, mMaxLOD);
    glSamplerParameterf(mSamplerInner, GL_TEXTURE_LOD_BIAS, mLODBias);
}

void TextureSampler2D::bind(u32 vs_location, u32 fs_location, u32 slot) const
{
    RIO_ASSERT(vs_location == fs_location);
    u32 location = vs_location;

    RIO_ASSERT(location != 0xFFFFFFFF);
    RIO_ASSERT(isBindable());

    update();

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, mpTexture2D->mHandle);

    glBindSampler(slot, mSamplerInner);
    glUniform1i(location, slot);
}

}

#endif // RIO_IS_WIN
