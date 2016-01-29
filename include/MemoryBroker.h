#ifndef MemoryBroker_H
#define MemoryBroker_H

#include "mem_struct.h"


/*---------------------------------------------------------------------------*/

class MemoryBroker : public ___vmstate_mem {
    
    private:
    ___WORD tospaceOffset;
    
    public:
    static const ___WORD MSECTION_SIZE, MSECTION_HALF;
    
    inline ___WORD getTospaceOffset() const {
        return tospaceOffset;
    }
    
    inline void toggleTospace() {
        tospace_at_top_ = !tospace_at_top_;
        tospaceOffset = tospace_at_top_ ? MSECTION_HALF : 0;
    }
    
    ___msection* nextMemorySection();
    ___WORD* start_of_tospace(___msection *ms) const;
};

#endif
