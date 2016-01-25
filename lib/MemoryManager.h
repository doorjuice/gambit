#ifndef MemoryManager_H
#define MemoryManager_H

#include "gambit.h"
#include "mem.h"


/*---------------------------------------------------------------------------*/

extern void ___fatal_error(char** msgs);

class MemoryManager : private ___msections {
    
    private:
    const ___processor_state ___ps;
    
    public:
    MemoryManager(___processor_state ps);
    void next_heap_msection();
    
    private:
    ___msection *next_msection(___msection *ms);
    ___WORD *start_of_tospace(___msection *s);
    void fatal_heap_overflow();
};

#endif
