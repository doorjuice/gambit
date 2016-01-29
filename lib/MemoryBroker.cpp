#include "MemoryBroker.h"


const ___WORD MemoryBroker::MSECTION_SIZE = 131072; //TODO sync with defined value
const ___WORD MemoryBroker::MSECTION_HALF = MSECTION_SIZE >> 1;


___msection* MemoryBroker::nextMemorySection() {
    ___msection* result;

    if (nb_msections_used_ > 0)
        result = alloc_msection_->next;
    else
        result = the_msections_->head;

    if (result != NULL) {
        alloc_msection_ = result;
        nb_msections_used_++;
    }

    return result;
}

___WORD* MemoryBroker::start_of_tospace(___msection *ms) const {
    return ms->base + (tospace_at_top_ ? MSECTION_HALF : 0);
}
