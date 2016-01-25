#include "MemoryManager.h"

void MemoryManager::next_heap_msection(___processor_state ___ps)
{
  if (heap_msection != 0)
    {
      words_prev_msections += alloc_heap_ptr - alloc_heap_start;
      heap_msection->alloc = alloc_heap_ptr;
    }

  heap_msection = next_msection (___ps, stack_msection);
  alloc_heap_start = start_of_tospace (___VMSTATE_FROM_PSTATE(___ps), heap_msection);
  alloc_heap_limit = alloc_heap_start + (___MSECTION_SIZE>>1);
  alloc_heap_ptr = alloc_heap_start;
}

___msection* MemoryManager::next_msection(___processor_state ___ps, ___msection *ms)
{
  ___msection *result;

  if (nb_msections_used == 0)
    result = the_msections->head;
  else
    result = alloc_msection->next;

  if (result == 0)
    {
      if (stack_msection == heap_msection)
        fatal_heap_overflow ();
      result = ms;
    }
  else
    {
      alloc_msection = result;
      nb_msections_used++;
    }

  return result;
}

___WORD* MemoryManager::start_of_tospace(___virtual_machine_state ___vms, ___msection *s)
{
#undef ___VMSTATE_MEM
#define ___VMSTATE_MEM(var) ___vms->mem.var

  if (tospace_at_top)
    return s->base + (___MSECTION_SIZE>>1);
  else
    return s->base;

#undef ___VMSTATE_MEM
#define ___VMSTATE_MEM(var) ___VMSTATE_FROM_PSTATE(___ps)->mem.var
}

void MemoryManager::fatal_heap_overflow()
{
  char *msgs[2];
  msgs[0] = "Heap overflow";
  msgs[1] = 0;
  ___fatal_error (msgs);
}
