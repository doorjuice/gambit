#include "MemoryManager.h"

#include "MemoryBroker.h"

#ifndef ___MSECTION_SIZE
#define ___MSECTION_SIZE 131072
#endif //FIXME replace with a static class constant


void MemoryManager::next_heap_msection()
{
  if (heap_msection_ != 0)
    {
      broker->words_prev_msections_ += alloc_heap_ptr_ - alloc_heap_start_;
      heap_msection_->alloc = alloc_heap_ptr_;
    }
    
  ___msection *next_section = broker->next_msection();
  if (next_section == NULL) {
    if (stack_msection_ == heap_msection_)
      fatal_heap_overflow();
    next_section = stack_msection_;
  }
  heap_msection_ = next_section;
  
  alloc_heap_start_ = broker->start_of_tospace(heap_msection_);
  alloc_heap_limit_ = alloc_heap_start_ + (___MSECTION_SIZE>>1);
  alloc_heap_ptr_ = alloc_heap_start_;
}

void MemoryManager::fatal_heap_overflow()
{
  char* msgs[] = {"Heap overflow", NULL};
  ___fatal_error(msgs);
}
