#include <timeQueue.h>
#include <stdexcept>

TimeQueue::TimeQueue()
{
  this->thread = std::thread(&TimeQueue::update, this);
}

TimeQueue::~TimeQueue()
{
  this->isRunning = false;
  
  if(this->thread.joinable())
    this->thread.join();
}

void TimeQueue::update()
{
  while(true)
  {
    double sleepDurationInSeconds = 0.01;
    if(!this->isRunning) break;

    std::this_thread::sleep_for(std::chrono::duration<double>(sleepDurationInSeconds));

    double currentTime = WINDOW.getTime();

    for(uint32_t i = 0; i < this->queue.size(); i++)
    {
      if(this->queue[i].done())
      {
        this->queue.erase(this->queue.begin() + i);
        i--;
        continue;
      }

      this->queue[i].invoke();
      double timeUntilNextExecution = std::max(this->queue[i].getNextExecutionTime() - currentTime, 0.001);
      
      sleepDurationInSeconds = std::min(sleepDurationInSeconds, timeUntilNextExecution);
    }

    std::unique_lock<std::mutex> lock(mtx, std::try_to_lock);

    if (!lock.owns_lock()) continue;

    this->queue.insert(this->queue.end(), this->waitingToBeAdded.begin(), this->waitingToBeAdded.end());
    this->waitingToBeAdded.clear();
  }
}

void TimeQueue::addToQueue(std::function<void()> call, double startDelay)
{
  std::unique_lock<std::mutex> lock(mtx);
  
  QueueItem item(call, WINDOW.getTime() + startDelay);
  this->waitingToBeAdded.push_back(item);
}

void TimeQueue::startRoutine(std::function<void()> call, double delay, double startDelay, uint32_t maxExecutions)
{
  std::unique_lock<std::mutex> lock(mtx);
  
  QueueItem item(call, WINDOW.getTime() + startDelay, delay, maxExecutions);
  this->waitingToBeAdded.push_back(item);
}
void TimeQueue::startRoutine(std::function<void()> call, double delay, double startDelay, double timeExecuting)
{
  if(startDelay + delay > timeExecuting)
    throw std::runtime_error("The time executing must be greater than the sum of start delay and delay, otherwise the routine will execute once or not at all. If you want to make it execute only once, use addToQueue() instead.");

  std::unique_lock<std::mutex> lock(mtx);

  uint32_t maxExecutions = (timeExecuting - startDelay) / delay;

  QueueItem item(call, WINDOW.getTime() + startDelay, delay, maxExecutions);
  this->waitingToBeAdded.push_back(item);
}