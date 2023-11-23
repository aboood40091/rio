#ifndef RIO_CONTROLLER_MGR_H
#define RIO_CONTROLLER_MGR_H

#include <controller/rio_Controller.h>
#include <task/rio_Task.h>

#include <vector>

namespace rio {

class ControlDevice;
class Controller;
class TaskMgr;

class ControllerMgr : public ITask
{
public:
#if RIO_IS_CAFE
    static constexpr ControllerDefine::ControllerId cMainControllerId   = ControllerDefine::CONTROLLER_CAFE_DRC;
    static constexpr ControllerDefine::ControllerId cMainGamepadId      = ControllerDefine::CONTROLLER_CAFE_DRC;
    static constexpr ControllerDefine::ControllerId cMainPointerId      = ControllerDefine::CONTROLLER_CAFE_DRC;
    static constexpr ControllerDefine::ControllerId cGamepadId          = ControllerDefine::CONTROLLER_CAFE_REMOTE;
#elif RIO_IS_WIN
    static constexpr ControllerDefine::ControllerId cMainControllerId   = ControllerDefine::CONTROLLER_WIN;
    static constexpr ControllerDefine::ControllerId cMainGamepadId      = ControllerDefine::CONTROLLER_WIN;
    static constexpr ControllerDefine::ControllerId cMainPointerId      = ControllerDefine::CONTROLLER_WIN;
    static constexpr ControllerDefine::ControllerId cGamepadId          = ControllerDefine::CONTROLLER_WIN_GAMEPAD;
#else
    static constexpr ControllerDefine::ControllerId cMainControllerId   = ControllerDefine::CONTROLLER_NULL;
    static constexpr ControllerDefine::ControllerId cMainGamepadId      = ControllerDefine::CONTROLLER_NULL;
    static constexpr ControllerDefine::ControllerId cMainPointerId      = ControllerDefine::CONTROLLER_NULL;
    static constexpr ControllerDefine::ControllerId cGamepadId          = ControllerDefine::CONTROLLER_NULL;
#endif

    // Maximum number of gamepads (Common minimum of all platforms)
    static constexpr u32 cNumGamepadMaxCommon = 4;

public:
    static bool createSingleton();
    static void destroySingleton();
    static ControllerMgr* instance() { return sInstance; }

private:
    static ControllerMgr* sInstance;

    ControllerMgr();
    virtual ~ControllerMgr();

    ControllerMgr(const ControllerMgr&);
    ControllerMgr& operator=(const ControllerMgr&);

    friend class TaskMgr;

private:
    virtual void calc_();

    void initializeDefault_();
    void finalizeDefault_();

public:
    // TODO: Add/remove devices & controllers

    Controller* getControllerByOrder(ControllerDefine::ControllerId id, s32 index) const;
    ControlDevice* getControlDevice(ControllerDefine::DeviceId id) const;

    s32 findControllerPort(const Controller* controller) const;

    // Main controller for the platform
    Controller* getMainController() const { return getControllerByOrder(cMainControllerId, 0); }
    // Main gamepad for the platform
    Controller* getMainGamepad()    const { return getControllerByOrder(cMainGamepadId,    0); }
    // Main controller with pointer for the platform
    Controller* getMainPointer()    const { return getControllerByOrder(cMainPointerId,    0); }

    Controller* getGamepad(u32 index) const { return getControllerByOrder(cGamepadId, index); }

private:
    std::vector<ControlDevice*> mDevices;
    std::vector<Controller*>    mControllers;
};

}

#endif // RIO_CONTROLLER_MGR_H
