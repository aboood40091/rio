#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <filedevice/rio_FileDeviceMgr.h>
#include <gpu/rio_Shader.h>

#include <GL/glew.h>

namespace {

struct MaxUniformBufferBindingsGetter
{
    MaxUniformBufferBindingsGetter()
    {
        glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &mValue);
        RIO_ASSERT(glGetError() == GL_NO_ERROR);
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

void Shader::load(const char* base_fname, ShaderMode)
{
    unload();

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

    u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &c_vertex_shader_src, nullptr);
    glCompileShader(vertex_shader);

#ifdef RIO_DEBUG
    // Error-checking...
    {
        int  success;
        char infoLog[512];
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
            RIO_LOG("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
            RIO_ASSERT(false);
        }
    }
#endif // RIO_DEBUG

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

    u32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &c_fragment_shader_src, nullptr);
    glCompileShader(fragment_shader);

#ifdef RIO_DEBUG
    // Error-checking...
    {
        int  success;
        char infoLog[512];
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
            RIO_LOG("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
            RIO_ASSERT(false);
        }
    }
#endif // RIO_DEBUG

    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, vertex_shader);
    glAttachShader(mShaderProgram, fragment_shader);
    glLinkProgram(mShaderProgram);

#ifdef RIO_DEBUG
    // Error-checking...
    {
        int  success;
        char infoLog[512];
        glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog);
            RIO_LOG("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
            RIO_ASSERT(false);
        }
    }
#endif // RIO_DEBUG

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    MemUtil::free(vertex_shader_src_file);
    MemUtil::free(fragment_shader_src_file);

    mLoaded = true;

    s32 uniform_block_num;
    glGetProgramiv(mShaderProgram, GL_ACTIVE_UNIFORM_BLOCKS, &uniform_block_num);
    RIO_ASSERT(glGetError() == GL_NO_ERROR);

#ifdef RIO_DEBUG
    static MaxUniformBufferBindingsGetter uniform_block_max_num;
    RIO_ASSERT(uniform_block_num <= uniform_block_max_num.getValue());
#endif // RIO_DEBUG

    for (s32 i = 0; i < uniform_block_num; i++)
        glUniformBlockBinding(mShaderProgram, i, i);
}

void Shader::unload()
{
    if (!mLoaded)
        return;

    glDeleteProgram(mShaderProgram);
    mShaderProgram = GL_NONE;

    mLoaded = false;
}

void Shader::setShaderMode(ShaderMode)
{
}

Shader::ShaderMode Shader::getShaderMode()
{
    return Shader::MODE_INVALID;
}

void Shader::bind() const
{
    RIO_ASSERT(mLoaded);

    glUseProgram(mShaderProgram);
}

u32 Shader::getVertexAttribLocation(const char* name) const
{
    RIO_ASSERT(mLoaded);
    return glGetAttribLocation(mShaderProgram, name);
}

u32 Shader::getVertexSamplerLocation(const char* name) const
{
    RIO_ASSERT(mLoaded);
    return glGetUniformLocation(mShaderProgram, name);
}

u32 Shader::getFragmentSamplerLocation(const char* name) const
{
    RIO_ASSERT(mLoaded);
    return glGetUniformLocation(mShaderProgram, name);
}

u32 Shader::getVertexUniformLocation(const char* name) const
{
    RIO_ASSERT(mLoaded);
    return glGetUniformLocation(mShaderProgram, name);
}

u32 Shader::getFragmentUniformLocation(const char* name) const
{
    RIO_ASSERT(mLoaded);
    return glGetUniformLocation(mShaderProgram, name);
}

u32 Shader::getVertexUniformBlockIndex(const char* name) const
{
    RIO_ASSERT(mLoaded);
    return glGetUniformBlockIndex(mShaderProgram, name);
}

u32 Shader::getFragmentUniformBlockIndex(const char* name) const
{
    RIO_ASSERT(mLoaded);
    return glGetUniformBlockIndex(mShaderProgram, name);
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

    glUniform1f(location, v);
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

    glUniform1i(location, v);
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

    glUniform1ui(location, v);
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

    glUniform2fv(location, 1, &v.x);
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

    glUniform2iv(location, 1, &v.x);
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

    glUniform2uiv(location, 1, &v.x);
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

    glUniform3fv(location, 1, &v.x);
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

    glUniform3iv(location, 1, &v.x);
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

    glUniform3uiv(location, 1, &v.x);
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

    glUniform4fv(location, 1, &v.x);
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

    glUniform4iv(location, 1, &v.x);
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

    glUniform4uiv(location, 1, &v.x);
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

    glUniformMatrix2fv(location, 1, GL_TRUE, v.a);
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

    glUniformMatrix3x2fv(location, 1, GL_TRUE, v.a);
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

    glUniformMatrix4x2fv(location, 1, GL_TRUE, v.a);
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

    glUniformMatrix2x3fv(location, 1, GL_TRUE, v.a);
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

    glUniformMatrix3fv(location, 1, GL_TRUE, v.a);
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

    glUniformMatrix4x3fv(location, 1, GL_TRUE, v.a);
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

    glUniformMatrix2x4fv(location, 1, GL_TRUE, v.a);
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

    glUniformMatrix3x4fv(location, 1, GL_TRUE, v.a);
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

    glUniformMatrix4fv(location, 1, GL_TRUE, v.a);
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

    glUniform4fv(location, count, &v[0].x);
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

    glUniform4iv(location, count, &v[0].x);
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

    glUniform4uiv(location, count, &v[0].x);
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
