#ifndef MemoryBroker_H
#define MemoryBroker_H

//#include "obj.h"
//#include "thread.h"


/*---------------------------------------------------------------------------*/

extern void* alloc_mem_aligned( ___SIZE_TS words, 
                                unsigned int multiplier,
                                unsigned int modulus );
extern void free_mem_aligned(void *ptr);

class MemoryBroker : public ___vmstate_mem, public ___vmstate_gcstats {
    
    private:
    //___MUTEX still_objs_lock_;
    ___WORD tospace_offset_;
    
    public:
    static const ___WORD MSECTION_SIZE, MSECTION_HALF;

    MemoryBroker();    
    friend ___SCMOBJ ___setup_mem_vmstate(___virtual_machine_state ___vms);
    friend void ___cleanup_mem_vmstate(___virtual_machine_state ___vms);
    
    inline ___WORD getTospaceOffset() const {
        return tospace_offset_;
    }
    
    inline void toggleTospace() {
        tospace_at_top_ = !tospace_at_top_;
        tospace_offset_ = tospace_at_top_ ? MSECTION_HALF : 0;
    }
    
    inline void addWordsPreviousSections(___WORD nbWords) {
        words_prev_msections_ += nbWords;
    }
    
    ___msection* nextMemorySection();
    void markStillObjectForScan(___WORD* stillObject);

    ___SCMOBJ nextExecutableWill();
    
    //TODO change those?
    ___WORD* getStartOfTospace(___msection* ms) const;
    ___WORD* getStartOfFromspace(___msection* ms) const;
    
    int find_msection (void* ptr) const;
    void adjust_msections(int n);
    void free_msections();
    
    private:
    void setup_rc();
    void cleanup_rc();
};

#endif
