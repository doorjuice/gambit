#include "gambit.h" //TODO replace me with "MemoryBroker.h"
#include "mem.h"


const ___WORD MemoryBroker::MSECTION_SIZE = ___MSECTION_SIZE;
const ___WORD MemoryBroker::MSECTION_HALF = MSECTION_SIZE >> 1;


MemoryBroker::MemoryBroker() {

    /*
    * Set the overflow reserve so that the rest parameter handler can
    * construct the rest parameter list without having to call the
    * garbage collector.
    */
    
    normal_overflow_reserve_ = 2*((___MAX_NB_PARMS+___SUBTYPED_OVERHEAD) +
                                ___MAX_NB_ARGS*(___PAIR_SIZE+___PAIR_OVERHEAD));
    overflow_reserve_ = normal_overflow_reserve_;
    
    /* Setup GC statistics */
    
    nb_gcs_ = 0.0;
    gc_user_time_ = 0.0;
    gc_sys_time_ = 0.0;
    gc_real_time_ = 0.0;
    bytes_allocated_minus_occupied_ = 0.0;
    
    last_gc_real_time_ = 0.0;
    last_gc_heap_size_ = ___CAST(___F64,heap_size_) * ___WS;
    last_gc_live_ = 0.0;
    last_gc_movable_ = 0.0;
    last_gc_nonmovable_ = 0.0;
    
    /* Initialize GC state */
    
    words_nonmovable_ = 0;
    words_prev_msections_ = 0;
    
    tospace_at_top_ = 0;
    tospace_offset_ = 0;
    nb_msections_used_ = 0;
    
    executable_wills_ = ___TAG(0,___EXECUTABLE_WILL); /* tagged empty list */
    
    // Warning: the MemoryBroker instance will _not_ be ready for use after being 
    // constructed, since the actual initial allocation of heap space is left for
    // later, specifically once the global setup parameter 'min_heap' is known.
}

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
