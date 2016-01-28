#ifndef MEM_STRUCT_H
#define MEM_STRUCT_H

#include "def.h"

/* Global variable structure */

typedef struct ___glo_struct
  {
    ___WORD val;
    ___WORD prm;
    struct ___glo_struct* next;
  } ___glo_struct;


/* Memory management */

/*
 * Movable Scheme objects are allocated in an area of memory
 * distributed in multiple noncontiguous sections (collectively
 * called the "msections").  All sections are of the same size and are
 * allocated through the '___alloc_mem' function.  The number of
 * sections can expand and contract to accommodate the needs of the
 * program.
 */

typedef struct ___msect
  {
    int index;             /* index in list of sections */
    int pos;               /* position in msections's 'sections' array */
    ___WORD *alloc;        /* heap allocation pointer, grows towards high addr. */
    struct ___msect *prev; /* previous section in list of sections */
    struct ___msect *next; /* next section in list of sections */
    ___WORD base[1];       /* content of section */
  } ___msection;

#define ___sizeof_msection(n) (sizeof (___msection) + ((n)-1) * ___WS)

typedef struct
  {
    int max_nb_sections;      /* actual size of 'sections' array */
    int nb_sections;          /* number of sections */
    ___msection *head;        /* head of doubly-linked list of sections */
    ___msection *tail;        /* tail of doubly-linked list of sections */
    ___msection *sections[1]; /* each section ordered by address */
                              /* (increasing order if ___ALLOC_MEM_UP */
                              /* is defined otherwise decreasing order) */
  } ___msections;

#define ___sizeof_msections(n) (sizeof (___msections) + ((n)-1) * sizeof (___msection*))

/*
 * Header structure of reference counted blocks of memory.
 */

typedef struct ___rc_header_struct
  {
    struct ___rc_header_struct *prev;
    struct ___rc_header_struct *next;
    ___SCMOBJ refcount; /* integer but declared ___SCMOBJ for alignment */
    ___SCMOBJ data; /* needed for C closures */
  } ___rc_header;


/*---------------------------------------------------------------------------*/

typedef struct ___pstate_mem_struct {

/* msection where continuation frames are currently being allocated */
___msection *stack_msection_;

/* start of allocation of continuation frames in stack_msection */
___WORD *alloc_stack_start_;

/* allocation pointer for continuation frames in stack_msection */
___WORD *alloc_stack_ptr_;

/* allocation limit for continuation frames in stack_msection */
___WORD *alloc_stack_limit_;

/* msection where movable objects are currently being allocated */
___msection *heap_msection_;

/* start of allocation of movable objects in heap_msection */
___WORD *alloc_heap_start_;

/* allocation pointer for movable objects in heap_msection */
___WORD *alloc_heap_ptr_;

/* allocation limit for movable objects in heap_msection */
___WORD *alloc_heap_limit_;

/* list of nonexecutable wills */
___WORD nonexecutable_wills_;

#ifdef ___CALL_GC_FREQUENTLY
int gc_calls_to_punt_;
#endif
} ___pstate_mem;

typedef struct ___vmstate_mem_struct {

/* size of heap in words (number of words that can be occupied) */
___SIZE_TS heap_size_;

/*
 * 'normal_overflow_reserve' is the number of words reserved in the
 * heap in normal circumstances for handling heap overflows.
 */
___SIZE_TS normal_overflow_reserve_;

/*
 * 'overflow_reserve' is the number of words currently reserved in the
 * heap for handling heap overflows.  Initially 'overflow_reserve' is
 * set to 'normal_overflow_reserve'.  When a heap overflow occurs,
 * some fraction of the 'overflow_reserve' is made available to the
 * heap overflow handler.  When a GC makes at least
 * 'normal_overflow_reserve' free, then 'overflow_reserve' is reset to
 * 'normal_overflow_reserve'.
 */
___SIZE_TS overflow_reserve_;

/* words occupied by nonmovable objects */
___SIZE_TS words_nonmovable_;

/* words occupied in heap by movable objects excluding current msections */
___SIZE_TS words_prev_msections_;

/* list of still objects */
___WORD still_objs_;

/* still objects remaining to scan */
___WORD still_objs_to_scan_;

/* the msections */
___msections *the_msections_;

/* location of tospace in each msection */
___BOOL tospace_at_top_;

/* number of msections used */
int nb_msections_used_;

/* last msection allocated */
___msection *alloc_msection_;

/* msection currently being scanned */
___msection *scan_msection_;

/* scan pointer in msection being scanned */
___WORD *scan_ptr_;

/* indicates if weak references must be traversed */
___BOOL traverse_weak_refs_;

/* GC hash tables reached by GC */
___WORD reached_gc_hash_tables_;

/* list of executable wills */
___WORD executable_wills_;

/* Head of list of reference counted blocks of memory */
___rc_header rc_head_;

/*
 * Garbage collection statistics.
 */

___F64 nb_gcs_;
___F64 gc_user_time_;
___F64 gc_sys_time_;
___F64 gc_real_time_;
___F64 bytes_allocated_minus_occupied_;

___F64 last_gc_user_time_;
___F64 last_gc_sys_time_;
___F64 last_gc_real_time_;
___F64 last_gc_heap_size_;
___F64 last_gc_alloc_;
___F64 last_gc_live_;
___F64 last_gc_movable_;
___F64 last_gc_nonmovable_;

} ___vmstate_mem;

typedef struct ___gstate_mem_struct {

  /*
   * Permanent objects are allocated in sections called "psections".
   * Each section contains multiple objects.  The sections are kept in a
   * list so that the storage they occupy can be reclaimed when the
   * program terminates.
   */

  void *psections;       /* list of psections */
  ___WORD *palloc_ptr;   /* allocation pointer in current psection */
  ___WORD *palloc_limit; /* allocation limit in current psection */

  /*
   * Global variables.
   */

  ___SIZE_T nb_glo_vars;    /* number of global variables */

  ___glo_struct *glo_list_head;  /* list of global variables */
  ___glo_struct *glo_list_tail;

} ___gstate_mem;

#endif

