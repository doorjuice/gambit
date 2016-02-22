#ifndef OBJ_H
#define OBJ_H

// require "typ.h"

/* Object Representation */

/*
 * For a complete description of the object representation, read the
 * file "lib/mem.c".  These definitions must agree with those in the
 * files "lib/_gambit#.scm" and "gsc/_t-c-3.scm".
 */

/*
 * Type tag assignment.
 *
 * Type tags are located in the lower 2 bits of a ___SCMOBJ.
 *
 * ___TB = number of tag bits
 * ___tFIXNUM   = tag for fixnums (small integers), must be 0
 * ___tSPECIAL  = tag for other immediates (#f, #t, (), #!eof, chars, ...)
 * ___tMEM1     = tag #1 for memory allocated objects (lower bit must be 1)
 * ___tMEM2     = tag #2 for memory allocated objects (lower bit must be 1)
 * ___tSUBTYPED = ___tMEM1
 * ___tPAIR     = ___tMEM1 or ___tMEM2
 */

#define ___TB 2
#define ___tFIXNUM   0
#define ___tSPECIAL  2
#define ___tMEM1     1
#define ___tMEM2     3
#define ___tSUBTYPED ___tMEM1

#ifdef ___USE_SAME_TAG_FOR_PAIRS_AND_SUBTYPED
#define ___tPAIR ___tMEM1
#else
#define ___tPAIR ___tMEM2
#endif

#define ___MEM_ALLOCATED(obj)((obj)&1)
#define ___MEM_ALLOCATED_CLEAR(obj)((obj)&~___CAST(___WORD,1))
#define ___MEM_ALLOCATED_SET(obj)((obj)|1)

/*
 * Subtype tag assignment.
 *
 * These tags are stored in the head of memory allocated objects
 * (including pairs).
 *
 * ___SB = number of subtype tag bits
 * ___sVECTOR       = tag for vectors
 * ___sPAIR         = tag for pairs
 * ___sRATNUM       = tag for ratnums
 * ___sCPXNUM       = tag for cpxnums
 * ___sSTRUCTURE    = tag for structures
 * ___sBOXVALUES    = tag for box and multiple-values objects
 * ___sMEROON       = tag for Meroon objects
 * ___sJAZZ         = tag for Jazz objects
 * ___sSYMBOL       = tag for symbols
 * ___sKEYWORD      = tag for keywords
 * ___sFRAME        = tag for continuation frames
 * ___sCONTINUATION = tag for continuations
 * ___sPROMISE      = tag for promises
 * ___sWEAK         = tag for weak objects (wills and GC hash tables)
 * ___sPROCEDURE    = tag for procedures
 * ___sRETURN       = tag for returns
 * ___sFOREIGN      = tag for foreign data
 * ___sSTRING       = tag for strings
 * ___sS8VECTOR     = tag for 8-bit signed integer vectors
 * ___sU8VECTOR     = tag for 8-bit unsigned integer vectors
 * ___sS16VECTOR    = tag for 16-bit signed integer vectors
 * ___sU16VECTOR    = tag for 16-bit unsigned integer vectors
 * ___sS32VECTOR    = tag for 32-bit signed integer vectors
 * ___sU32VECTOR    = tag for 32-bit unsigned integer vectors
 * ___sF32VECTOR    = tag for 32-bit floating point number vectors
 * ___sS64VECTOR    = tag for 64-bit signed integer vectors
 * ___sU64VECTOR    = tag for 64-bit unsigned integer vectors
 * ___sF64VECTOR    = tag for 64-bit floating point number vectors
 * ___sFLONUM       = tag for flonums
 * ___sBIGNUM       = tag for bignums
 *
 * The five subtype tags ___sS64VECTOR, ___sU64VECTOR, ___sF64VECTOR,
 * ___sFLONUM, ___sBIGNUM must come last.  These object types (and
 * ___sFOREIGN which is always allocated as a ___STILL object) must
 * be aligned on a multiple of 8.
 */

#define ___SB 5
#define ___sVECTOR       0
#define ___sPAIR         1
#define ___sRATNUM       2
#define ___sCPXNUM       3
#define ___sSTRUCTURE    4
#define ___sBOXVALUES    5
#define ___sMEROON       6
#define ___sJAZZ         7
#define ___sSYMBOL       8
#define ___sKEYWORD      9
#define ___sFRAME        10
#define ___sCONTINUATION 11
#define ___sPROMISE      12
#define ___sWEAK         13
#define ___sPROCEDURE    14
#define ___sRETURN       15
#define ___sFOREIGN      18
#define ___sSTRING       19
#define ___sS8VECTOR     20
#define ___sU8VECTOR     21
#define ___sS16VECTOR    22
#define ___sU16VECTOR    23
#define ___sS32VECTOR    24
#define ___sU32VECTOR    25
#define ___sF32VECTOR    26
#define ___sS64VECTOR    27
#define ___sU64VECTOR    28
#define ___sF64VECTOR    29
#define ___sFLONUM       30
#define ___sBIGNUM       31

/*
 * Head type tag assignment.
 *
 * Head type tags are stored in the lower 3 bits of the head of memory
 * allocated objects.
 *
 * ___HTB = number of head tag bits
 * ___MOVABLE0 = tag for movable objects in generation 0
 * ___MOVABLE1 = tag for movable objects in generation 1
 * ___MOVABLE2 = tag for movable objects in generation 2
 * ___FORW     = tag for movable objects that have been forwarded
 * ___STILL    = tag for still objects
 * ___PERM     = tag for permanent objects
 *
 * note: the tag ___FORW+(1<<___TB) is also used for forwarding pointers
 */

#define ___HTB 3
#define ___MOVABLE0 0
#define ___MOVABLE1 1
#define ___MOVABLE2 2
#define ___FORW     3
#define ___STILL    5
#define ___PERM     6

/*
 * Miscellaneous masks.
 */

#define ___LF (___HTB+___SB)

#define ___MASK(type,bits)((___CAST(type,1)<<bits)-1)
#define ___TMASK ___MASK(___WORD,___TB)
#define ___HTMASK ___MASK(___WORD,___HTB)
#define ___SMASK (___MASK(___WORD,___SB)<<___HTB)
#define ___LMASK (~___MASK(unsigned ___WORD,___LF))
#define ___LSMASK (~___MASK(unsigned ___WORD,___HTB))

/*
 * Value constructors for object references inside executable code.
 */

#define ___FAL      ___SPECIAL(-1)
#define ___TRU      ___SPECIAL(-2)
#define ___NUL      ___SPECIAL(-3)
#define ___EOF      ___SPECIAL(-4)
#define ___VOID     ___SPECIAL(-5)
#define ___ABSENT   ___SPECIAL(-6)
#define ___UNB1     ___SPECIAL(-7)
#define ___UNB2     ___SPECIAL(-8)
#define ___OPTIONAL ___SPECIAL(-9)
#define ___KEYOBJ   ___SPECIAL(-10)
#define ___REST     ___SPECIAL(-11)
#define ___UNUSED   ___SPECIAL(-14)
#define ___DELETED  ___SPECIAL(-15)
#define ___SPECIAL(x)(___FIX(x)+___tSPECIAL)
#define ___FIX(x)___CAST(___WORD,___CAST(unsigned ___WORD,x)<<___TB)
#if ___SCMOBJ_WIDTH == 32
#define ___BIGFIX(i,x)___SUB(i)
#else
#define ___BIGFIX(i,x)___FIX(x)
#endif
#define ___CHR(x)___SPECIAL(___CS_SELECT(___CAST_U8(x),___CAST_U16(x),___CAST_U32(x)))

#define ___SYM(i,id)___CAST_FAKEWORD_TO_WORD(___symtbl[i])
#define ___KEY(i,id)___CAST_FAKEWORD_TO_WORD(___keytbl[i])
#define ___CNS(i)___TAG((___ALIGNUP(___cnstbl,___WS)+i*(___PAIR_SIZE+1)),___tPAIR)
#define ___SUB(i)___CAST_FAKEWORD_TO_WORD(___subtbl[i])

/*
 * Value constructors for object references inside constant
 * memory allocated objects.
 */

#define ___REF_FAL      ___FAL
#define ___REF_TRU      ___TRU
#define ___REF_NUL      ___NUL
#define ___REF_EOF      ___EOF
#define ___REF_VOID     ___VOID
#define ___REF_ABSENT   ___ABSENT
#define ___REF_UNB1     ___UNB1
#define ___REF_UNB2     ___UNB2
#define ___REF_OPTIONAL ___OPTIONAL
#define ___REF_KEYOBJ   ___KEYOBJ
#define ___REF_REST     ___REST
#define ___REF_UNUSED   ___UNUSED
#define ___REF_DELETED  ___DELETED
#define ___REF_FIX(x)___FIX(x)
#if ___SCMOBJ_WIDTH == 32
#define ___REF_BIGFIX(i,x)___REF_SUB(i)
#else
#define ___REF_BIGFIX(i,x)___REF_FIX(x)
#endif
#define ___REF_CHR(x)___CHR(x)

#define ___REF_SYM(i,id)(___FIX(-1-i)+___tMEM2)
#define ___REF_KEY(i,id)(___FIX(-1-i)+___tMEM1)
#define ___REF_CNS(i)(___FIX(i)+___tMEM2)
#define ___REF_SUB(i)(___FIX(i)+___tMEM1)

#ifdef ___SUBCOUNT
#define ___REF_PRC(i)(___FIX(___SUBCOUNT+i)+___tMEM1)
#else
#define ___REF_PRC(i)(___FIX(i)+___tMEM1)
#endif


/*---------------------------------------------------------------------------*/

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


/*---------------------------------------------------------------------------*/

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

/*
 * Garbage collection statistics.
 */

typedef struct ___vmstate_gcstats_struct
  {
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
  } ___vmstate_gcstats;


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
___WORD tospace_at_top_;

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
___WORD gc_hash_tables_reached_;

/* list of executable wills */
___WORD executable_wills_;

/* Head of list of reference counted blocks of memory */
___rc_header rc_head_;

// GC statistics have been moved to the MemoryBroker

} ___vmstate_mem;


/*---------------------------------------------------------------------------*/

/* Global variable structure */

typedef struct ___glo_struct
  {
    ___WORD val;
    ___WORD prm;
    struct ___glo_struct* next;
  } ___glo_struct;

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

