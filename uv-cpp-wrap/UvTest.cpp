#include "UvTest.h"

#include <iostream>

UvTest::UvTest(SM::EventLoopUV *eventLoop)
    : mEventLoop(eventLoop)
{
    mWorkerThread = std::make_unique<WorkerThread>();
    mWorkerLoop = mWorkerThread->GetThreadLoop();
}

void UvTest::doWake()
{
    mEventLoop->postTask([this]()
                              { std::cout << "I am also run in main loop! t_id:" << std::this_thread::get_id() << std::endl; });
    mWorkerLoop->postTask([this]()
                               { std::cout << "I am in worker loop! t_id:" << std::this_thread::get_id() << std::endl; });
}

UvTest::~UvTest()
{
}
