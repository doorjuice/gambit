#ifndef MemoryManager_H
#define MemoryManager_H

//#include "obj.h"


/*---------------------------------------------------------------------------*/

extern void ___fatal_error(char** msgs);

class MemoryBroker;

class MemoryManager : public ___pstate_mem {
    
    private:
    MemoryBroker* broker;
    friend struct ___virtual_machine_state_struct;
    
    void init();
    friend ___SCMOBJ ___setup_mem_pstate(___processor_state ___ps, 
                                         ___virtual_machine_state ___vms);
    
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
