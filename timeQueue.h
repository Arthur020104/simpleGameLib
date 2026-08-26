#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <queueItem.h>

class TimeQueue
{
  public:
    TimeQueue();
    ~TimeQueue();

    void addToQueue(std::function<void()> call, double startDelay);
    void startRoutine(std::function<void()> call, double delay, double startDelay = 0.0, uint32_t maxExecutions = UINT32_MAX);
    void startRoutine(std::function<void()> call, double delay, double startDelay, double timeExecuting);
  private:
    std::vector<QueueItem> queue;
    std::thread thread;

    mutable std::mutex mtx;
    std::vector<QueueItem> waitingToBeAdded;

    bool isRunning = true;
    
    void update();
};