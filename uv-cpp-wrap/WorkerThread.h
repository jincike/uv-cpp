#include "EventLoop.h"
#include <thread>
#include <condition_variable>
#include <mutex>
//thread_local std::unique_ptr<SM::EventLoopUV> mEventLoop;
class WorkerThread
{
public:
    WorkerThread();
    ~WorkerThread();

    SM::EventLoopUV* GetThreadLoop();

private:
    std::thread mThread;
    void InitEventLoop();
    SM::EventLoopUV* mEventLoop;
    std::condition_variable mInitCond;
    std::mutex mInitMutex;
    bool mReady = false;
};