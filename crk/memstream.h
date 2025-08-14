#ifndef CRK_MEMSTREAM_H
#define CRK_MEMSTREAM_H

#include "com.h"

typedef struct CRK_AllocatorVtable
{
	void *(*malloc)(u64 size);
	void *(*realloc)(void *, u64 size);
	void (*free)(void *);
} CRK_AllocatorVtable;

typedef struct CRK_MemStream
{
	void *data;
	u64 size;
} CRK_MemStream;

// create memory stream.
CRK_MemStream *CRK_MemStreamCreate(u64 size);
// resize memory stream.
// truncates data if new size is less than previous.
CRK_MemStream *CRK_MemStreamResize(CRK_MemStream *stream, u64 size);
// destroy memory stream.
void CRK_MemStreamDestroy(CRK_MemStream *stream);

#endif