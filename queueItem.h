#pragma once
#include <functional>
#include <cstdint>
#include <window.h>
#include <memory>

class QueueItem 
{
  public:
    QueueItem(std::function<void()> call, double nextExecutionTime, double delay = 0.0, uint32_t maxExecutions = 1):
      call(call), delay(delay), nextExecutionTime(nextExecutionTime), maxExecutions(maxExecutions), startTime(WINDOW.getTime()) {};

    void invoke();
    bool done() { return this->totalExecutions >= this->maxExecutions; };
    double getNextExecutionTime() { return this->nextExecutionTime; };
  private:
    std::function<void()> call;
    uint32_t totalExecutions = 0;
    uint32_t maxExecutions = 1;

    double startTime = 0.0, delay = 0.0, nextExecutionTime = 0.0;
};