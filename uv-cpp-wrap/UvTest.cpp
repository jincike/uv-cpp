#include "UvTest.h"
#include <iostream>

UvTest::UvTest(SM::EventLoopUV *eventLoop)
    : mEventLoop(eventLoop)
{
    // doWake();
}

void UvTest::doWake()
{
    mEventLoop->postAsyncTask([this]()
                              { std::cout << "I am in inner loop!" << std::endl;
                              this->doWake(); });
}

UvTest::~UvTest()
{
}
