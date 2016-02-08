#ifndef MemoryBroker_H
#define MemoryBroker_H

//#include "thread.h"
//#include "mem_struct.h"


/*---------------------------------------------------------------------------*/

class MemoryBroker : public ___vmstate_mem {
    
    public: //TODO replace me with a proper constructor and private scope
    //___MUTEX still_objs_lock_;
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
    void markStillObjectForScan(___WORD* stillObject);
    
    //DEPRECATED
    ___WORD* getStartOfTospace(___msection* ms) const;
    ___WORD* getStartOfFromspace(___msection* ms) const;
};

#endif
