#include <controller/rio_ControlDevice.h>
#include <controller/rio_ControllerMgr.h>
#include <filedevice/rio_FileDeviceMgr.h>
#include <task/rio_TaskMgr.h>

#if RIO_IS_CAFE

#include <controller/cafe/rio_CafeRemoteControllerCafe.h>
#include <controller/cafe/rio_CafeWPadDeviceCafe.h>

#include <controller/cafe/rio_CafeDRCControllerCafe.h>
#include <controller/cafe/rio_CafeVPadDeviceCafe.h>

#elif RIO_IS_WIN

#include <controller/win/rio_WinGamepadControllerWin.h>

#include <controller/win/rio_WinControllerWin.h>
#include <controller/win/rio_KeyboardMouseDeviceWin.h>

#endif

#include <algorithm>

namespace rio {

ControllerMgr* ControllerMgr::sInstance = nullptr;

bool ControllerMgr::createSingleton()
{
    if (sInstance)
        return false;

    sInstance = TaskMgr::instance()->createTask<ControllerMgr>();
    return sInstance;
}

void ControllerMgr::destroySingleton()
{
    if (!sInstance)
        return;

    TaskMgr::instance()->destroyTask(sInstance);
    sInstance = nullptr;
}

ControllerMgr::ControllerMgr()
    : ITask("rio::ControllerMgr")
{
    initializeDefault_();
}

ControllerMgr::~ControllerMgr()
{
    finalizeDefault_();
}

void ControllerMgr::initializeDefault_()
{
    mControllers.reserve(
#if RIO_IS_CAFE
        5
#elif RIO_IS_WIN
        1 + (GLFW_JOYSTICK_LAST + 1)
#else
        0
#endif
    );

#if RIO_IS_CAFE
    mDevices.push_back(new CafeWPadDevice(this));
    for (u32 i = 0; i < 4; i++)
    {
        mControllers.push_back(new CafeRemoteController(this, i));
    }

    mDevices.push_back(new CafeVPadDevice(this));
    {
        mControllers.push_back(new CafeDRCController(this));
    }
#elif RIO_IS_WIN
    {
        FileDevice::LoadArg arg;
        arg.path = "gamecontrollerdb.txt";

        char* file = (char*)FileDeviceMgr::instance()->load(arg);
        glfwUpdateGamepadMappings(file);
        MemUtil::free(file); file = nullptr;
    }

    for (u32 i = 0; i <= GLFW_JOYSTICK_LAST; i++)
    {
        mControllers.push_back(new WinGamepadController(this, i));
    }

    mDevices.push_back(new KeyboardMouseDevice(this));
    {
        mControllers.push_back(new WinController(this));
    }
#endif
}

void ControllerMgr::finalizeDefault_()
{
    // TODO: FIXME (Only clear devices/controllers registered by initializeDefault_())

    for (ControlDevice* device : mDevices)
        delete device;

    mDevices.clear();

    for (Controller* controller : mControllers)
        delete controller;

    mControllers.clear();
}

void ControllerMgr::calc_()
{
    for (ControlDevice* device : mDevices)
        device->calc();

    for (Controller* controller : mControllers)
        controller->calc();
}

Controller* ControllerMgr::getControllerByOrder(ControllerDefine::ControllerId id, s32 index) const
{
    for (Controller* controller : mControllers)
    {
        if (controller->mId == id)
        {
            if (index == 0)
                return controller;

            index--;
        }
    }

    return nullptr;
}

ControlDevice* ControllerMgr::getControlDevice(ControllerDefine::DeviceId id) const
{
    for (ControlDevice* device : mDevices)
    {
        if (device->mId == id)
            return device;
    }

    return nullptr;
}

s32 ControllerMgr::findControllerPort(const Controller* controller) const
{
    RIO_ASSERT(controller);

    size_t idx = std::distance(mControllers.cbegin(), std::find(mControllers.cbegin(), mControllers.cend(), controller));
    if (idx < mControllers.size())
        return idx;

    return -1;
}

}
