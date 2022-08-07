#ifndef RIO_TASK_MGR_H
#define RIO_TASK_MGR_H

#include <task/rio_Task.h>

namespace rio {

class TaskMgr
{
public:
    static bool createSingleton();
    static void destroySingleton();
    static TaskMgr* instance() { return sInstance; }

private:
    static TaskMgr* sInstance;

    TaskMgr() { }
    ~TaskMgr() { }

    TaskMgr(const TaskMgr&);
    TaskMgr& operator=(const TaskMgr&);

public:
    template <typename T>
    T* createTask();

    bool destroyTask(ITask* task);
    bool requestDestroyTask(ITask* task);

    void calc();

private:
    bool changeTaskState_(ITask* task, ITask::State state);

private:
    ITask::List  mPrepareList;
    ITask::List  mActiveList;
    ITask::List  mDestroyableList;
};

template <typename T>
T* TaskMgr::createTask()
{
    T* task = new T;
    changeTaskState_(task, ITask::STATE_PREPARE);
    return task;
}

} // namespace rio

#endif // RIO_TASK_MGR_H
