#include "memstream.h"

typedef struct CRK_MemStreamExt
{
	CRK_MemStream head;
	CRK_AllocatorVtable *allocator;
} CRK_MemStreamExt;

#define _MEMSTREAM_PAD 0xF
#include <stdlib.h>

CRK_MemStream *CRK_MemStreamCreate(u64 size)
{
	CRK_MemStreamExt *stream;
	u64 totalSize;
	CRK_AllocatorVtable *allocator; 

	// custom allocator
	allocator = NULL;

	totalSize = size + ((sizeof *stream + _MEMSTREAM_PAD) & ~_MEMSTREAM_PAD);
	if (allocator == NULL)
		stream = malloc(totalSize);
	else
		stream = allocator->malloc(totalSize);

	stream->head.size = size;
	stream->head.data = ((u8*)stream) + ((sizeof *stream + _MEMSTREAM_PAD) & ~_MEMSTREAM_PAD);
	stream->allocator = allocator;

	return (CRK_MemStream*)stream;
}

CRK_MemStream *CRK_MemStreamResize(CRK_MemStream *stream, u64 size)
{
	CRK_MemStreamExt *streamExt;
	CRK_AllocatorVtable *allocator;

	streamExt = (CRK_MemStreamExt*)stream;
	allocator = streamExt->allocator;
	if (allocator == NULL)
		streamExt = realloc(streamExt, size);
	else
		streamExt = allocator->realloc(streamExt, size);
	return (CRK_MemStream*)streamExt;
}

void CRK_MemStreamDestroy(CRK_MemStream *stream)
{
	CRK_MemStreamExt *streamExt;
	CRK_AllocatorVtable *allocator;

	streamExt = (CRK_MemStreamExt*)stream;
	allocator = streamExt->allocator;
	if (allocator == NULL)
		free(streamExt);
	else
		allocator->free(streamExt);
}