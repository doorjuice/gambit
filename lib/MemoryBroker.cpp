#include "gambit.h" //TODO replace me with "MemoryBroker.h"
#include "mem.h"


const ___WORD MemoryBroker::MSECTION_SIZE = ___MSECTION_SIZE;
const ___WORD MemoryBroker::MSECTION_HALF = MSECTION_SIZE >> 1;


MemoryBroker::MemoryBroker() {
    
    setup_rc(); /* Setup reference counted memory management. */
  
   /*
    * Set the overflow reserve so that the rest parameter handler can
    * construct the rest parameter list without having to call the
    * garbage collector.
    */
    
    normal_overflow_reserve_ = 2*( (___MAX_NB_PARMS+___SUBTYPED_OVERHEAD) +
                                    ___MAX_NB_ARGS*(___PAIR_SIZE+___PAIR_OVERHEAD) );
    overflow_reserve_ = normal_overflow_reserve_;
    
    /* Setup GC statistics */
    
    nb_gcs_ = 0.0;
    gc_user_time_ = 0.0;
    gc_sys_time_ = 0.0;
    gc_real_time_ = 0.0;
    bytes_allocated_minus_occupied_ = 0.0;
    
    last_gc_real_time_ = 0.0;
    last_gc_heap_size_ = ___CAST(___F64,heap_size_) * ___WS;
    last_gc_live_ = 0.0;
    last_gc_movable_ = 0.0;
    last_gc_nonmovable_ = 0.0;
    
    /* Initialize GC state */
    
    words_nonmovable_ = 0;
    words_prev_msections_ = 0;
    nb_msections_used_ = 0;
    tospace_at_top_ = 0;
    
    executable_wills_ = ___TAG(0,___EXECUTABLE_WILL); /* tagged empty list */
    
    // Warning: the MemoryBroker instance will _not_ be ready for use after being 
    // constructed, since the actual initial allocation of heap space is left for
    // later, specifically once the global setup parameter 'min_heap' is known.
}


void MemoryBroker::addStillObject(___WORD* stillObject) {
    addWordsNonMovable(stillObject[___STILL_LENGTH_OFS]);
    stillObject[___STILL_LINK_OFS] = still_objs_;
    still_objs_ = ___CAST(___WORD, stillObject);
}

void MemoryBroker::markStillObjectForScan(___WORD* stillObject) {
    //___MUTEX_LOCK(still_objs_lock_);
    if (___COMPARE_AND_SWAP_WORD(stillObject + ___STILL_MARK_OFS, -1, 
                                 ___CAST(___WORD, still_objs_to_scan_)) 
        == -1)
        still_objs_to_scan_ = ___CAST(___WORD, stillObject);
    //___MUTEX_UNLOCK(still_objs_lock_);
}

void MemoryBroker::markReachedHashTable(___WORD* hashTable) {
    hashTable[0] = gc_hash_tables_reached_;
    gc_hash_tables_reached_ = ___TAG((hashTable-1), 0);
}


void MemoryBroker::scanStillObjects(___PSDNC) {
    ___PSGET
    ___WORD* base;

    while ((base = ___CAST(___WORD*, still_objs_to_scan_)) != NULL) {
        still_objs_to_scan_ = base[___STILL_MARK_OFS];
        scan (___PSP base + ___STILL_BODY_OFS);
    }
}

void MemoryBroker::scanMovableObjects(___PSDNC) {
    ___PSGET

    //FIXME why does this require the 'active' ps_state_mem?
    while (true) {
        if (scan_msection_ == ___ps_mem.heap_msection_) {
            if (scan_ptr_ >= ___ps_mem.alloc_heap_ptr_)
              break;
        }
        else if (scan_ptr_ >= scan_msection_->alloc) {
            scan_msection_ = scan_msection_->next;
            scan_ptr_ = getStartOfTospace(scan_msection_);
            continue;
        }
        ___WORD* body = scan_ptr_ + 1;
        ___SIZE_TS words = scan (___PSP body);
        scan_ptr_ = body + words;
    }
}


___msection* MemoryBroker::nextMemorySection() {
    ___msection* result;

    if (nb_msections_used_ > 0)
        result = alloc_msection_->next;
    else
        result = the_msections_->head;

    if (result != NULL) {
        alloc_msection_ = result;
        nb_msections_used_++;
    }

    return result;
}

___SCMOBJ MemoryBroker::nextExecutableWill() {
    ___WORD will = executable_wills_;
    if (___UNTAG(will) == 0) /* end of list? */
        return ___FAL;
    else {
        executable_wills_ = ___BODY(will)[0];
        return will;
    }
}


void MemoryBroker::prepareGC(MemoryManager& psmem) {
    words_prev_msections_ = 0;
    nb_msections_used_ = 0;
    toggleTospace();
    
    psmem.stack_msection_ = NULL;
    psmem.heap_msection_ = NULL;
    psmem.nextHeapSection();
    
    scan_msection_ = psmem.heap_msection_;
    scan_ptr_ = psmem.alloc_heap_ptr_;
    
    /* maintain list of GC hash tables reached by GC */
    gc_hash_tables_reached_ = ___TAG(0,0);
    
    /* trace externally referenced still objects */
    initStillObjectsToScan();
}

void MemoryBroker::initStillObjectsToScan() {
    ___WORD* base = ___CAST(___WORD*, still_objs_);
    ___WORD* to_scan = NULL;

    while (base != NULL) {
        if (base[___STILL_REFCOUNT_OFS] == 0)
            base[___STILL_MARK_OFS] = -1;
        else {
            base[___STILL_MARK_OFS] = ___CAST(___WORD, to_scan);
            to_scan = base;
        }
        base = ___CAST(___WORD*, base[___STILL_LINK_OFS]);
    }

  still_objs_to_scan_ = ___CAST(___WORD, to_scan);
}

void MemoryBroker::updateStats(const MemoryManager& psmem,
                               ___F64 userTime, ___F64 systemTime, ___F64 realTime) {
    nb_gcs_ += 1.0;
    gc_user_time_ += userTime;
    gc_sys_time_  += systemTime;
    gc_real_time_ += realTime;
    
    last_gc_user_time_ = userTime;
    last_gc_sys_time_  = systemTime;
    last_gc_real_time_ = realTime;
    last_gc_heap_size_ = ___CAST(___F64, getHeapSize()) * ___WS;
    last_gc_alloc_ = bytes_allocated_minus_occupied_ +
                     ___CAST(___F64, psmem.getWordsOccupied()) * ___WS;
    last_gc_live_ = ___CAST(___F64, psmem.getWordsOccupied()) * ___WS;
    last_gc_movable_ = ___CAST(___F64, psmem.getWordsMovable()) * ___WS;
    last_gc_nonmovable_ = ___CAST(___F64, words_nonmovable_) * ___WS;
}


/*---------------------------------------------------------------------------*/

/* Allocation of movable objects.  */

/*
 * 'find_msection (ms, ptr)' finds the position in the 'ms->sections'
 * array of the msection that contains the pointer 'ptr'.  More
 * precisely, if ___ALLOC_MEM_UP is defined, it returns the integer
 * 'i' (-1<=i<=n-1) such that 'ptr' is between the start of section i
 * and section i+1.  -1 is returned if 'ptr' is lower than the lowest
 * section and 'n' is returned if 'ptr' is not lower than the highest
 * section.  If ___ALLOC_MEM_UP is not defined, it returns the integer
 * 'i' (0<=i<=n) such that 'ptr' is between the start of section i and
 * section i-1.  n is returned if 'ptr' is lower than the lowest
 * section and 0 is returned if 'ptr' is not lower than the highest
 * section.
 */

int MemoryBroker::find_msection(void* ptr) const {
    int nb_sections = the_msections_->nb_sections;
    ___msection** sections = the_msections_->sections;

#ifdef ___ALLOC_MEM_UP
    if (nb_sections == 0 || ptr < ___CAST(void*,sections[0]))
        return -1;
#else
    if (nb_sections == 0 || ptr < ___CAST(void*,sections[nb_sections-1]))
        return nb_sections;
#endif

    int lo = 0, hi = nb_sections-1; /* binary search */
    while (lo < hi) /* loop invariant: lo <= find_msection (ms, ptr) <= hi */ 
    { 
      int mid = (lo+hi) / 2; /* lo <= mid < hi */
#ifdef ___ALLOC_MEM_UP
      if (ptr < ___CAST(void*,sections[mid+1])) hi = mid; else lo = mid+1;
#else
      if (ptr < ___CAST(void*,sections[mid])) lo = mid+1; else hi = mid;
#endif
    }

    return lo;
}

/*
 * 'adjust_msections (msp, n)' contracts or expands the msections
 * pointed to by 'msp' so that it contains 'n' sections.  When the
 * msections is contracted, the last sections allocated (i.e. those at
 * the end of the doubly-linked list of sections) will be reclaimed.
 * When expanding the msections there may not be enough memory to
 * allocate new sections so the operation may fail.  However
 * 'adjust_msections' will always leave the msections in a consistent
 * state and there will be at least as many sections as when the
 * expansion was started.  Failure can be detected by checking the
 * 'nb_sections' field.
 */

void MemoryBroker::adjust_msections(int n) {
    int max_ns, ns;
    ___msections* ms = the_msections_;
    ___msection *hd, *tl;
  
    if (ms == NULL) {
        max_ns = 0;
        ns = 0;
        hd = 0;
        tl = 0;
    }
    else {
        max_ns = ms->max_nb_sections;
        ns = ms->nb_sections;
        hd = ms->head;
        tl = ms->tail;
    }

    if (ms == NULL || n > max_ns) {
        /* must allocate a new msections structure */
        ___msections* new_ms;
  
        while (n > max_ns) /* grow max_nb_sections until big enough */
            max_ns = 2*max_ns + 1;
  
        new_ms = ___CAST(___msections*,
                         alloc_mem_aligned(___WORDS(___sizeof_msections(max_ns)),
                                                    1,
                                                    0));
  
        if (new_ms == 0)
            return;
  
        new_ms->max_nb_sections = max_ns;
        new_ms->nb_sections = ns;
        new_ms->head = hd;
        new_ms->tail = tl;
  
        for (int i = ns-1; i >= 0; i--)
            new_ms->sections[i] = ms->sections[i];
  
        if (ms != NULL)
            free_mem_aligned(ms);
  
        the_msections_ = ms = new_ms;
    }

    if (n < ns) { /* contraction of the msections */
        do {
            ___msection* s = tl;
            tl = tl->prev;
  
            if (tl == NULL)
                hd = NULL;
            else
                tl->next = NULL;
  
            for (int j = s->pos; j < ns-1; j++) {
                ms->sections[j] = ms->sections[j+1];
                ms->sections[j]->pos = j;
            }
  
            free_mem_aligned (s);
        } while (--ns > n);
  
        ms->nb_sections = ns;
        ms->head = hd;
        ms->tail = tl;
  
        /*
        * Contraction of the msections structure is not performed
        * because there is typically very little memory to be
        * reclaimed.
        */
    }
    else { /* expansion of the msections */
        while (ns < n) {
            ___msection* s = ___CAST(___msection*,
                                    alloc_mem_aligned
                                      (___WORDS(___sizeof_msection(___MSECTION_SIZE)),
                                        1,
                                        0));
  
            if (s == 0)
              return;
  
            int i = find_msection(___CAST(void*,s));

#ifdef ___ALLOC_MEM_UP
            i++;
#endif

            for (int j = ns; j > i; j--) {
                ms->sections[j] = ms->sections[j-1];
                ms->sections[j]->pos = j;
            }
  
            ms->sections[i] = s;
  
            if (tl == NULL) {
                hd = s;
                s->index = 0;
            }
            else {
                tl->next = s;
                s->index = tl->index + 1;
            }
  
            s->pos = i;
            s->prev = tl;
            s->next = NULL;
            tl = s;
  
            ms->nb_sections = ++ns;
            ms->head = hd;
            ms->tail = tl;
        }
    }
}

/*
 * 'free_msections (msp)' releases all memory associated with the
 * msections pointed to by 'msp'.
 */

void MemoryBroker::free_msections() {
    if (the_msections_ != NULL) {
        for (int i = the_msections_->nb_sections-1; i >= 0; i--)
            free_mem_aligned(the_msections_->sections[i]);
  
        free_mem_aligned(the_msections_);
        the_msections_ = NULL;
    }
}


/*---------------------------------------------------------------------------*/

/* Allocation of reference counted blocks of memory. */

void* MemoryBroker::alloc_rc(___rc_header* rch) {
    ___rc_header* head = &rc_head_;
    ___rc_header* tail = head->prev;

    rch->prev = tail;
    rch->next = head;
    head->prev = rch;
    tail->next = rch;

    rch->refcount = 1;
    rch->data = ___FAL;

    return ___CAST(void*, rch + 1);
}

void MemoryBroker::setup_rc() {
    rc_head_.prev = &rc_head_;
    rc_head_.next = &rc_head_;
    rc_head_.refcount = 1;
    rc_head_.data = ___FAL;
}

void MemoryBroker::cleanup_rc() {
    ___rc_header* head = rc_head_.next;
    
    rc_head_.prev = &rc_head_;
    rc_head_.next = &rc_head_;
    
    while (head != &rc_head_) {
        ___rc_header* next = head->next;
        ___free_mem(head);
        head = next;
    }
}
