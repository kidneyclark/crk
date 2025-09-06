#ifndef CRK_MEM_H
#define CRK_MEM_H

#include "com.h"
/*
extern CRKDECLSPEC void *CRKCALL CRK_MallocInternal(size_t size, const char *file, const char *line);
extern CRKDECLSPEC void *CRKCALL CRK_ReallocInternal(void *ptr, size_t size, const char *file, const char *line);
extern CRKDECLSPEC void CRKCALL CRK_FreeInternal(void *ptr, const char *file, const char *line);*/

//#define CRK_MEM_DEBUG
#ifdef CRK_MEM_DEBUG
	#define CRK_Malloc(x) CRK_MallocInteral((x), __FILE__, __LINE__)
	#define CRK_Free(ptr) CRK_FreeInternal((ptr), __FILE__, __LINE__)
	#define CRK_Realloc(ptr, x) CRK_ReallocInteral((ptr), (x), __FILE__, __LINE__)
#else
	#include <stdlib.h>
	#define CRK_Malloc(x) malloc((x))
	#define CRK_Free(ptr) free((ptr))
	#define CRK_Realloc(ptr, x) realloc((ptr), (x));
#endif

#endif