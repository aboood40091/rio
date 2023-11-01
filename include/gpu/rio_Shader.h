#ifndef RIO_GPU_SHADER_H
#define RIO_GPU_SHADER_H

#include <math/rio_MathTypes.h>

#if RIO_IS_CAFE
typedef struct GX2VertexShader GX2VertexShader;
typedef struct GX2PixelShader GX2PixelShader;
#endif // RIO_IS_CAFE

namespace rio {

class Shader
{
public:
    enum ShaderMode
    {
        MODE_UNIFORM_REGISTER = 0,
        MODE_UNIFORM_BLOCK    = 1,
        //
        //
        MODE_COUNT            = 4,
        MODE_INVALID          = MODE_COUNT
    };

public:
    Shader()
        : mLoaded(false)
    {
        initialize_();
    }

    ~Shader()
    {
        unload();
    }

private:
    Shader(const Shader&);
    Shader& operator=(const Shader&);

public:
    // Load shader resource by filename and expected shader mode (MODE_INVALID = Don't care).
    // The expected shader mode is used on Cafe to verify that the loaded shader matches that shader mode.
    void load(const char* base_fname, ShaderMode exp_mode = MODE_INVALID);

#if RIO_IS_CAFE

    // Wrap pre-existing GX2VertexShader and GX2PixelShader instances
    void load(GX2VertexShader* p_vertex_shader, GX2PixelShader* p_pixel_shader);

#elif RIO_IS_WIN

    // Load shader resource by source strings.
    void load(const char* c_vertex_shader_src, const char* c_fragment_shader_src);

#endif

    // Unload the shader resource.
    void unload();

    // Check if the shader resource has been loaded.
    bool isLoaded() const { return mLoaded; }

#if RIO_IS_CAFE

    // Get the GX2 Vertex Shader pointer.
    GX2VertexShader* getVertexShader() const { return mpVertexShader; }
    // Get the GX2 Pixel Shader pointer.
    GX2PixelShader*  getPixelShader()  const { return mpPixelShader; }

    // Get the shader's mode.
    ShaderMode getSelfShaderMode() const { return mShaderMode; }

#elif RIO_IS_WIN

    u32 getShaderProgram() const { return mShaderProgram; }

#endif

    // Set the current global shader mode.
    static void setShaderMode(ShaderMode mode, bool force = false);
    // Get the current global shader mode.
    static ShaderMode getShaderMode();

    void bind(bool forceSetShaderMode = false) const;

    u32 getVertexAttribLocation(const char* name) const;

    u32 getVertexSamplerLocation(const char* name) const;
    u32 getFragmentSamplerLocation(const char* name) const;

    u32 getVertexUniformLocation(const char* name) const;
    u32 getFragmentUniformLocation(const char* name) const;

    u32 getVertexUniformBlockIndex(const char* name) const;
    u32 getFragmentUniformBlockIndex(const char* name) const;

    // ---------------- Uniform variables setter functions ----------------
    // (For uniform blocks, use the UniformBlock class instead)

    // GLSL type: float
    static void setUniform(f32 v, u32 vs_location, u32 fs_location);

    // GLSL type: int
    static void setUniform(s32 v, u32 vs_location, u32 fs_location);

    // GLSL type: uint
    static void setUniform(u32 v, u32 vs_location, u32 fs_location);

    // GLSL type: vec2
    static void setUniform(const BaseVec2f& v, u32 vs_location, u32 fs_location);
    static void setUniform(f32 v0, f32 v1, u32 vs_location, u32 fs_location)
    {
        setUniform((BaseVec2f){ v0, v1 }, vs_location, fs_location);
    }

    // GLSL type: ivec2
    static void setUniform(const BaseVec2i& v, u32 vs_location, u32 fs_location);
    static void setUniform(s32 v0, s32 v1, u32 vs_location, u32 fs_location)
    {
        setUniform((BaseVec2i){ v0, v1 }, vs_location, fs_location);
    }

    // GLSL type: uvec2
    static void setUniform(const BaseVec2u& v, u32 vs_location, u32 fs_location);
    static void setUniform(u32 v0, u32 v1, u32 vs_location, u32 fs_location)
    {
        setUniform((BaseVec2u){ v0, v1 }, vs_location, fs_location);
    }

    // GLSL type: vec3
    static void setUniform(const BaseVec3f& v, u32 vs_location, u32 fs_location);
    static void setUniform(f32 v0, f32 v1, f32 v2, u32 vs_location, u32 fs_location)
    {
        setUniform((BaseVec3f){ v0, v1, v2 }, vs_location, fs_location);
    }

    // GLSL type: ivec3
    static void setUniform(const BaseVec3i& v, u32 vs_location, u32 fs_location);
    static void setUniform(s32 v0, s32 v1, s32 v2, u32 vs_location, u32 fs_location)
    {
        setUniform((BaseVec3i){ v0, v1, v2 }, vs_location, fs_location);
    }

    // GLSL type: uvec3
    static void setUniform(const BaseVec3u& v, u32 vs_location, u32 fs_location);
    static void setUniform(u32 v0, u32 v1, u32 v2, u32 vs_location, u32 fs_location)
    {
        setUniform((BaseVec3u){ v0, v1, v2 }, vs_location, fs_location);
    }

    // GLSL type: vec4
    static void setUniform(const BaseVec4f& v, u32 vs_location, u32 fs_location);
    static void setUniform(f32 v0, f32 v1, f32 v2, f32 v3, u32 vs_location, u32 fs_location)
    {
        setUniform((BaseVec4f){ v0, v1, v2, v3 }, vs_location, fs_location);
    }

    // GLSL type: ivec4
    static void setUniform(const BaseVec4i& v, u32 vs_location, u32 fs_location);
    static void setUniform(s32 v0, s32 v1, s32 v2, s32 v3, u32 vs_location, u32 fs_location)
    {
        setUniform((BaseVec4i){ v0, v1, v2, v3 }, vs_location, fs_location);
    }

    // GLSL type: uvec4
    static void setUniform(const BaseVec4u& v, u32 vs_location, u32 fs_location);
    static void setUniform(u32 v0, u32 v1, u32 v2, u32 v3, u32 vs_location, u32 fs_location)
    {
        setUniform((BaseVec4u){ v0, v1, v2, v3 }, vs_location, fs_location);
    }

    // GLSL type: mtx2
    static void setUniform(const BaseMtx22f& v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx32
    static void setUniform(const BaseMtx23f& v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx42
    static void setUniform(const BaseMtx24f& v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx23
    static void setUniform(const BaseMtx32f& v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx3
    static void setUniform(const BaseMtx33f& v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx43
    static void setUniform(const BaseMtx34f& v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx24
    static void setUniform(const BaseMtx42f& v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx34
    static void setUniform(const BaseMtx43f& v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx4
    static void setUniform(const BaseMtx44f& v, u32 vs_location, u32 fs_location);

    // GLSL type: float[]
    //static void setUniformArray(u32 count, f32* v, u32 vs_location, u32 fs_location);

    // GLSL type: int[]
    //static void setUniformArray(u32 count, s32* v, u32 vs_location, u32 fs_location);

    // GLSL type: uint[]
    //static void setUniformArray(u32 count, u32* v, u32 vs_location, u32 fs_location);

    // GLSL type: vec2[]
    //static void setUniformArray(u32 count, const BaseVec2f* v, u32 vs_location, u32 fs_location);

    // GLSL type: ivec2[]
    //static void setUniformArray(u32 count, const BaseVec2i* v, u32 vs_location, u32 fs_location);

    // GLSL type: uvec2[]
    //static void setUniformArray(u32 count, const BaseVec2u* v, u32 vs_location, u32 fs_location);

    // GLSL type: vec3[]
    //static void setUniformArray(u32 count, const BaseVec3f* v, u32 vs_location, u32 fs_location);

    // GLSL type: ivec3[]
    //static void setUniformArray(u32 count, const BaseVec3i* v, u32 vs_location, u32 fs_location);

    // GLSL type: uvec3[]
    //static void setUniformArray(u32 count, const BaseVec3u* v, u32 vs_location, u32 fs_location);

    // GLSL type: vec4[]
    static void setUniformArray(u32 count, const BaseVec4f* v, u32 vs_location, u32 fs_location);

    // GLSL type: ivec4[]
    static void setUniformArray(u32 count, const BaseVec4i* v, u32 vs_location, u32 fs_location);

    // GLSL type: uvec4[]
    static void setUniformArray(u32 count, const BaseVec4u* v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx2[]
    //static void setUniformArray(u32 count, const BaseMtx22f* v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx32[]
    //static void setUniformArray(u32 count, const BaseMtx23f* v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx42[]
    //static void setUniformArray(u32 count, const BaseMtx24f* v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx23[]
    //static void setUniformArray(u32 count, const BaseMtx32f* v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx3[]
    //static void setUniformArray(u32 count, const BaseMtx33f* v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx43[]
    //static void setUniformArray(u32 count, const BaseMtx34f* v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx24[]
    //static void setUniformArray(u32 count, const BaseMtx42f* v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx34[]
    //static void setUniformArray(u32 count, const BaseMtx43f* v, u32 vs_location, u32 fs_location);

    // GLSL type: mtx4[]
    //static void setUniformArray(u32 count, const BaseMtx44f* v, u32 vs_location, u32 fs_location);

private:
    void initialize_();

private:
    bool                mLoaded;
#if RIO_IS_CAFE
    bool                mSelfAllocated;
    GX2VertexShader*    mpVertexShader;
    GX2PixelShader*     mpPixelShader;
    ShaderMode          mShaderMode;
    static ShaderMode   sCurrentShaderMode;
#elif RIO_IS_WIN
    u32                 mShaderProgram;
#endif
};

}

#endif // RIO_GPU_SHADER_H
