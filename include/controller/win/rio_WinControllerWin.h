#ifndef RIO_WIN_CONTROLLER_WIN_H
#define RIO_WIN_CONTROLLER_WIN_H

#include <controller/rio_Controller.h>

namespace rio {

class WinController : public Controller
{
public:
    static const u8 sDefaultPadConfig[PAD_IDX_MAX];

public:
    WinController(ControllerMgr* mgr);
    virtual ~WinController() { }

    void setPadConfig(const u8* pad_config);

private:
    virtual void calcImpl_();

    u8 mPadConfig[PAD_IDX_MAX];
};

}

#endif // RIO_WIN_CONTROLLER_WIN_H
