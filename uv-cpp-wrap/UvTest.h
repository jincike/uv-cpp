#include "EventLoop.h"

class UvTest
{
public:
    UvTest(SM::EventLoopUV *eventLoop);
    ~UvTest();
    void doWake();

private:
    SM::EventLoopUV *mEventLoop;
};

class Manager
{
public:
    static Manager& getInstance()
    {
        static Manager instance;
        return instance;
    }
};