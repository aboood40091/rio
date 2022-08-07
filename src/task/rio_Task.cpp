#include <task/rio_Task.h>

namespace rio {

ITask::ITask(const char* name)
    : mTaskListNode(this)
    , mName(name)
    , mState(STATE_CREATED)
{
}

} // namespace rio
