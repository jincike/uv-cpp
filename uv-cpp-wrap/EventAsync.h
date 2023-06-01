#ifndef UV_ASYNC_HPP
#define UV_ASYNC_HPP
#include <uv.h>
#include <memory>
#include <functional>
#include <queue>
#include <mutex>

namespace SM
{
using DefaultCallback = std::function<void()>;
class Async  : public std::enable_shared_from_this<Async>
{
public:
    using OnCloseCompletedCallback = std::function<void()>;
    Async();
    void  init(uv_loop_t *loop_);
    virtual ~Async();

    void runInThisLoop(DefaultCallback&& callback);

    void close(OnCloseCompletedCallback&& callback);
    //EventLoop* Loop();
private:
    uv_loop_t *loop_;
    std::mutex mutex_;
    uv_async_t* handle_;
    std::queue<DefaultCallback> callbacks_;
    OnCloseCompletedCallback onCloseCompletCallback_;
    void process();
    static void Callback(uv_async_t* handle);
    void onCloseCompleted();

};

}
#endif
