#include <iostream>
#include "EventLoop.h"
#include <memory>
#include <thread>
#include <chrono>
#include "UvTest.h"

int main()
{
    std::unique_ptr<SM::EventLoopUV> eventLoop = std::make_unique<SM::EventLoopUV>();
    std::unique_ptr<UvTest> uvTest = std::make_unique<UvTest>(eventLoop.get());
    eventLoop->postAsyncTask([]()
                             { std::cout << "I am run in loop" << std::endl; });
               uvTest->doWake();              
    eventLoop->run();
    std::cout << "I am in main" << std::endl;
    eventLoop->postAsyncTask([&]()
                             { std::this_thread::sleep_for(std::chrono::milliseconds(5000)); });
    return 0;
}