#ifndef RIO_GL_H
#define RIO_GL_H

#include <misc/rio_Types.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef RIO_DEBUG

inline void GLClearError(const char* file, s32 line)
{
    bool printed = false;

    while (GLenum error = glGetError())
    {
        if (!printed)
        {
            RIO_LOG("File \"%s\", line %d\n", file, line);
            printed = true;
        }
        RIO_LOG("[OpenGL Error] (%u): Clear\n", error);
    }
}

inline void GLCheckError(const char* file, s32 line, const char* arg = nullptr)
{
    bool printed = false;

    while (GLenum error = glGetError())
    {
        if (!printed)
        {
            if (arg != nullptr)
            {
                RIO_LOG("File \"%s\", line %d\n%s\n", file, line, arg);
            }
            else
            {
                RIO_LOG("File \"%s\", line %d\n", file, line);
            }
            printed = true;
        }
        RIO_LOG("[OpenGL Error] (%u)\n", error);
    }

    if (printed)
    {
        RIO_ASSERT(false);
    }
}

#define RIO_GL_CHECK_ERROR() GLCheckError(__FILE__, __LINE__)

#define RIO_GL_CALL(ARG)                        \
    do                                          \
    {                                           \
        GLClearError(__FILE__, __LINE__);       \
        ARG;                                    \
        GLCheckError(__FILE__, __LINE__, #ARG); \
    } while (0)

#else

#define RIO_GL_CHECK_ERROR()
#define RIO_GL_CALL(ARG) ARG

#endif // RIO_DEBUG

#endif // RIO_GL_H
