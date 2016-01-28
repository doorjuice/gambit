#ifndef MemoryManager_H
#define MemoryManager_H

#include "gambit.h"
#include "mem.h"


/*---------------------------------------------------------------------------*/

extern void ___fatal_error(char** msgs);

class MemoryBroker;

class MemoryManager : public ___pstate_mem {
    
    private:
    MemoryBroker* const broker;
    
    public:
    MemoryManager(MemoryBroker* memory_broker);
    void next_heap_msection();
    
    private:
    void fatal_heap_overflow();
};

#endif
