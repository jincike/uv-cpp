#include "EventAsync.h"
#include <iostream>
namespace SM
{
    Async::Async()
        : handle_(nullptr),
          onCloseCompletCallback_(nullptr)
    {
    }

    void Async::init(uv_loop_t *loop)
    {
        loop_ = loop;
        if (nullptr == handle_)
        {
            handle_ = new uv_async_t();
            handle_->data = this;
            uv_async_init(loop_, handle_, Async::Callback);
           // uv_async_send(handle_);
        }
    }

    Async::~Async()
    {
        //close();
    }

    void Async::runInThisLoop(DefaultCallback&& callback)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            callbacks_.push(std::move(callback));
        }
        if (handle_ != nullptr){
            uv_async_send(handle_);
        }
    }

    void Async::process()
    {
        std::queue<DefaultCallback> callbacks;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            callbacks_.swap(callbacks);
        }
        while (!callbacks.empty())
        {
            auto func = callbacks.front();
            func();
            callbacks.pop();
        }
    }

    void Async::close(Async::OnCloseCompletedCallback&& callback)
    {
        onCloseCompletCallback_ = std::move(callback);
        if (::uv_is_closing((uv_handle_t *)handle_) == 0)
        {
            ::uv_close((uv_handle_t *)handle_, [](uv_handle_t *handle)
                       {
            auto ptr = static_cast<Async*>(handle->data);
            ptr->onCloseCompleted();
            delete (uv_async_t*)handle; });
        }
    }

    // EventLoop Async::Loop()
    // {
    //     return loop_;
    // }

    void Async::Callback(uv_async_t *handle)
    {
        auto async = static_cast<Async*>(handle->data);
        async->process();
    }

    void Async::onCloseCompleted()
    {
        if (nullptr != onCloseCompletCallback_)
        {
            onCloseCompletCallback_();
        }
    }
};