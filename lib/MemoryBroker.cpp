#include "gambit.h" //TODO replace me with "MemoryBroker.h"
#include "mem.h"


/* words occupied in heap by movable objects including current msections */
#define wORDS_MOVABLE \
(2*(words_prev_msections_ + \
(alloc_stack_start_ - alloc_stack_ptr_) + \
(alloc_heap_ptr_ - alloc_heap_start_)))

/* words occupied in heap including current msections */
#define wORDS_OCCUPIED (words_nonmovable_ + wORDS_MOVABLE)

/* words usable in msections */
#define wORDS_MOVABLE_USABLE \
(2*the_msections_->nb_sections*___CAST(___SIZE_TS,((___MSECTION_SIZE>>1)-___MSECTION_FUDGE+1)))

/* words available in heap */
#define wORDS_AVAILABLE \
(words_nonmovable_ + wORDS_MOVABLE_USABLE - \
overflow_reserve_ - 2*___MSECTION_FUDGE)


const ___WORD MemoryBroker::MSECTION_SIZE = ___MSECTION_SIZE;
const ___WORD MemoryBroker::MSECTION_HALF = MSECTION_SIZE >> 1;


extern void adjust_msections(___msections **msp, int n);

MemoryBroker::MemoryBroker(int nbProcessors) {
  /*
   * It is important to initialize the following pointers first so
   * that if the program terminates early the procedure
   * ___cleanup_mem_vmstate will not access dangling pointers.
   */

  the_msections_ = 0;
  still_objs_    = 0;

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

  /* Allocate Gambit VM heap */
    int init_nb_sections;
  init_nb_sections = ((___GSTATE->setup_params.min_heap >> ___LWS) +
                      overflow_reserve_ + 2*___MSECTION_FUDGE +
                      2*((___MSECTION_SIZE>>1)-___MSECTION_FUDGE+1) - 1) /
                     (2*((___MSECTION_SIZE>>1)-___MSECTION_FUDGE+1));

  if (init_nb_sections < ___MIN_NB_MSECTIONS)
    init_nb_sections = ___MIN_NB_MSECTIONS;
    
      /*
   * Make sure there are at least 2 msections for each processor
   * in this VM.
   */

  if (init_nb_sections < 2 * nbProcessors)
    init_nb_sections = 2 * nbProcessors;

  adjust_msections (&the_msections_, init_nb_sections);

  if (the_msections_ == 0 ||
      the_msections_->nb_sections != init_nb_sections)
    //return ___FIX(___HEAP_OVERFLOW_ERR);
    ___FIX(___HEAP_OVERFLOW_ERR);

#ifdef ENABLE_CONSISTENCY_CHECKS
  if (___DEBUG_SETTINGS_LEVEL(___GSTATE->setup_params.debug_settings) >= 1)
    {
      zap_fromspace (___vms);
      stack_fudge_used = 0;
      heap_fudge_used = 0;
    }
#endif

  words_nonmovable_ = 0;
  words_prev_msections_ = 0;

  tospace_at_top_ = 0;
  tospaceOffset = 0;
  nb_msections_used_ = 0;

  executable_wills_ = ___TAG(0,___EXECUTABLE_WILL); /* tagged empty list */

  heap_size_ = wORDS_AVAILABLE;
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
