#ifndef CRK_THREAD_H
#define CRK_THREAD_H

#include "com.h"

// thread handle
typedef struct CRK_Thread CRK_Thread;

typedef void*(*CRK_ThreadFunc)(void*);

// starts thread and returns handle.
extern CRKDECLSPEC CRK_Thread *CRKCALL CRK_ThreadCreate(CRK_ThreadFunc func, void *args);
// join thread and store retval in *ret.
// return true on success.
extern CRKDECLSPEC void CRKCALL CRK_ThreadJoin(CRK_Thread *thread, void **ret);
// detach thread.
// return true on success.
extern CRKDECLSPEC void CRKCALL CRK_ThreadDetach(CRK_Thread *thread);

// mutex handle
typedef struct CRK_Mutex CRK_Mutex;

// create mutex.
extern CRKDECLSPEC CRK_Mutex *CRKCALL CRK_MutexCreate();
// destroy mutex.
extern CRKDECLSPEC void CRKCALL CRK_MutexDestroy(CRK_Mutex *mtx);
// lock mutex.
extern CRKDECLSPEC void CRKCALL CRK_MutexLock(CRK_Mutex *mtx);
// unlock mutex.
extern CRKDECLSPEC void CRKCALL CRK_MutexUnlock(CRK_Mutex *mtx);


// condition variable handle
typedef struct CRK_CondVar CRK_CondVar;

// create condition variable.
extern CRKDECLSPEC CRK_CondVar *CRKCALL CRK_CondVarCreate();
// destroy condition variable.
extern CRKDECLSPEC void CRKCALL CRK_CondVarDestroy(CRK_CondVar *cv);
// make thread wait and unlocks mutex.
// waits until signal or broadcast.
extern CRKDECLSPEC void CRKCALL CRK_CondVarWait(CRK_CondVar *cv, CRK_Mutex *mtx);
// signal ONE thread waiting on the condition variable.
extern CRKDECLSPEC void CRKCALL CRK_CondVarSignal(CRK_CondVar *cv);
// signal ALL threads waiting on the condition variable.
extern CRKDECLSPEC void CRKCALL CRK_CondVarBroadcast(CRK_CondVar *cv);

// maybe add later:
// thread cancel/kill
// thread get itself


#endif