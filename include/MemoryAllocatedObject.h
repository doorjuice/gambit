#ifndef MemoryAllocatedObject_H
#define MemoryAllocatedObject_H

class MemoryAllocatedObject {
  
  	public:
	
    ___WORD *body;
    ___WORD head;
    int head_typ;
    int subtype;

    MemoryAllocatedObject(___WORD obj);
    ___SIZE_TS getSize();
    ___WORD *move(___WORD *dest);
};

#endif
