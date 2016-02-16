#ifndef MemoryBroker_H
#define MemoryBroker_H

//#include "obj.h"
//#include "thread.h"


/*---------------------------------------------------------------------------*/

class MemoryBroker : public ___vmstate_mem {
    
    private:
    //___MUTEX still_objs_lock_;
    ___WORD tospace_offset_;
    
    public:
    static const ___WORD MSECTION_SIZE, MSECTION_HALF;
    
    friend ___SCMOBJ ___setup_mem_vmstate(___virtual_machine_state ___vms);
    MemoryBroker();
    
    inline ___WORD getTospaceOffset() const {
        return tospace_offset_;
    }
    
    inline void toggleTospace() {
        tospace_at_top_ = !tospace_at_top_;
        tospace_offset_ = tospace_at_top_ ? MSECTION_HALF : 0;
    }
    
    ___msection* nextMemorySection();
    void markStillObjectForScan(___WORD* stillObject);
    
    //DEPRECATED
    ___WORD* getStartOfTospace(___msection* ms) const;
    ___WORD* getStartOfFromspace(___msection* ms) const;
};

#endif
