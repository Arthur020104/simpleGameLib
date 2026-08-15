#include <queueItem.h>

void QueueItem::invoke()
{
  if(this->totalExecutions >= this->maxExecutions || this->nextExecutionTime > WINDOW.getTime()) return;
  
  this->call();
  this->totalExecutions++;
  this->nextExecutionTime = WINDOW.getTime() + this->delay;
}