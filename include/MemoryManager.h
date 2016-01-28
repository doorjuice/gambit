#ifndef MemoryManager_H
#define MemoryManager_H

#include "mem_struct.h"


/*---------------------------------------------------------------------------*/

extern void ___fatal_error(char** msgs);

class MemoryBroker;

class MemoryManager : public ___pstate_mem {
    friend struct ___virtual_machine_state_struct;
    
    private:
    MemoryBroker* broker;
    
    public:
    void next_heap_msection();
    
    private:
    void fatal_heap_overflow();
};

#endif
