#ifndef CRK_IO_INTERNAL_H
#define CRK_IO_INTERNAL_H

#include "io.h"

typedef struct CRK_FileVtable
{
	u64 (*fileRead)(CRK_File *file, void *dst, u64 size, u64 count);
	// write file
	u64 (*fileWrite)(CRK_File *file, const void *src, u64 size, u64 count);
	// seek file pos
	i32 (*fileSeek)(CRK_File *file, i64 offset, CRK_FileSeekPos origin);
	// tell file pos
	i64 (*fileTell)(CRK_File *file);
	// close file; file store its corresponding device
	i32 (*fileClose)(CRK_File *file);
	// get file size
	u64 (*fileSize)(CRK_File *file);
} CRK_FileVtable;

typedef struct CRK_File
{
	CRK_FileVtable *vtable;
	u64 type;
} CRK_File;

#endif