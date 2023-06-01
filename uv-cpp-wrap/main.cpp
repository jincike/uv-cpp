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
    eventLoop->postTask([&uvTest]()
                             { std::cout << "I am run in main loop" << std::endl;
                             uvTest->doWake(); });

    eventLoop->run();
    return 0;
}