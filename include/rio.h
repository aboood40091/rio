#ifndef RIO_INIT_H
#define RIO_INIT_H

#include <task/rio_TaskMgr.h>

namespace rio {

// The created root task
extern ITask* sRootTask;

// Initialize RIO and its global managers
bool Initialize();

// Initialize RIO and its global managers, followed by the root task of type "T"
template <typename T>
bool Initialize()
{
    // Prevent double initialization
    RIO_ASSERT(sRootTask == nullptr);

    if (!Initialize())
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
