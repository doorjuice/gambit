#include "MemoryManager.h"

MemoryManager::MemoryManager(___processor_state ps)
: ___ps(ps) 
{ }

void MemoryManager::next_heap_msection()
{
  if (heap_msection != 0)
    {
      words_prev_msections += alloc_heap_ptr - alloc_heap_start;
      heap_msection->alloc = alloc_heap_ptr;
    }

  heap_msection = next_msection(stack_msection);
  alloc_heap_start = start_of_tospace(heap_msection);
  alloc_heap_limit = alloc_heap_start + (___MSECTION_SIZE>>1);
  alloc_heap_ptr = alloc_heap_start;
}

___msection* MemoryManager::next_msection(___msection *ms)
{
  ___msection *result;

  if (nb_msections_used == 0)
    result = the_msections->head;
  else
    result = alloc_msection->next;

  if (result == 0)
    {
      if (stack_msection == heap_msection)
        fatal_heap_overflow();
      result = ms;
    }
  else
    {
      alloc_msection = result;
      nb_msections_used++;
    }

  return result;
}

___WORD* MemoryManager::start_of_tospace(___msection *s)
{
  if (tospace_at_top)
    return s->base + (___MSECTION_SIZE>>1);
  else
    return s->base;
}

void MemoryManager::fatal_heap_overflow()
{
  char* msgs[] = {"Heap overflow", NULL};
  ___fatal_error(msgs);
}
