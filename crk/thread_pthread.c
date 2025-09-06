#include "thread_internal.h"

#include <stdlib.h>
#include "mem.h"

// CRK_Thread

CRK_Thread *CRK_ThreadCreate(CRK_ThreadFunc func, void *args)
{
	CRK_Thread *thread = CRK_Malloc(sizeof *thread);
	pthread_create(&thread->posixThread, NULL, func, args);
	return thread;
}

void CRK_ThreadJoin(CRK_Thread *thread, void **ret)
{
	pthread_join(thread->posixThread, ret);
	CRK_Free(thread);
}

void CRKCALL CRK_ThreadDetach(CRK_Thread *thread)
{
	pthread_detach(thread->posixThread);
	CRK_Free(thread);
}

// CRK_Mutex

CRK_Mutex *CRK_MutexCreate()
{
	CRK_Mutex *mtx = CRK_Malloc(sizeof *mtx);
	pthread_mutex_init(&mtx->posixMutex, NULL);
	return mtx;
}

void CRK_MutexDestroy(CRK_Mutex *mtx)
{
	pthread_mutex_destroy(&mtx->posixMutex);
	CRK_Free(mtx);
}

void CRK_MutexLock(CRK_Mutex *mtx)
{
	pthread_mutex_lock(&mtx->posixMutex);
}

void CRK_MutexUnlock(CRK_Mutex *mtx)
{
	pthread_mutex_unlock(&mtx->posixMutex);
}

// CRK_CondVar

CRK_CondVar *CRK_CondVarCreate()
{
	CRK_CondVar *cv = CRK_Malloc(sizeof *cv);
	pthread_cond_init(&cv->posixCond, NULL);
	return cv;
}

void CRK_CondVarDestroy(CRK_CondVar *cv)
{
	pthread_cond_destroy(&cv->posixCond);
	CRK_Free(cv);
}

void CRK_CondVarWait(CRK_CondVar *cv, CRK_Mutex *mtx)
{
	pthread_cond_wait(&cv->posixCond, &mtx->posixMutex);
}

void CRK_CondVarSignal(CRK_CondVar *cv)
{
	pthread_cond_signal(&cv->posixCond);
}

void CRK_CondVarBroadcast(CRK_CondVar *cv)
{
	pthread_cond_broadcast(&cv->posixCond);
}