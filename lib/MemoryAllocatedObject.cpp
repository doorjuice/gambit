#include "MemoryAllocatedObject.h"

#include "MemoryManager.h"

/* The following includes are needed for debugging. */
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

___WORD* MovableObject::move(___PSD ___WORD* alloc) {
    ___PSGET
    
#ifdef GATHER_STATS
    gatherStats();
#endif
    
    alloc = requireMemory(___PSP alloc);
    
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

    alloc = forwardTo(alloc) + getLength();

#if ___WS == 4
    if (padding)
        *alloc++ = paddingData;
#endif

    alloc_heap_ptr = alloc;
    return body - ___BODY_OFS;
}

___WORD* MovableObject::requireMemory(___PSD ___WORD* alloc) {
    ___PSGET
    ___WORD* limit = alloc_heap_limit;
    const ___SIZE_TS size = getSize();
    
    while (alloc + size > limit) {
        alloc_heap_ptr = alloc;
        next_heap_msection(___ps);
        alloc = alloc_heap_ptr;
        limit = alloc_heap_limit;
    }
    return alloc;
}

___WORD* MovableObject::forwardTo(___WORD* dest) {
    *dest++ = getHead();
    body[-1] = ___TAG((dest - ___BODY_OFS), ___FORW);
    body = ___memcpy(dest, body, getLength());
    return dest;
}

void MovableObject::gatherStats() {
    if (getSubtype() == ___sPAIR)
        movable_pair_objs++;
    else if (getLength() <= MAX_STAT_SIZE)
        movable_subtyped_objs[getLength()]++;
    else
        movable_subtyped_objs[MAX_STAT_SIZE+1]++;
}


/*---------------------------------------------------------------------------*/

StillObject::StillObject(___WORD* obj)
    : MemoryAllocatedObject(obj) {
    assert(isStill());
}

___WORD StillObject::mark(___WORD scanList) {
    assert(!isMarked());
    body[___STILL_MARK_OFS - ___STILL_BODY_OFS] = scanList;
    scanList = ___CAST(___WORD,body - ___STILL_BODY_OFS);
    return scanList;
}
