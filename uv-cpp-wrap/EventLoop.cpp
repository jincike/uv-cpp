#include "EventLoop.h"
#include <iostream>
#include <assert.h>
namespace SM
{
    EventLoopUV::EventLoopUV()
        : EventLoopUV(SM::Mode::New)
    {
    }

    EventLoopUV::EventLoopUV(SM::Mode mode)
        : loop_(nullptr),
          async_(nullptr)
    {
        if (mode == SM::Mode::New)
        {
            loop_ = new uv_loop_t();
            int initRet = uv_loop_init(loop_);
            assert(initRet == 0);
        }
        async_ = new Async();
        async_->init(loop_);
    }

    EventLoopUV::~EventLoopUV()
    {
        this->stop();
        if (loop_ != uv_default_loop())
        {
            uv_loop_close(loop_);
            delete async_;
            delete loop_;
        }
    }

    void EventLoopUV::run()
    {
        uv_run(loop_, UV_RUN_DEFAULT);
    }

    void EventLoopUV::runNoWait()
    {
        uv_run(loop_, UV_RUN_NOWAIT);
    }

    int EventLoopUV::stop()
    {
        async_->close([this]()
                      { uv_stop(this->loop_); });
        return 0;
    }

    void EventLoopUV::postTaskAndWait(DefaultCallback &&func)
    {
        if (nullptr == func)
            return;

        bool ready = false;
        std::mutex mtx;
        std::unique_lock<std::mutex> lg(mtx);
        std::condition_variable mInitCond;
        this->postTask([&mInitCond, &ready, task = std::move(func)]()
                       {
             task();
             ready = true;
             mInitCond.notify_all(); });

        mInitCond.wait(lg, [&ready]()
                       { return ready; });
    }

    void EventLoopUV::postTask(DefaultCallback &&func)
    {
        if (nullptr == func)
        {
            return;
        }
        async_->runInThisLoop(std::move(func));
    }
};