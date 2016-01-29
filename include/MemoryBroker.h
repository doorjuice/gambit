#ifndef MemoryBroker_H
#define MemoryBroker_H

#include "mem_struct.h"


/*---------------------------------------------------------------------------*/

class MemoryBroker : public ___vmstate_mem {
    
    public: //TODO replace me with a proper constructor and private scope
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
    
    //DEPRECATED
    ___WORD* getStartOfTospace(___msection* ms) const;
    ___WORD* getStartOfFromspace(___msection* ms) const;
};

#endif
