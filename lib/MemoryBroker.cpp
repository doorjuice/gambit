#include "MemoryBroker.h"


___msection* MemoryBroker::next_msection() {
  ___msection *result;

  if (nb_msections_used_ == 0)
    result = the_msections_->head;
  else
    result = alloc_msection_->next;

  if (result != NULL) {
    alloc_msection_ = result;
    nb_msections_used_++;
  }

  return result;
}

___WORD* MemoryBroker::start_of_tospace(___msection *s) {
  if (tospace_at_top_)
    return s->base + (___MSECTION_SIZE>>1);
  else
    return s->base;
}

