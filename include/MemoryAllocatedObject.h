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

class MemoryAllocatedObject {
  
  	public:
	
    ___WORD *body;
    ___WORD head;
    int subtype;

    MemoryAllocatedObject(___WORD obj);
};

class MovableObject : public MemoryAllocatedObject {
    public:

    MovableObject(___WORD obj);
    
    ___SIZE_TS getSize();
    ___WORD *move(___WORD *dest);
};

class StillObject : public MemoryAllocatedObject {
    public:

    StillObject(___WORD obj);
    
    bool isMarked();
    ___WORD mark(___WORD scanList);
};

#endif
