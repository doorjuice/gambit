#ifndef MemoryBroker_H
#define MemoryBroker_H

//#include "obj.h"
//#include "thread.h"


/*---------------------------------------------------------------------------*/

extern void ___fatal_error(char** msgs);

extern void  free_mem_aligned(void* ptr);
extern void* alloc_mem_aligned( ___SIZE_TS words, 
                                unsigned int multiplier,
                                unsigned int modulus );

extern void process_wills(___PSDNC);
extern ___SIZE_TS scan(___PSD ___WORD *body);


class MemoryManager;

class MemoryBroker : private ___vmstate_mem {
    
    private:
    ___vmstate_gcstats gcstats_;
    //___MUTEX still_objs_lock_;
    
    public:
    static const ___WORD MSECTION_SIZE, MSECTION_HALF;
    static void reportFatalOverflow(char* msg);

    MemoryBroker();    
    friend ___SCMOBJ ___setup_mem_vmstate(___virtual_machine_state ___vms);
    friend void ___cleanup_mem_vmstate(___virtual_machine_state ___vms);
    
    ___WORD getWordsMovableUsable() const; /* words usable in msections */
    ___WORD getWordsAvailable() const; /* words available in heap */
    ___BOOL refreshOverflowReserve(___WORD wordsOccupied);
    
    inline ___SIZE_TS getNormalOverflowReserve() const {
        return normal_overflow_reserve_;
    }
    
    inline int getNbSectionsUsed() const {
        return nb_msections_used_;
    }
    
    inline int getNbSections() const {
        return the_msections_->nb_sections;
    }
    
    inline ___WORD getHeapSize() const {
        return heap_size_;
    }
    
    inline void refreshHeapSize() {
        heap_size_ = getWordsAvailable();
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
        return ms->base + (tospace_at_top_ & MSECTION_HALF);
    }
    
    inline ___WORD* getStartOfFromspace(___msection* ms) const {
        return ms->base + (~tospace_at_top_ & MSECTION_HALF);
    }
    
    inline ___WORD* getStartOfFromspace(int iSection) const {
        return getStartOfFromspace(the_msections_->sections[iSection]);
    }
    
    inline ___WORD* getStartOfFromspace() const {
        return getStartOfFromspace(the_msections_->head);
    }
    
    inline void toggleTospace() {
        tospace_at_top_ = ~tospace_at_top_;
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
    
    void scanMemoryObjects(___PSDNC);
    void scanStillObjects(___PSDNC);
    void scanMovableObjects(___PSDNC);
    
    ___msection* nextMemorySection();
    ___SCMOBJ nextExecutableWill();
    
    void prepareGC(MemoryManager& psmem);
    void initStillObjectsToScan();
    void updateStats(const MemoryManager& psmem,
                     ___F64 userTime, ___F64 systemTime, ___F64 realTime);
    
    inline const ___vmstate_gcstats* getStats() const { return &gcstats_; }
    inline ___F64 get_bytes_allocated_minus_occupied() const {
        return gcstats_.bytes_allocated_minus_occupied_;
    }
    inline ___F64 set_bytes_allocated_minus_occupied(___F64 val) {
        gcstats_.bytes_allocated_minus_occupied_ = val;
    }
    
    int  find_msection (void* ptr) const;
    void adjust_msections(int n);
    void free_msections();
    
    inline ___rc_header* get_rc_list() { return &rc_head_; }
    void* alloc_rc(___rc_header* rch);
    
    private:
    void setup_rc();
    void cleanup_rc();
    
#ifdef ___DEBUG_GARBAGE_COLLECT
    friend void dump_memory_map(___PSDNC);
    friend void validate_old_obj(___PSD ___WORD obj);
#endif
};

#endif
