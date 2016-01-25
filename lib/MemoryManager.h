#ifndef MemoryManager_H
#define MemoryManager_H

#include "gambit.h"
#include "mem.h"


/*---------------------------------------------------------------------------*/

extern void ___fatal_error(char** msgs);

class MemoryManager {
    
    public:
    void next_heap_msection(___processor_state ___ps);
    
    private:
    ___msection *next_msection(___processor_state ___ps, ___msection *ms);
    ___WORD *start_of_tospace(___virtual_machine_state ___vms, ___msection *s);
    void fatal_heap_overflow();
};

#endif
