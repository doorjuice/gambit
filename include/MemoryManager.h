#ifndef MemoryManager_H
#define MemoryManager_H

//#include "obj.h"


/*---------------------------------------------------------------------------*/

extern void ___fatal_error(char** msgs);

class MemoryBroker;

class MemoryManager : public ___pstate_mem {
    friend struct ___virtual_machine_state_struct;
    
    private:
    MemoryBroker* broker;
    
    public:
    static void reportFatalOverflow(char* msg);
    
    inline void updateHeapPtr(___WORD* allocPtr) {
        alloc_heap_ptr_ = allocPtr;
    }
    
    ___WORD* requireHeapSpace(const ___SIZE_TS size);
    ___WORD* nextHeapSection();
    ___WORD* nextStackSection();
};

#endif
