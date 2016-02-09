#ifndef THREAD_H
#define THREAD_H

#include "typ.h"

/*
 * Thread system inlined operations.
 */

#ifdef ___USE_POSIX_THREAD_SYSTEM

#ifndef ___DONT_HAVE_PTHREAD_H
#include <pthread.h>
#endif

#define ___MUTEX pthread_mutex_t

#define ___MUTEX_INIT(mut) (pthread_mutex_init (&mut, NULL) == 0)
#define ___MUTEX_DESTROY(mut) (pthread_mutex_destroy (&mut) == 0)
#define ___MUTEX_LOCK(mut) (pthread_mutex_lock (&mut) == 0)
#define ___MUTEX_UNLOCK(mut) (pthread_mutex_unlock (&mut) == 0)
#define ___THREAD_ID (pthread_self())

#endif


#ifdef ___USE_WIN32_THREAD_SYSTEM

#define ___MUTEX HANDLE

#define ___MUTEX_INIT(mut) ((mut = CreateMutex (NULL, FALSE, NULL)) != NULL)
#define ___MUTEX_DESTROY(mut) CloseHandle (mut)
#define ___MUTEX_LOCK(mut) (WaitForSingleObject (mut, INFINITE) != WAIT_FAILED)
#define ___MUTEX_UNLOCK(mut) ReleaseMutex (mut)
#define ___THREAD_ID (GetCurrentThreadId())

#endif


#if ___WS == 4
#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
#define ___USE_sync_builtins
#endif
#else
#if ___WS == 8
#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
#define ___USE_sync_builtins
#endif
#endif
#endif


#ifndef ___USE_sync_builtins
#ifdef ___USE_WIN32_THREAD_SYSTEM
#define ___USE_WIN32_sync
#else
#ifdef ___USE_POSIX_THREAD_SYSTEM
#define ___USE_emulated_sync
#else
#define ___USE_sequential_sync
#endif
#endif
#endif


#ifdef ___USE_sync_builtins

#define ___COMPARE_AND_SWAP_WORD(p,oldval,newval) \
__sync_val_compare_and_swap(p,oldval,newval)

#define ___FETCH_AND_ADD_WORD(p,val) \
__sync_fetch_and_add(p,val)

#define ___FETCH_AND_CLEAR_WORD(p) \
__sync_fetch_and_and(p,0)

#endif


#ifdef ___USE_WIN32_sync

#define ___COMPARE_AND_SWAP_WORD(p,oldval,newval) \
___CAST(___WORD,InterlockedCompareExchangePointer(___CAST(void**,p),___CAST(void*,oldval),___CAST(void*,newval)))

#if ___WS == 4
#define ___FETCH_AND_ADD_WORD(p,val) \
(InterlockedAdd(___CAST(LONG volatile *,p),val)-(val))
#define ___FETCH_AND_CLEAR_WORD(p) \
InterlockedAnd(___CAST(LONG volatile *,p),0)
#else
#define ___FETCH_AND_ADD_WORD(p,val) \
(InterlockedAdd64(___CAST(LONGLONG volatile *,p),val)-(val))
#define ___FETCH_AND_CLEAR_WORD(p) \
InterlockedAnd64(___CAST(LONGLONG volatile *,p),0)
#endif

#endif


#ifdef ___USE_emulated_sync

#define ___COMPARE_AND_SWAP_WORD(p,oldval,newval) \
___EXT(___emulated_compare_and_swap_word)(p,oldval,newval)

#define ___FETCH_AND_ADD_WORD(p,val) \
___EXT(___emulated_fetch_and_add_word)(p,val)

#define ___FETCH_AND_CLEAR_WORD(p) \
___EXT(___emulated_fetch_and_clear_word)(p)

#endif


#ifdef ___USE_sequential_sync

#define ___COMPARE_AND_SWAP_WORD(p,oldval,newval) \
((___ps->temp1 = *(p)) == (oldval) && (*(p) = (newval)), ___ps->temp1)

#define ___FETCH_AND_ADD_WORD(p,val) \
(___ps->temp1 = *(p), *(p) += (val), ___ps->temp1)

#define ___FETCH_AND_CLEAR_WORD(p) \
(___ps->temp1 = *(p), *(p) = 0, ___ps->temp1)

#endif

#ifdef ___CPU_x86
#ifdef __GNUC__
#define ___CPU_RELAX() __asm__ __volatile__ ("rep nop")
#endif
#endif

#ifndef ___CPU_RELAX
#define ___CPU_RELAX()
#endif

typedef struct ___thread_struct
  {
    void (*start_fn) ___P((struct ___thread_struct *self),());

    void *data_ptr;
    ___SCMOBJ data_scmobj;

#ifdef ___USE_POSIX_THREAD_SYSTEM

    pthread_t thread_id;

#endif

#ifdef ___USE_WIN32_THREAD_SYSTEM

    HANDLE thread_handle;
    DWORD thread_id;

#endif
  } ___thread;

#endif
