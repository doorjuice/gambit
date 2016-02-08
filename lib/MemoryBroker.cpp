#include "gambit.h"

#include "MemoryAllocatedObject.h" //FIXME temp include


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

//TODO still_objs_to_scan could also be unique per thread
void MemoryBroker::markStillObjectForScan(___WORD* stillObject) {
    //___MUTEX_LOCK(still_objs_lock_);
    if (___COMPARE_AND_SWAP_WORD(stillObject + ___STILL_MARK_OFS, -1, 
                                 ___CAST(___WORD, still_objs_to_scan_)) 
        == -1)
        still_objs_to_scan_ = ___CAST(___WORD, stillObject);
    //___MUTEX_UNLOCK(still_objs_lock_);
}

___WORD* MemoryBroker::getStartOfTospace(___msection* ms) const {
    return ms->base + (tospace_at_top_ ? MSECTION_HALF : 0);
}

___WORD* MemoryBroker::getStartOfFromspace(___msection* ms) const {
    return ms->base + (tospace_at_top_ ? 0 : MSECTION_HALF);
}
