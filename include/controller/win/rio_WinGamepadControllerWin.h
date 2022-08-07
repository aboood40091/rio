#ifndef RIO_WIN_GAMEPAD_CONTROLLER_WIN_H
#define RIO_WIN_GAMEPAD_CONTROLLER_WIN_H

#include <controller/rio_Controller.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace rio {

class WinGamepadController : public Controller
{
public:
    WinGamepadController(ControllerMgr* mgr, s32 channel);
    virtual ~WinGamepadController() { }

    virtual bool isConnected() const { return mIsConnected; }

    s32 getChannel() const { return mChannel; }
    const char* getGamepadName() const { return mGamepadName; }

private:
    virtual void calcImpl_();

    bool                mIsConnected;
    s32                 mChannel;
    const char*         mGamepadName;
    GLFWgamepadstate    mState;
};

}

#endif // RIO_WIN_GAMEPAD_CONTROLLER_WIN_H
