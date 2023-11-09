#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <filedevice/rio_FileDeviceMgr.h>
#include <gpu/rio_Shader.h>

#include <misc/gl/rio_GL.h>

namespace {

struct MaxUniformBufferBindingsGetter
{
    MaxUniformBufferBindingsGetter()
    {
        RIO_GL_CALL(glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &mValue));
      //RIO_ASSERT(glGetError() == GL_NO_ERROR);
    }

    s32 getValue() const
    {
        return mValue;
    }

private:
    s32 mValue;
};

}

namespace rio {

void Shader::initialize_()
{
    mShaderProgram = GL_NONE;
}

void Shader::load(const char* c_vertex_shader_src, const char* c_fragment_shader_src)
{
    unload();

    u32 vertex_shader;
    RIO_GL_CALL(vertex_shader = glCreateShader(GL_VERTEX_SHADER));
    RIO_GL_CALL(glShaderSource(vertex_shader, 1, &c_vertex_shader_src, nullptr));
    RIO_GL_CALL(glCompileShader(vertex_shader));

#ifdef RIO_DEBUG
    // Error-checking...
    {
        int  success;
        char infoLog[512];
        RIO_GL_CALL(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            RIO_GL_CALL(glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog));
            RIO_LOG("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
            RIO_ASSERT(false);
        }
    }
#endif // RIO_DEBUG

    u32 fragment_shader;
    RIO_GL_CALL(fragment_shader = glCreateShader(GL_FRAGMENT_SHADER));
    RIO_GL_CALL(glShaderSource(fragment_shader, 1, &c_fragment_shader_src, nullptr));
    RIO_GL_CALL(glCompileShader(fragment_shader));

#ifdef RIO_DEBUG
    // Error-checking...
    {
        int  success;
        char infoLog[512];
        RIO_GL_CALL(glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            RIO_GL_CALL(glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog));
            RIO_LOG("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
            RIO_ASSERT(false);
        }
    }
#endif // RIO_DEBUG

    RIO_GL_CALL(mShaderProgram = glCreateProgram());
    RIO_GL_CALL(glAttachShader(mShaderProgram, vertex_shader));
    RIO_GL_CALL(glAttachShader(mShaderProgram, fragment_shader));
    RIO_GL_CALL(glLinkProgram(mShaderProgram));

#ifdef RIO_DEBUG
    // Error-checking...
    {
        int  success;
        char infoLog[512];
        RIO_GL_CALL(glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success));
        if (!success)
        {
            RIO_GL_CALL(glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog));
            RIO_LOG("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
            RIO_ASSERT(false);
        }
    }
#endif // RIO_DEBUG

    RIO_GL_CALL(glDeleteShader(vertex_shader));
    RIO_GL_CALL(glDeleteShader(fragment_shader));

    mLoaded = true;

    s32 uniform_block_num;
    RIO_GL_CALL(glGetProgramiv(mShaderProgram, GL_ACTIVE_UNIFORM_BLOCKS, &uniform_block_num));
  //RIO_ASSERT(glGetError() == GL_NO_ERROR);

#ifdef RIO_DEBUG
    static MaxUniformBufferBindingsGetter uniform_block_max_num;
    RIO_ASSERT(uniform_block_num <= uniform_block_max_num.getValue());
#endif // RIO_DEBUG

    for (s32 i = 0; i < uniform_block_num; i++)
        RIO_GL_CALL(glUniformBlockBinding(mShaderProgram, i, i));
}

void Shader::load(const char* base_fname, ShaderMode)
{
    const std::string base_path = std::string("shaders/") + base_fname;

    char* vertex_shader_src_file;
    u32 vertex_shader_src_file_len;
    {
        FileDevice::LoadArg arg;
        arg.path = base_path + ".vert";

        vertex_shader_src_file = (char*)FileDeviceMgr::instance()->load(arg);
        vertex_shader_src_file_len = arg.read_size;
    }

    const std::string vertex_shader_src = std::string(vertex_shader_src_file, vertex_shader_src_file_len);
    const char* const c_vertex_shader_src = vertex_shader_src.c_str();

    char* fragment_shader_src_file;
    u32 fragment_shader_src_file_len;
    {
        FileDevice::LoadArg arg;
        arg.path = base_path + ".frag";

        fragment_shader_src_file = (char*)FileDeviceMgr::instance()->load(arg);
        fragment_shader_src_file_len = arg.read_size;
    }

    const std::string fragment_shader_src = std::string(fragment_shader_src_file, fragment_shader_src_file_len);
    const char* const c_fragment_shader_src = fragment_shader_src.c_str();

    load(c_vertex_shader_src, c_fragment_shader_src);

    MemUtil::free(vertex_shader_src_file);
    MemUtil::free(fragment_shader_src_file);
}

void Shader::unload()
{
    if (!mLoaded)
        return;

    RIO_GL_CALL(glDeleteProgram(mShaderProgram));
    mShaderProgram = GL_NONE;

    mLoaded = false;
}

void Shader::setShaderMode(ShaderMode, bool)
{
}

Shader::ShaderMode Shader::getShaderMode()
{
    return Shader::MODE_INVALID;
}

void Shader::bind(bool) const
{
    RIO_ASSERT(mLoaded);

    RIO_GL_CALL(glUseProgram(mShaderProgram));
}

u32 Shader::getVertexAttribLocation(const char* name) const
{
    RIO_ASSERT(mLoaded);
    u32 loc;
    RIO_GL_CALL(loc = glGetAttribLocation(mShaderProgram, name));
    return loc;
}

u32 Shader::getVertexSamplerLocation(const char* name) const
{
    RIO_ASSERT(mLoaded);
    u32 loc;
    RIO_GL_CALL(loc = glGetUniformLocation(mShaderProgram, name));
    return loc;
}

u32 Shader::getFragmentSamplerLocation(const char* name) const
{
    RIO_ASSERT(mLoaded);
    u32 loc;
    RIO_GL_CALL(loc = glGetUniformLocation(mShaderProgram, name));
    return loc;
}

u32 Shader::getVertexUniformLocation(const char* name) const
{
    RIO_ASSERT(mLoaded);
    u32 loc;
    RIO_GL_CALL(loc = glGetUniformLocation(mShaderProgram, name));
    return loc;
}

u32 Shader::getFragmentUniformLocation(const char* name) const
{
    RIO_ASSERT(mLoaded);
    u32 loc;
    RIO_GL_CALL(loc = glGetUniformLocation(mShaderProgram, name));
    return loc;
}

u32 Shader::getVertexUniformBlockIndex(const char* name) const
{
    RIO_ASSERT(mLoaded);
    u32 loc;
    RIO_GL_CALL(loc = glGetUniformBlockIndex(mShaderProgram, name));
    return loc;
}

u32 Shader::getFragmentUniformBlockIndex(const char* name) const
{
    RIO_ASSERT(mLoaded);
    u32 loc;
    RIO_GL_CALL(loc = glGetUniformBlockIndex(mShaderProgram, name));
    return loc;
}

void Shader::setUniform(f32 v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniform1f(location, v));
}

void Shader::setUniform(s32 v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniform1i(location, v));
}

void Shader::setUniform(u32 v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniform1ui(location, v));
}

void Shader::setUniform(const BaseVec2f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniform2fv(location, 1, &v.x));
}

void Shader::setUniform(const BaseVec2i& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniform2iv(location, 1, &v.x));
}

void Shader::setUniform(const BaseVec2u& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniform2uiv(location, 1, &v.x));
}

void Shader::setUniform(const BaseVec3f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniform3fv(location, 1, &v.x));
}

void Shader::setUniform(const BaseVec3i& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniform3iv(location, 1, &v.x));
}

void Shader::setUniform(const BaseVec3u& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniform3uiv(location, 1, &v.x));
}

void Shader::setUniform(const BaseVec4f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniform4fv(location, 1, &v.x));
}

void Shader::setUniform(const BaseVec4i& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniform4iv(location, 1, &v.x));
}

void Shader::setUniform(const BaseVec4u& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniform4uiv(location, 1, &v.x));
}

void Shader::setUniform(const BaseMtx22f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix2fv(location, 1, GL_TRUE, v.a));
}

void Shader::setUniformColumnMajor(const BaseMtx22f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix2fv(location, 1, GL_FALSE, v.a));
}

void Shader::setUniform(const BaseMtx23f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix3x2fv(location, 1, GL_TRUE, v.a));
}

void Shader::setUniformColumnMajor(const BaseMtx32f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix3x2fv(location, 1, GL_FALSE, v.a));
}

void Shader::setUniform(const BaseMtx24f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix4x2fv(location, 1, GL_TRUE, v.a));
}

void Shader::setUniformColumnMajor(const BaseMtx42f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix4x2fv(location, 1, GL_FALSE, v.a));
}

void Shader::setUniform(const BaseMtx32f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix2x3fv(location, 1, GL_TRUE, v.a));
}

void Shader::setUniformColumnMajor(const BaseMtx23f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix2x3fv(location, 1, GL_FALSE, v.a));
}

void Shader::setUniform(const BaseMtx33f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix3fv(location, 1, GL_TRUE, v.a));
}

void Shader::setUniformColumnMajor(const BaseMtx33f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix3fv(location, 1, GL_FALSE, v.a));
}

void Shader::setUniform(const BaseMtx34f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix4x3fv(location, 1, GL_TRUE, v.a));
}

void Shader::setUniformColumnMajor(const BaseMtx43f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix4x3fv(location, 1, GL_FALSE, v.a));
}

void Shader::setUniform(const BaseMtx42f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix2x4fv(location, 1, GL_TRUE, v.a));
}

void Shader::setUniformColumnMajor(const BaseMtx24f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix2x4fv(location, 1, GL_FALSE, v.a));
}

void Shader::setUniform(const BaseMtx43f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix3x4fv(location, 1, GL_TRUE, v.a));
}

void Shader::setUniformColumnMajor(const BaseMtx34f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix3x4fv(location, 1, GL_FALSE, v.a));
}

void Shader::setUniform(const BaseMtx44f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix4fv(location, 1, GL_TRUE, v.a));
}

void Shader::setUniformColumnMajor(const BaseMtx44f& v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, v.a));
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
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_ASSERT(count);
    RIO_ASSERT(v);

    RIO_GL_CALL(glUniform4fv(location, count, &v[0].x));
}

void Shader::setUniformArray(u32 count, const BaseVec4i* v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_ASSERT(count);
    RIO_ASSERT(v);

    RIO_GL_CALL(glUniform4iv(location, count, &v[0].x));
}

void Shader::setUniformArray(u32 count, const BaseVec4u* v, u32 vs_location, u32 fs_location)
{
    u32 location;

    if (vs_location != 0xFFFFFFFF)
    {
        if (fs_location != 0xFFFFFFFF)
            RIO_ASSERT(vs_location == fs_location);

        location = vs_location;
    }
    else
    {
        if (fs_location == 0xFFFFFFFF)
            return;

        location = fs_location;
    }

    RIO_ASSERT(count);
    RIO_ASSERT(v);

    RIO_GL_CALL(glUniform4uiv(location, count, &v[0].x));
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

#endif // RIO_IS_WIN
