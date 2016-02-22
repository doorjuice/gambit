#include "MemoryAllocatedObject.h"

#include <assert.h>
#include <string.h>


___WORD* ___memcpy (___WORD* dest, const ___WORD* src, ___SIZE_T words) {
    /* fallback version:
    while (words-- > 0)
        *dest++ = *src++;
    */
    memcpy(dest, src, words << ___LWS);
    return dest;
}

MemoryAllocatedObject::MemoryAllocatedObject(___WORD* obj) {
    body = obj + ___BODY_OFS;
    head = body[-1];
    length = ___HD_WORDS(head);
    subtype = ___HD_SUBTYPE(head);
}

MovableObject* MemoryAllocatedObject::asMovable() {
    assert(isMovable());
    return static_cast<MovableObject*>(this);
}

StillObject* MemoryAllocatedObject::asStill() {
    assert(isStill());
    return static_cast<StillObject*>(this);
}


/*---------------------------------------------------------------------------*/

#ifdef GATHER_STATS
___SIZE_TS MovableObject::movable_pair_objs;
___SIZE_TS MovableObject::movable_subtyped_objs[MovableObject::MAX_STAT_SIZE + 1];
#endif


MovableObject::MovableObject(___WORD* obj)
    : MemoryAllocatedObject(obj) {
    assert(isMovable());
}

___SIZE_TS MovableObject::getSize() const {
#if ___WS == 4
    return getLength() + (getSubtype() >= ___sS64VECTOR ? 2 : 1);
#else
    return getLength() + 1;
#endif
}

___WORD* MovableObject::move(___PSDNC) {
    ___PSGET
    ___WORD* alloc = ___ps_mem.requireHeapSpace(getSize());
    
#ifdef GATHER_STATS
    gatherStats();
#endif
    
#if ___WS == 4
    /*
    * ___sS64VECTOR, ___sU64VECTOR, ___sF64VECTOR,
    * ___sFLONUM and ___sBIGNUM need to be aligned on a
    * multiple of 8.
    */
    int padding = 0;
    if (getSubtype() >= ___sS64VECTOR) {
        if ((___CAST(___WORD,alloc) & (8-1)) == 0)
            *alloc++ = paddingData;
        else
            padding = 1;
    }
#endif

    alloc = forwardTo(alloc);

#if ___WS == 4
    if (padding)
        *alloc++ = paddingData;
#endif

    ___ps_mem.updateHeapPtr(alloc);
    return body - ___BODY_OFS;
}

___WORD* MovableObject::forwardTo(___WORD* dest) {
    *dest++ = getHead();
    body[-1] = ___TAG((dest - ___BODY_OFS), ___FORW);
    body = ___memcpy(dest, body, getLength());
    return dest + getLength();
    /*if (___COMPARE_AND_SWAP_WORD(body-1, getHead(), 
                                 ___TAG((dest+1 - ___BODY_OFS), ___FORW)) 
        == getHead()) {
        *dest++ = getHead();
        body = ___memcpy(dest, body, getLength());
        return body + getLength();
    }
    else {
        body = ___UNTAG_AS(body[-1], ___FORW) + ___BODY_OFS;
        return dest;
    }*/
}

#ifdef GATHER_STATS

void MovableObject::gatherStats() {
    if (getSubtype() == ___sPAIR)
        movable_pair_objs++;
    else if (getLength() < MAX_STAT_SIZE)
        movable_subtyped_objs[getLength()]++;
    else
        movable_subtyped_objs[MAX_STAT_SIZE]++;
}

void MovableObject::resetStats() {
    movable_pair_objs = 0;
    for (int i = 0; i <= MAX_STAT_SIZE; i++)
        movable_subtyped_objs[i] = 0;
}

#endif

/*---------------------------------------------------------------------------*/

StillObject::StillObject(___WORD* obj)
    : MemoryAllocatedObject(obj) {
    assert(isStill());
}

void StillObject::mark(___PSDNC) {
    assert(!isMarked());
    ___PSGET
    ___vm_mem.markStillObjectForScan(body - ___STILL_BODY_OFS);
}
