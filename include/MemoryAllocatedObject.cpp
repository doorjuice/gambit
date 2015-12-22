#include "MemoryAllocatedObject.h"

MemoryAllocatedObject::MemoryAllocatedObject(___WORD obj) {
    body = ___UNTAG(obj) + ___BODY_OFS;
    head = body[-1];
    subtype = ___HD_SUBTYPE(head);
    head_typ = ___HD_TYP(head);
}

___SIZE_TS MemoryAllocatedObject::getSize() {
    return ___HD_WORDS(head);
}

___WORD *MemoryAllocatedObject::move(___WORD *dest) {
    *dest++ = head;
    body[-1] = ___TAG((dest - ___BODY_OFS), ___FORW);
    ___SIZE_TS words = getSize();
    while (words-- > 0)
        *dest++ = *body++;
    return dest;
}
