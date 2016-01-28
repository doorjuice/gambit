#ifndef MemoryBroker_H
#define MemoryBroker_H

#include "gambit.h"


/*---------------------------------------------------------------------------*/

class MemoryBroker : public ___vmstate_mem {
    
    public:
    ___msection* next_msection();
    ___WORD *start_of_tospace(___msection *s);
};

#endif
