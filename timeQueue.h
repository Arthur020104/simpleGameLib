#pragma once
#include <vector>
#include <queueItem.h>
#include <thread>
#include <mutex>
#include <memory>

class TimeQueue
{
  public:
    TimeQueue();
    ~TimeQueue();

    void addToQueue(std::function<void()> call, double startDelay);
    void startRoutine(std::function<void()> call, double delay, double startDelay = 0.0, u_int32_t maxExecutions = UINT32_MAX);
    void startRoutine(std::function<void()> call, double delay, double startDelay, double timeExecuting);
  private:
    std::vector<QueueItem> queue;
    std::thread thread;

    mutable std::mutex mtx;
    std::vector<QueueItem> waitingToBeAdded;

    bool isRunning = true;
    
    void update();
};