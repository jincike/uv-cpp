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
          async_(nullptr),
          status_(NotStarted)
    {
        if (mode == SM::Mode::New)
        {
            loop_ = new uv_loop_t();
            int initRet = uv_loop_init(loop_);
            assert(initRet == 0);
           
        }
        else
        {
            loop_ = uv_default_loop();
        }
        async_ = new Async();
         async_->init(loop_);
    }

    EventLoopUV::~EventLoopUV()
    {
        if (loop_ != uv_default_loop())
        {
            uv_loop_close(loop_);
            delete async_;
            delete loop_;
        }
    }

    EventLoopUV &EventLoopUV::DefaultLoop()
    {
        static EventLoopUV defaultLoop(SM::Mode::Default);
        return defaultLoop;
    }

    uv_loop_t *EventLoopUV::handle()
    {
        return loop_;
    }

    int EventLoopUV::run()
    {
        if (status_ == Status::NotStarted)
        {
            
            loopThreadId_ = std::this_thread::get_id();
            status_ = Status::Started;
            auto rst = ::uv_run(loop_, UV_RUN_DEFAULT);
            status_ = Status::Stopped;
            return 0;
        }
        return -1;
    }

    int EventLoopUV::runNoWait()
    {
        if (status_ == Status::NotStarted)
        {
            async_->init(loop_);
            loopThreadId_ = std::this_thread::get_id();
            status_ = Status::Started;
            auto rst = ::uv_run(loop_, UV_RUN_NOWAIT);
            status_ = Status::NotStarted;
            return rst;
        }
        return -1;
    }

    int EventLoopUV::stop()
    {
        if (status_ == Status::Started)
        {
            async_->close([this]()
                          { uv_stop(this->loop_); });
             return 0;
        }
        return -1;
    }

    bool EventLoopUV::isStopped()
    {
        return status_ == Status::Stopped;
    }

    SM::Status EventLoopUV::getStatus()
    {
        return status_;
    }

    bool EventLoopUV::isRunInLoopThread()
    {
        if (status_ == Status::Started)
        {
            return std::this_thread::get_id() == loopThreadId_;
        }
        // EventLoop未运行.
        return false;
    }

    void EventLoopUV::runInThisLoop(const DefaultCallback &func)
    {
        if (nullptr == func)
            return;

        if (isRunInLoopThread() || isStopped())
        {
            func();
            return;
        }
    }

    void EventLoopUV::postAsyncTask(DefaultCallback&&func)
    {
        if (nullptr == func)
        {
            return;
        }
        async_->runInThisLoop(std::move(func));
    }
    // const char *EventLoopUV::GetErrorMessage(int status)
    // {
    //     if (WriteInfo::Disconnected == status)
    //     {
    //         static char info[] = "the connection is disconnected";
    //         return info;
    //     }
    //     return uv_strerror(status);
    // }
};