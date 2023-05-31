#ifndef SM_EVENT_LOOP_H
#define SM_EVENT_LOOP_H

#include <uv.h>
#include <thread>
#include <atomic>
#include <functional>
#include <memory>

#include "EventAsync.h"

namespace SM
{
    using DefaultCallback = std::function<void()>;
    //using EventLoopPtr = std::shared_ptr<EventLoopUV>;

    enum Mode
    {
        Default,
        New
    };
    enum Status
    {
        NotStarted,
        Started,
        Stopped
    };

    class Async;
    class EventLoopUV
    {
    public:
        EventLoopUV();
        ~EventLoopUV();

        static EventLoopUV& DefaultLoop();

        int run();
        int runNoWait();
        int stop();
        bool isStopped();
        Status getStatus();
        bool isRunInLoopThread();
        void runInThisLoop(const DefaultCallback& func);
        void postAsyncTask(DefaultCallback&& func);
        uv_loop_t *handle();

        static const char *GetErrorMessage(int status);

    private:
        EventLoopUV(Mode mode);

        std::thread::id loopThreadId_;
        uv_loop_t *loop_;
        Async *async_;
        std::atomic<Status> status_;
    };
};
#endif