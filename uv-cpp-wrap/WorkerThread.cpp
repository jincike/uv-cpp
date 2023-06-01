#include "WorkerThread.h"
#include "EventLoop.h"
#include <iostream>

WorkerThread::WorkerThread()
{
    std::unique_lock<std::mutex> lg(mInitMutex);
    mThread = std::thread(&WorkerThread::InitEventLoop, this);
    mInitCond.wait(lg, [this]()
                   { return this->mReady; });
}

WorkerThread::~WorkerThread()
{
    mThread.join();
}

void WorkerThread::InitEventLoop()
{
    auto eventLoop = std::make_unique<SM::EventLoopUV>();
    mEventLoop = eventLoop.get();
    {
        std::lock_guard<std::mutex> lg(mInitMutex);
        this->mReady = true;
        mInitCond.notify_all();
    }
    eventLoop.get()->run();
}

SM::EventLoopUV *WorkerThread::GetThreadLoop()
{
    return mEventLoop;
}