#include "MemoryAllocatedObject.h"

MemoryAllocatedObject::MemoryAllocatedObject(___WORD obj) {
    //assert(___MEM_ALLOCATED(obj));
    body = ___UNTAG(obj) + ___BODY_OFS;
    head = body[-1];
    subtype = ___HD_SUBTYPE(head);
}


// ----------------

MovableObject::MovableObject(___WORD obj)
    : MemoryAllocatedObject(obj) {
    ;
}

___SIZE_TS MovableObject::getSize() {
    return ___HD_WORDS(head);
}

___WORD* MovableObject::move(___WORD* dest) {
    *dest++ = head;
    body[-1] = ___TAG((dest - ___BODY_OFS), ___FORW);
    ___SIZE_TS words = getSize();
    while (words-- > 0)
        *dest++ = *body++;
    return dest;
}


// ----------------

StillObject::StillObject(___WORD obj)
    : MemoryAllocatedObject(obj) {
    ;
}

bool StillObject::isMarked() {
    return body[___STILL_MARK_OFS - ___STILL_BODY_OFS] != -1;
}

___WORD StillObject::mark(___WORD scanList) {
    //assert(!isMarked()); TODO
    body[___STILL_MARK_OFS - ___STILL_BODY_OFS] = scanList;
    scanList = ___CAST(___WORD,body - ___STILL_BODY_OFS);
    return scanList;
}
