#ifndef RIO_TASK_H
#define RIO_TASK_H

#include <misc/rio_BitFlag.h>
#include <container/rio_TList.h>

namespace rio {

class ITask
{
public:
    typedef TListNode<ITask*> ListNode;
    typedef TList<ITask*> List;

public:
    enum State
    {
        STATE_CREATED,
        STATE_PREPARE,
        STATE_RUNNING,
        STATE_DESTROYABLE,
        STATE_DEAD
    };

protected:
    ITask(const char* name);

    virtual ~ITask()
    {
    }

    virtual void prepare_()
    {
    }

    virtual void enter_()
    {
    }

    virtual void exit_()
    {
    }

    virtual void calc_()
    {
    }

public:
    void setName(const char* name) { mName = name; }
    const char* getName() const { return mName; }

    State getState() const { return mState; }

protected:
    ListNode    mTaskListNode;
    const char* mName;
    State       mState;

    friend class TaskMgr;
};

} // namespace rio

#endif // RIO_TASK_H
