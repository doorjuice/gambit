#ifndef MemoryBroker_H
#define MemoryBroker_H

//#include "obj.h"
//#include "thread.h"


/*---------------------------------------------------------------------------*/

extern void* alloc_mem_aligned( ___SIZE_TS words, 
                                unsigned int multiplier,
                                unsigned int modulus );

extern void free_mem_aligned(void *ptr);

extern ___SIZE_TS scan(___PSD ___WORD *body);


class MemoryManager;

class MemoryBroker : public ___vmstate_mem, public ___vmstate_gcstats {
    
    private:
    //___vmstate_gcstats gcstats_; TODO replace inheritance with composition
    //___MUTEX still_objs_lock_;
    
    public:
    static const ___WORD MSECTION_SIZE, MSECTION_HALF;

    MemoryBroker();    
    friend ___SCMOBJ ___setup_mem_vmstate(___virtual_machine_state ___vms);
    friend void ___cleanup_mem_vmstate(___virtual_machine_state ___vms);
    
    inline ___WORD getHeapSize() const {
        return heap_size_;
    }
    
    inline ___WORD getWordsNonMovable() const {
        return words_nonmovable_;
    }
    
    inline void addWordsNonMovable(___WORD nbWords) {
        words_nonmovable_ += nbWords;
    }
    
    inline void releaseWordsNonMovable(___WORD nbWords) {
        words_nonmovable_ -= nbWords;
    }
    
    inline ___WORD getWordsPreviousSections() const {
        return words_prev_msections_;
    }
    
    inline void addWordsPreviousSections(___WORD nbWords) {
        words_prev_msections_ += nbWords;
    }
    
    inline ___WORD* getStartOfTospace(___msection* ms) const {
        return ms->base + (tospace_at_top_ ? MSECTION_HALF : 0);
        //return ms->base + (tospace_at_top_ & MSECTION_HALF);
    }
    
    inline ___WORD* getStartOfFromspace(___msection* ms) const {
        return ms->base + (tospace_at_top_ ? 0 : MSECTION_HALF);
        //return ms->base + (~tospace_at_top_ & MSECTION_HALF);
    }

    inline void toggleTospace() {
        tospace_at_top_ = !tospace_at_top_;
        //tospace_at_top_ = ~tospace_at_top_;
    }
    
    inline ___BOOL traverseWeakRefs() const {
        return traverse_weak_refs_;
    }
    
    inline ___WORD getHashTablesReached() const {
        return gc_hash_tables_reached_;
    }
    
    inline ___WORD* getExecutableWillsList() {
        return &executable_wills_;
    }
    
    inline ___WORD* getStillObjectsList() {
        return &still_objs_;
    }
    
    //TODO still_objs_to_scan could also be unique per thread
    void addStillObject(___WORD* stillObject);
    void markStillObjectForScan(___WORD* stillObject);
    void markReachedHashTable(___WORD* hashTable);
    
    void scanStillObjects(___PSDNC);
    void scanMovableObjects(___PSDNC);
    
    ___msection* nextMemorySection();
    ___SCMOBJ nextExecutableWill();
    
    void prepareGC(MemoryManager& psmem);
    void initStillObjectsToScan();
    void updateStats(const MemoryManager& psmem,
                     ___F64 userTime, ___F64 systemTime, ___F64 realTime);
    
    int find_msection (void* ptr) const;
    void adjust_msections(int n);
    void free_msections();
    
    inline ___rc_header* get_rc_list() { return &rc_head_; }
    void* alloc_rc(___rc_header* rch);
    
    private:
    void setup_rc();
    void cleanup_rc();
};

#endif
