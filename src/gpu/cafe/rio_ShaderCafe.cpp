#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <filedevice/rio_FileDeviceMgr.h>
#include <gpu/rio_Shader.h>

#include <whb/gfx.h>

namespace {

static inline GX2AttribVar*
GX2GetVertexAttribVar(const GX2VertexShader* shader,
                      const char* name)
{
    for (u32 i = 0; i < shader->attribVarCount; i++)
    {
       if (strcmp(shader->attribVars[i].name, name) == 0)
           return &shader->attribVars[i];
    }

    return nullptr;
}

static inline s32
GX2GetVertexAttribVarLocation(const GX2VertexShader* shader,
                              const char* name)
{
    GX2AttribVar* attrib_var = GX2GetVertexAttribVar(shader, name);
    if (!attrib_var)
        return -1;

    return (s32)attrib_var->location;
}

static inline GX2SamplerVar*
GX2GetVertexSamplerVar(const GX2VertexShader* shader,
                       const char* name)
{
    for (u32 i = 0; i < shader->samplerVarCount; i++)
    {
       if (strcmp(shader->samplerVars[i].name, name) == 0)
           return &shader->samplerVars[i];
    }

    return nullptr;
}

static inline s32
GX2GetVertexSamplerVarLocation(const GX2VertexShader* shader,
                               const char* name)
{
    GX2SamplerVar* sampler_var = GX2GetVertexSamplerVar(shader, name);
    if (!sampler_var)
        return -1;

    return (s32)sampler_var->location;
}

static inline GX2SamplerVar*
GX2GetPixelSamplerVar(const GX2PixelShader* shader,
                      const char* name)
{
    for (u32 i = 0; i < shader->samplerVarCount; i++)
    {
       if (strcmp(shader->samplerVars[i].name, name) == 0)
           return &shader->samplerVars[i];
    }

    return nullptr;
}

static inline s32
GX2GetPixelSamplerVarLocation(const GX2PixelShader* shader,
                              const char* name)
{
    GX2SamplerVar* sampler_var = GX2GetPixelSamplerVar(shader, name);
    if (!sampler_var)
        return -1;

    return (s32)sampler_var->location;
}

static inline s32
GX2GetVertexUniformVarOffset(const GX2VertexShader* shader,
                             const char* name)
{
    GX2UniformVar* uniform_var = GX2GetVertexUniformVar(shader, name);
    if (!uniform_var)
        return -1;

    return (s32)uniform_var->offset;
}

static inline s32
GX2GetPixelUniformVarOffset(const GX2PixelShader* shader,
                            const char* name)
{
    GX2UniformVar* uniform_var = GX2GetPixelUniformVar(shader, name);
    if (!uniform_var)
        return -1;

    return (s32)uniform_var->offset;
}

}

namespace rio {

Shader::ShaderMode Shader::sCurrentShaderMode = Shader::MODE_INVALID;

void Shader::initialize_()
{
    mpVertexShader = nullptr;
    mpPixelShader  = nullptr;
}

void Shader::load(const char* base_fname, ShaderMode exp_mode)
{
    unload();

    if (exp_mode != MODE_INVALID)
    {
        RIO_ASSERT(exp_mode == MODE_UNIFORM_REGISTER || exp_mode == MODE_UNIFORM_BLOCK);
    }

    FileDevice::LoadArg arg;
    arg.path = std::string("shaders/") + base_fname + ".gsh";

    u8* file = FileDeviceMgr::instance()->load(arg);

    mpVertexShader = WHBGfxLoadGFDVertexShader(0, file);
    RIO_ASSERT(mpVertexShader);

    if (exp_mode == MODE_INVALID)
    {
        RIO_ASSERT(mpVertexShader->mode == GX2_SHADER_MODE_UNIFORM_REGISTER || mpVertexShader->mode == GX2_SHADER_MODE_UNIFORM_BLOCK);
        exp_mode = (ShaderMode)mpVertexShader->mode;
    }
    else
    {
        RIO_ASSERT(mpVertexShader->mode == (GX2ShaderMode)exp_mode);
    }

    mpPixelShader = WHBGfxLoadGFDPixelShader(0, file);
    RIO_ASSERT(mpPixelShader);
    RIO_ASSERT(mpPixelShader->mode == (GX2ShaderMode)exp_mode);

    MemUtil::free(file);

    mShaderMode = exp_mode;
    mLoaded = true;
}

void Shader::unload()
{
    if (!mLoaded)
        return;

    WHBGfxFreeVertexShader(mpVertexShader);
    mpVertexShader = nullptr;

    WHBGfxFreePixelShader(mpPixelShader);
    mpPixelShader = nullptr;

    mLoaded = false;
}

void Shader::setShaderMode(ShaderMode mode)
{
    RIO_ASSERT(mode == MODE_UNIFORM_REGISTER || mode == MODE_UNIFORM_BLOCK);

    if (sCurrentShaderMode != mode)
    {
        sCurrentShaderMode = mode;
        GX2SetShaderModeEx((GX2ShaderMode)mode, 48, 64, 0, 0, 200, 192);
    }
}

Shader::ShaderMode Shader::getShaderMode()
{
    return sCurrentShaderMode;
}

void Shader::bind()
{
    RIO_ASSERT(mLoaded);

    setShaderMode(mShaderMode);

    GX2SetVertexShader(mpVertexShader);
    GX2SetPixelShader(mpPixelShader);
}

u32 Shader::getVertexAttribLocation(const char* name)
{
    RIO_ASSERT(mLoaded);
    return GX2GetVertexAttribVarLocation(mpVertexShader, name);
}

u32 Shader::getVertexSamplerLocation(const char* name)
{
    RIO_ASSERT(mLoaded);
    return GX2GetVertexSamplerVarLocation(mpVertexShader, name);
}

u32 Shader::getFragmentSamplerLocation(const char* name)
{
    RIO_ASSERT(mLoaded);
    return GX2GetPixelSamplerVarLocation(mpPixelShader, name);
}

u32 Shader::getVertexUniformLocation(const char* name)
{
    RIO_ASSERT(mLoaded);
    return GX2GetVertexUniformVarOffset(mpVertexShader, name);
}

u32 Shader::getFragmentUniformLocation(const char* name)
{
    RIO_ASSERT(mLoaded);
    return GX2GetPixelUniformVarOffset(mpPixelShader, name);
}

void Shader::setUniform(f32 v, u32 vs_location, u32 fs_location)
{
    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, 1, &v);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, 1, &v);
}

void Shader::setUniform(s32 v, u32 vs_location, u32 fs_location)
{
    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, 1, &v);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, 1, &v);
}

void Shader::setUniform(u32 v, u32 vs_location, u32 fs_location)
{
    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, 1, &v);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, 1, &v);
}

void Shader::setUniform(const BaseVec2f& v, u32 vs_location, u32 fs_location)
{
    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, 2, &v.x);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, 2, &v.x);
}

void Shader::setUniform(const BaseVec2i& v, u32 vs_location, u32 fs_location)
{
    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, 2, &v.x);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, 2, &v.x);
}

void Shader::setUniform(const BaseVec2u& v, u32 vs_location, u32 fs_location)
{
    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, 2, &v.x);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, 2, &v.x);
}

void Shader::setUniform(const BaseVec3f& v, u32 vs_location, u32 fs_location)
{
    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, 3, &v.x);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, 3, &v.x);
}

void Shader::setUniform(const BaseVec3i& v, u32 vs_location, u32 fs_location)
{
    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, 3, &v.x);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, 3, &v.x);
}

void Shader::setUniform(const BaseVec3u& v, u32 vs_location, u32 fs_location)
{
    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, 3, &v.x);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, 3, &v.x);
}

void Shader::setUniform(const BaseVec4f& v, u32 vs_location, u32 fs_location)
{
    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, 4, &v.x);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, 4, &v.x);
}

void Shader::setUniform(const BaseVec4i& v, u32 vs_location, u32 fs_location)
{
    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, 4, &v.x);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, 4, &v.x);
}

void Shader::setUniform(const BaseVec4u& v, u32 vs_location, u32 fs_location)
{
    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, 4, &v.x);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, 4, &v.x);
}

void Shader::setUniform(const BaseMtx22f& v, u32 vs_location, u32 fs_location)
{
    if (vs_location == 0xFFFFFFFF && fs_location == 0xFFFFFFFF)
        return;

    const u32 NUM_COL = 2;

    f32 mat_trans[NUM_COL][4] = {
        { v.m[0][0], v.m[1][0] },
        { v.m[0][1], v.m[1][1] }
    };

    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, NUM_COL*4, mat_trans);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, NUM_COL*4, mat_trans);
}

void Shader::setUniform(const BaseMtx23f& v, u32 vs_location, u32 fs_location)
{
    if (vs_location == 0xFFFFFFFF && fs_location == 0xFFFFFFFF)
        return;

    const u32 NUM_COL = 3;

    f32 mat_trans[NUM_COL][4] = {
        { v.m[0][0], v.m[1][0] },
        { v.m[0][1], v.m[1][1] },
        { v.m[0][2], v.m[1][2] }
    };

    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, NUM_COL*4, mat_trans);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, NUM_COL*4, mat_trans);
}

void Shader::setUniform(const BaseMtx24f& v, u32 vs_location, u32 fs_location)
{
    if (vs_location == 0xFFFFFFFF && fs_location == 0xFFFFFFFF)
        return;

    const u32 NUM_COL = 4;

    f32 mat_trans[NUM_COL][4] = {
        { v.m[0][0], v.m[1][0] },
        { v.m[0][1], v.m[1][1] },
        { v.m[0][2], v.m[1][2] },
        { v.m[0][3], v.m[1][3] }
    };

    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, NUM_COL*4, mat_trans);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, NUM_COL*4, mat_trans);
}

void Shader::setUniform(const BaseMtx32f& v, u32 vs_location, u32 fs_location)
{
    if (vs_location == 0xFFFFFFFF && fs_location == 0xFFFFFFFF)
        return;

    const u32 NUM_COL = 2;

    f32 mat_trans[NUM_COL][4] = {
        { v.m[0][0], v.m[1][0], v.m[2][0] },
        { v.m[0][1], v.m[1][1], v.m[2][1] }
    };

    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, NUM_COL*4, mat_trans);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, NUM_COL*4, mat_trans);
}

void Shader::setUniform(const BaseMtx33f& v, u32 vs_location, u32 fs_location)
{
    if (vs_location == 0xFFFFFFFF && fs_location == 0xFFFFFFFF)
        return;

    const u32 NUM_COL = 3;

    f32 mat_trans[NUM_COL][4] = {
        { v.m[0][0], v.m[1][0], v.m[2][0] },
        { v.m[0][1], v.m[1][1], v.m[2][1] },
        { v.m[0][2], v.m[1][2], v.m[2][2] }
    };

    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, NUM_COL*4, mat_trans);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, NUM_COL*4, mat_trans);
}

void Shader::setUniform(const BaseMtx34f& v, u32 vs_location, u32 fs_location)
{
    if (vs_location == 0xFFFFFFFF && fs_location == 0xFFFFFFFF)
        return;

    const u32 NUM_COL = 4;

    f32 mat_trans[NUM_COL][4] = {
        { v.m[0][0], v.m[1][0], v.m[2][0] },
        { v.m[0][1], v.m[1][1], v.m[2][1] },
        { v.m[0][2], v.m[1][2], v.m[2][2] },
        { v.m[0][3], v.m[1][3], v.m[2][3] }
    };

    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, NUM_COL*4, mat_trans);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, NUM_COL*4, mat_trans);
}

void Shader::setUniform(const BaseMtx42f& v, u32 vs_location, u32 fs_location)
{
    if (vs_location == 0xFFFFFFFF && fs_location == 0xFFFFFFFF)
        return;

    const u32 NUM_COL = 2;

    f32 mat_trans[NUM_COL][4] = {
        { v.m[0][0], v.m[1][0], v.m[2][0], v.m[3][0] },
        { v.m[0][1], v.m[1][1], v.m[2][1], v.m[3][1] }
    };

    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, NUM_COL*4, mat_trans);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, NUM_COL*4, mat_trans);
}

void Shader::setUniform(const BaseMtx43f& v, u32 vs_location, u32 fs_location)
{
    if (vs_location == 0xFFFFFFFF && fs_location == 0xFFFFFFFF)
        return;

    const u32 NUM_COL = 3;

    f32 mat_trans[NUM_COL][4] = {
        { v.m[0][0], v.m[1][0], v.m[2][0], v.m[3][0] },
        { v.m[0][1], v.m[1][1], v.m[2][1], v.m[3][1] },
        { v.m[0][2], v.m[1][2], v.m[2][2], v.m[3][2] }
    };

    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, NUM_COL*4, mat_trans);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, NUM_COL*4, mat_trans);
}

void Shader::setUniform(const BaseMtx44f& v, u32 vs_location, u32 fs_location)
{
    if (vs_location == 0xFFFFFFFF && fs_location == 0xFFFFFFFF)
        return;

    const u32 NUM_COL = 4;

    f32 mat_trans[NUM_COL][4] = {
        { v.m[0][0], v.m[1][0], v.m[2][0], v.m[3][0] },
        { v.m[0][1], v.m[1][1], v.m[2][1], v.m[3][1] },
        { v.m[0][2], v.m[1][2], v.m[2][2], v.m[3][2] },
        { v.m[0][3], v.m[1][3], v.m[2][3], v.m[3][3] }
    };

    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, NUM_COL*4, mat_trans);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, NUM_COL*4, mat_trans);
}

// GLSL type: float[]
//void Shader::setUniformArray(u32 count, f32* v, u32 vs_location, u32 fs_location)

// GLSL type: int[]
//void Shader::setUniformArray(u32 count, s32* v, u32 vs_location, u32 fs_location)

// GLSL type: uint[]
//void Shader::setUniformArray(u32 count, u32* v, u32 vs_location, u32 fs_location)

// GLSL type: vec2[]
//void Shader::setUniformArray(u32 count, const BaseVec2f* v, u32 vs_location, u32 fs_location)

// GLSL type: ivec2[]
//void Shader::setUniformArray(u32 count, const BaseVec2i* v, u32 vs_location, u32 fs_location)

// GLSL type: uvec2[]
//void Shader::setUniformArray(u32 count, const BaseVec2u* v, u32 vs_location, u32 fs_location)

// GLSL type: vec3[]
//void Shader::setUniformArray(u32 count, const BaseVec3f* v, u32 vs_location, u32 fs_location)

// GLSL type: ivec3[]
//void Shader::setUniformArray(u32 count, const BaseVec3i* v, u32 vs_location, u32 fs_location)

// GLSL type: uvec3[]
//void Shader::setUniformArray(u32 count, const BaseVec3u* v, u32 vs_location, u32 fs_location)

void Shader::setUniformArray(u32 count, const BaseVec4f* v, u32 vs_location, u32 fs_location)
{
    RIO_ASSERT(count);
    RIO_ASSERT(v);

    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, count*4, &v[0].x);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, count*4, &v[0].x);
}

void Shader::setUniformArray(u32 count, const BaseVec4i* v, u32 vs_location, u32 fs_location)
{
    RIO_ASSERT(count);
    RIO_ASSERT(v);

    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, count*4, &v[0].x);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, count*4, &v[0].x);
}

void Shader::setUniformArray(u32 count, const BaseVec4u* v, u32 vs_location, u32 fs_location)
{
    RIO_ASSERT(count);
    RIO_ASSERT(v);

    if (vs_location != 0xFFFFFFFF)
        GX2SetVertexUniformReg(vs_location, count*4, &v[0].x);

    if (fs_location != 0xFFFFFFFF)
        GX2SetPixelUniformReg(fs_location, count*4, &v[0].x);
}

// GLSL type: mtx2[]
//void Shader::setUniformArray(u32 count, const BaseMtx22f* v, u32 vs_location, u32 fs_location)

// GLSL type: mtx32[]
//void Shader::setUniformArray(u32 count, const BaseMtx23f* v, u32 vs_location, u32 fs_location)

// GLSL type: mtx42[]
//void Shader::setUniformArray(u32 count, const BaseMtx24f* v, u32 vs_location, u32 fs_location)

// GLSL type: mtx23[]
//void Shader::setUniformArray(u32 count, const BaseMtx32f* v, u32 vs_location, u32 fs_location)

// GLSL type: mtx3[]
//void Shader::setUniformArray(u32 count, const BaseMtx33f* v, u32 vs_location, u32 fs_location)

// GLSL type: mtx43[]
//void Shader::setUniformArray(u32 count, const BaseMtx34f* v, u32 vs_location, u32 fs_location)

// GLSL type: mtx24[]
//void Shader::setUniformArray(u32 count, const BaseMtx42f* v, u32 vs_location, u32 fs_location)

// GLSL type: mtx34[]
//void Shader::setUniformArray(u32 count, const BaseMtx43f* v, u32 vs_location, u32 fs_location)

// GLSL type: mtx4[]
//void Shader::setUniformArray(u32 count, const BaseMtx44f* v, u32 vs_location, u32 fs_location)

}

#endif // RIO_IS_CAFE
