#ifndef RIO_INIT_H
#define RIO_INIT_H

#include <task/rio_TaskMgr.h>

namespace rio {

// The created root task
extern ITask* sRootTask;

struct InitializeArg
{
    struct
    {
        u32 width = 1280;
        u32 height = 720;
#if RIO_IS_WIN
        bool resizable = false;
#endif // RIO_IS_WIN
    } window;
    struct
    {
        const char* shader_path = "primitive_renderer";
    } primitive_renderer;
};

extern const InitializeArg cDefaultInitializeArg;

// Initialize RIO and its global managers
bool Initialize(const InitializeArg& arg = cDefaultInitializeArg);

// Initialize RIO and its global managers, followed by the root task of type "T"
template <typename T>
bool Initialize(const InitializeArg& arg = cDefaultInitializeArg)
{
    // Prevent double initialization
    RIO_ASSERT(sRootTask == nullptr);

    if (!Initialize(arg))
        return false;

    // Create the root task
    sRootTask = TaskMgr::instance()->createTask<T>();

    return true;
}

// Enters RIO's main loop, which executes all tasks and then renders all layers
void EnterMainLoop();

// Terminate RIO and its global managers
void Exit();

} // namespace rio

#endif // RIO_INIT_H
