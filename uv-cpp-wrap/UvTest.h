#include "EventLoop.h"
#include "WorkerThread.h"
class UvTest
{
public:
    UvTest(SM::EventLoopUV *eventLoop);
    ~UvTest();
    void doWake();

private:
    std::unique_ptr<WorkerThread> mWorkerThread;
    SM::EventLoopUV *mEventLoop;
    SM::EventLoopUV *mWorkerLoop;
};
