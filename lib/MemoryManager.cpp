#include "gambit.h" //TODO replace me with the following:

//#include "MemoryManager.h"

//#include "MemoryBroker.h"

#include <assert.h>


void MemoryManager::reportFatalOverflow(char* msg) {
    char* msgs[] = {msg, NULL};
    ___fatal_error(msgs);
}

void MemoryManager::init() {
    assert(broker != NULL); // You can think of 'init' as a delayed constructor
    
   /*
    * Allocate processor's stack and heap.
    */
    
    stack_msection_ = NULL;
    heap_msection_ = NULL;
    
    nextStackSection(); /* allocate one msection for stack */
    nextHeapSection();  /* allocate one msection for local heap */
    
   /*
    * Create "break frame" of initial top section.
    */
    
    alloc_stack_ptr_ = alloc_stack_start_;
    
    ___FP_ADJFP(alloc_stack_start_, ___BREAK_FRAME_SPACE)
    ___FP_SET_STK(alloc_stack_start_, -___BREAK_FRAME_NEXT, ___END_OF_CONT_MARKER)
    
   /*
    * Setup nonexecutable will list.
    */
    
    nonexecutable_wills_ = ___TAG(0,0); /* tagged empty list */
}

___WORD* MemoryManager::requireHeapSpace(const ___SIZE_TS size) {
    while (alloc_heap_ptr_ + size > alloc_heap_limit_)
        nextHeapSection();
    return alloc_heap_ptr_;
}

___WORD* MemoryManager::nextHeapSection() {
    if (heap_msection_ != NULL) {
        broker->words_prev_msections_ += alloc_heap_ptr_ - alloc_heap_start_;
        heap_msection_->alloc = alloc_heap_ptr_;
    }
    
    ___msection* nextSection = broker->nextMemorySection();
    if (nextSection == NULL) {
        if (heap_msection_ == stack_msection_)
            reportFatalOverflow("Heap overflow");
        nextSection = stack_msection_;
    }
    heap_msection_ = nextSection;
  
    alloc_heap_start_ = heap_msection_->base + broker->getTospaceOffset();
    alloc_heap_limit_ = alloc_heap_start_ + MemoryBroker::MSECTION_HALF;
    return alloc_heap_ptr_ = alloc_heap_start_;
}

___WORD* MemoryManager::nextStackSection() {
    if (stack_msection_ != NULL) {
        broker->words_prev_msections_ += alloc_stack_start_ - alloc_stack_ptr_;
        stack_msection_->alloc = alloc_stack_ptr_;
    }
    
    ___msection* nextSection = broker->nextMemorySection();
    if (nextSection == NULL) {
        if (stack_msection_ == heap_msection_)
            reportFatalOverflow("Stack overflow");
        nextSection = heap_msection_;
    }
    stack_msection_ = nextSection;

    alloc_stack_limit_ = stack_msection_->base + broker->getTospaceOffset();
    alloc_stack_start_ = alloc_stack_limit_ + MemoryBroker::MSECTION_HALF;
    return alloc_stack_ptr_ = alloc_stack_start_;
}
