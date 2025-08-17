#ifndef CRK_THREAD_INTERNAL_H
#define CRK_THREAD_INTERNAL_H

#include "thread.h"
#include <pthread.h>

typedef struct CRK_Thread
{
	const char *error;
	pthread_t posixThread;
} CRK_Thread;

typedef struct CRK_Mutex
{
	pthread_mutex_t posixMutex;
} CRK_Mutex;

typedef struct CRK_CondVar
{
	pthread_cond_t posixCond;
} CRK_CondVar;

#endif