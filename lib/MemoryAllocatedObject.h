#ifndef MemoryAllocatedObject_H
#define MemoryAllocatedObject_H

/* Constants related to representation of permanent and still objects: */

#ifdef ___USE_HANDLES
    #define ___PERM_HAND_OFS 0
    #define ___PERM_BODY_OFS 2
#else
    #define ___PERM_HAND_OFS ___PERM_BODY_OFS
    #define ___PERM_BODY_OFS 1
#endif

#define ___STILL_LINK_OFS 0
#define ___STILL_REFCOUNT_OFS 1
#define ___STILL_LENGTH_OFS 2
#define ___STILL_MARK_OFS 3
#ifdef ___USE_HANDLES
    #define ___STILL_HAND_OFS 4
    #define ___STILL_BODY_OFS 6
#else
    #define ___STILL_HAND_OFS ___STILL_BODY_OFS
    #define ___STILL_BODY_OFS (5+1)/************/
#endif

/*---------------------------------------------------------------------------*/

#ifdef GATHER_STATS
    #define MAX_STAT_SIZE 20
    ___HIDDEN ___SIZE_TS movable_pair_objs;
    ___HIDDEN ___SIZE_TS movable_subtyped_objs[MAX_STAT_SIZE+2];
#endif

/*---------------------------------------------------------------------------*/

static void next_heap_msection(___processor_state ___ps);

class MovableObject;
class StillObject;

class MemoryAllocatedObject {
  
  	protected:
    ___WORD* body;
    ___WORD head;
    ___SIZE_TS length;
    int subtype;

    public:
    MemoryAllocatedObject(___WORD obj);
    
    ___WORD getHead() const {
        return head;
    }
    
    int getSubtype() const {
        return subtype;
    }
    
    bool isPermanent() const;
    bool isForwarded() const;
    
    MovableObject* asMovable();
    StillObject* asStill();
};

class MovableObject : public MemoryAllocatedObject {
    static const ___WORD paddingData = ___MAKE_HD_WORDS(0, ___sVECTOR);
    
    public:
    MovableObject(___WORD obj);
    
    ___SIZE_TS getSize() const;
    ___WORD* move(___PSD ___WORD* alloc);
    
    private:
    ___WORD* requireMemory(___PSD ___WORD* alloc);
    void gatherStats();
    ___WORD* moveData(___WORD* dest);
};

class StillObject : public MemoryAllocatedObject {
    public:
    StillObject(___WORD obj);
    
    bool isMarked();
    ___WORD mark(___WORD scanList);
};

#endif
