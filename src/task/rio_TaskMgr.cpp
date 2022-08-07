#include <task/rio_TaskMgr.h>

namespace rio {

TaskMgr* TaskMgr::sInstance = nullptr;

bool TaskMgr::createSingleton()
{
    if (sInstance)
        return false;

    sInstance = new TaskMgr;
    return true;
}

void TaskMgr::destroySingleton()
{
    if (!sInstance)
        return;

    delete sInstance;
    sInstance = nullptr;
}

bool TaskMgr::destroyTask(ITask* task)
{
    if (changeTaskState_(task, ITask::STATE_DESTROYABLE))
    {
        if (changeTaskState_(task, ITask::STATE_DEAD))
        {
            delete task;
            return true;
        }
    }

    return false;
}

bool TaskMgr::requestDestroyTask(ITask* task)
{
    return changeTaskState_(task, ITask::STATE_DESTROYABLE);
}

void TaskMgr::calc()
{
    for (ITask::List::iterator it = mPrepareList.begin(); it != mPrepareList.end(); )
    {
        ITask* task = *it;
        task->prepare_();
        ++it;

        changeTaskState_(task, ITask::STATE_RUNNING);
    }

    for (ITask::List::iterator it = mActiveList.begin(); it != mActiveList.end(); ++it)
        (*it)->calc_();

    for (ITask::List::iterator it = mDestroyableList.begin(); it != mDestroyableList.end(); )
    {
        ITask* task = *it;
        ++it;

        if (changeTaskState_(task, ITask::STATE_DEAD))
        {
            task->~ITask();
            delete task;
        }
    }
}

bool TaskMgr::changeTaskState_(ITask* task, ITask::State state)
{
    ITask::State curr_state = task->mState;
    if (curr_state == state)
        return false;

    switch (state)
    {
    case ITask::STATE_PREPARE:
        RIO_ASSERT(curr_state == ITask::STATE_CREATED);

        task->mState = ITask::STATE_PREPARE;
        task->mTaskListNode.erase();
        mPrepareList.pushBack(&task->mTaskListNode);

        break;

    case ITask::STATE_RUNNING:
        RIO_ASSERT(curr_state == ITask::STATE_PREPARE);

        task->mState = ITask::STATE_RUNNING;
        task->mTaskListNode.erase();
        mActiveList.pushBack(&task->mTaskListNode);

        task->enter_();

        break;

    case ITask::STATE_DESTROYABLE:
        if (curr_state != ITask::STATE_RUNNING)
            return false;

        task->mState = ITask::STATE_DESTROYABLE;
        task->mTaskListNode.erase();
        mDestroyableList.pushBack(&task->mTaskListNode);

        break;

    case ITask::STATE_DEAD:
        task->exit_();

        task->mState = ITask::STATE_DEAD;
        task->mTaskListNode.erase();

        break;

    default:
        RIO_ASSERT(false);
        return false;
    }

    return true;
}

} // namespace rio
