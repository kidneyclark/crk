#include "io_internal.h"

#include "memstream.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct CRK_MemStreamFile
{
	CRK_File head;
	CRK_MemStream *stream;
	bool flexible;
	i64 pos;
} CRK_MemStreamFile;


u64 mem_fileRead(CRK_File *file, void *dst, u64 size, u64 count)
{
	CRK_MemStreamFile *memFile = (CRK_MemStreamFile*)file;
	u8 *offdata;
	assert((memFile->pos + size*count <= memFile->stream->size) && "MemStreamFile Access Violation!");
	offdata = ((u8*)memFile->stream->data) + memFile->pos;
	memcpy(dst, offdata, size*count);
	return count;
}

u64 mem_fileWrite(CRK_File *file, const void *src, u64 size, u64 count)
{
	CRK_MemStreamFile *memFile = (CRK_MemStreamFile*)file;
	u8 *offdata;
	if (memFile->pos + size*count > memFile->stream->size)
	{
		assert((memFile->flexible) && "MemStreamFile Overflow!");
		CRK_MemStreamResize(memFile->stream, memFile->pos + size*count);
	}
	offdata = ((u8*)memFile->stream->data) + memFile->pos;
	memcpy(offdata, src, size*count);
	return count;
}

i32 mem_fileSeek(CRK_File *file, i64 offset, CRK_FileSeekPos origin)
{
	CRK_MemStreamFile *memFile = (CRK_MemStreamFile*)file;
	switch (origin)
	{
	case CRK_FileSeekPos_SET:
		memFile->pos = origin;
		break;
	case CRK_FileSeekPos_END:
		memFile->pos = memFile->stream->size - origin;
		break;
	case CRK_FileSeekPos_CUR:
		memFile->pos += origin;
		break;
	default:
		return -1;
	}
	assert((memFile->pos >= 0) && "Invalid seek position!");
	return 0;
}

i64 mem_fileTell(CRK_File *file)
{
	CRK_MemStreamFile *memFile = (CRK_MemStreamFile*)file;
	return memFile->pos;
}

i32 mem_fileClose(CRK_File *file)
{
	CRK_MemStreamFile *memFile = (CRK_MemStreamFile*)file;
	CRK_MemStreamDestroy(memFile->stream);
	memFile->stream = NULL;
	return 0;
}

u64 mem_fileSize(CRK_File *file)
{
	CRK_MemStreamFile *memFile = (CRK_MemStreamFile*)file;
	return memFile->stream->size;
}

static CRK_FileVtable memVtable = 
{
	mem_fileRead,
	mem_fileWrite,
	mem_fileSeek,
	mem_fileTell,
	mem_fileClose,
	mem_fileSize
};

#define CRK_MEMSTREAMFILE_TYPE 2025110801ull

CRK_File *CRK_FileAsMemStreamCreate(u64 size, bool flexible)
{
	CRK_MemStreamFile *file = malloc(sizeof *file);
	file->head.type = CRK_MEMSTREAMFILE_TYPE;
	file->head.vtable = &memVtable;
	file->pos = 0;
	file->flexible = flexible;
	file->stream = CRK_MemStreamCreate(size);
	return (CRK_File*)file;
}

CRK_File *CRK_FileAsMemStreamLink(CRK_MemStream *stream, bool flexible)
{
	CRK_MemStreamFile *file = malloc(sizeof *file);
	file->head.type = CRK_MEMSTREAMFILE_TYPE;
	file->head.vtable = &memVtable;
	file->pos = 0;
	file->flexible = flexible;
	file->stream = stream;
	return (CRK_File*)file;
}

CRK_MemStream *CRK_FileAsMemStreamUnlink(CRK_File *file)
{
	CRK_MemStream *stream;
	CRK_MemStreamFile *memFile;
	assert((file->type == CRK_MEMSTREAMFILE_TYPE) && "Not a MemStreamFile.");
	memFile = (CRK_MemStreamFile*)file;
	stream = memFile->stream;
	free(file); // because were not calling CRK_FileClose.
	return stream;
}

const CRK_MemStream *CRK_FileAsMemStreamGet(CRK_File *file)
{
	CRK_MemStreamFile *memFile;
	assert((file->type == CRK_MEMSTREAMFILE_TYPE) && "Not a MemStreamFile.");
	memFile = (CRK_MemStreamFile*)file;
	return memFile->stream;
}

CRK_MemStream *CRK_FileDumpAsMemStream(CRK_File *file)
{
	u64 size, tell;
	CRK_MemStream *stream;
	assert(file != NULL && "Dumping NULL file!");

	size = CRK_FileGetSize(file);
	stream = CRK_MemStreamCreate(size);
	tell = CRK_FileTell(file);
	CRK_FileSeek(file, 0, CRK_FileSeekPos_SET); // at the start
	size = CRK_FileRead(file, stream->data, 1, size);
	CRK_FileSeek(file, tell, CRK_FileSeekPos_SET); // restore pos
	stream = CRK_MemStreamResize(stream, size); // shrink to read data size

	return stream;
}