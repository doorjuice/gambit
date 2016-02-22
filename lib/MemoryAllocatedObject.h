#ifndef MemoryAllocatedObject_H
#define MemoryAllocatedObject_H

#include "gambit.h"
#include "mem.h"


/*---------------------------------------------------------------------------*/

class MovableObject;
class StillObject;

class MemoryAllocatedObject {

    protected:
    ___WORD* body;

    private:
    ___WORD head;
    ___SIZE_TS length;
    int subtype;

    public:
    MemoryAllocatedObject(___WORD* obj);
    
    inline ___WORD getHead() const {
        return head;
    }
    
    inline ___SIZE_TS getLength() const {
        return length;
    }
    
    inline int getSubtype() const {
        return subtype;
    }
    
    inline bool isMovable() const {
        return ___HD_TYP(head) == ___MOVABLE0;
    }
    
    inline bool isStill() const {
        return ___HD_TYP(head) == ___STILL;
    }
    
    inline bool isPermanent() const {
        return ___HD_TYP(head) == ___PERM;
    }
    
    inline bool isForwarded() const {
        return ___TYP(___HD_TYP(head)) == ___FORW;
    }
    
    MovableObject* asMovable();
    StillObject* asStill();
};

class MovableObject : public MemoryAllocatedObject {

    private:
    static const ___WORD paddingData = ___MAKE_HD_WORDS(0, ___sVECTOR);
    
#ifdef GATHER_STATS
    static const ___SIZE_TS MAX_STAT_SIZE = 20;
    static ___SIZE_TS movable_pair_objs;
    static ___SIZE_TS movable_subtyped_objs[MAX_STAT_SIZE + 1];
#endif

    public:
    MovableObject(___WORD* obj);
    
    ___SIZE_TS getSize() const;
    ___WORD* move(___PSDNC);
    
    private:
    ___WORD* forwardTo(___WORD* dest);
    
#ifdef GATHER_STATS
    void gatherStats();
    public: static void resetStats();
#endif
};

class StillObject : public MemoryAllocatedObject {

    public:
    StillObject(___WORD* obj);
    
    inline bool isMarked() const {
        return body[___STILL_MARK_OFS - ___STILL_BODY_OFS] != -1;
    }
    
    void mark(___PSDNC);
};

#endif
