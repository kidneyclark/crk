#include "io_internal.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct CRK_DiskFile
{
	CRK_File head;
	const char *name;
	CRK_FileAccess access;
	FILE *file;
} CRK_DiskFile;

u64 disk_fileRead(CRK_File *file, void *dst, u64 size, u64 count)
{
	CRK_DiskFile *diskFile = (CRK_DiskFile *)file;
	return fread(dst, size, count, diskFile->file);
}

u64 disk_fileWrite(CRK_File *file, const void *src, u64 size, u64 count)
{
	CRK_DiskFile *diskFile = (CRK_DiskFile *)file;
	return fwrite(src, size, count, diskFile->file);
}

i32 disk_fileSeek(CRK_File *file, i64 offset, CRK_FileSeekPos origin)
{
	CRK_DiskFile *diskFile = (CRK_DiskFile *)file;
	int stdorigin = 0;
	switch (origin)
	{
	case CRK_FileSeekPos_SET:
		stdorigin = SEEK_SET;
		break;
	case CRK_FileSeekPos_END:
		stdorigin = SEEK_END;
		break;
	case CRK_FileSeekPos_CUR:
		stdorigin = SEEK_CUR;
		break;
	default:
		return -1;
	}
	return fseek(diskFile->file, offset, stdorigin);
}

i64 disk_fileTell(CRK_File *file)
{
	CRK_DiskFile *diskFile = (CRK_DiskFile *)file;
	return ftell(diskFile->file);
}

i32 disk_fileClose(CRK_File *file)
{
	CRK_DiskFile *diskFile = (CRK_DiskFile *)file;
	return fclose(diskFile->file);
}

// posix stuff
#include <sys/stat.h>

u64 disk_fileSize(CRK_File *file)
{
	CRK_DiskFile *diskFile = (CRK_DiskFile *)file;
	struct stat s;
	if (stat(diskFile->name, &s) == 0)
		return s.st_size;
	return 0;
}

static CRK_FileVtable diskVtable = {disk_fileRead,  disk_fileWrite,
                                    disk_fileSeek,  disk_fileTell,
                                    disk_fileClose, disk_fileSize};

#define CRK_DISKFILE_TYPE 2025110801ull

CRK_File *CRK_FileOpen(const char *path, CRK_FileAccess access)
{
	CRK_DiskFile *file = malloc(sizeof *file);
	file->head.type = CRK_DISKFILE_TYPE;
	file->head.vtable = &diskVtable;
	file->name = path;
	file->access = access;
	return (CRK_File *)file;
}

u64 CRK_FileRead(CRK_File *file, void *dst, u64 size, u64 count)
{
	return file->vtable->fileRead(file, dst, size, count);
}

u64 CRK_FileWrite(CRK_File *file, const void *src, u64 size, u64 count)
{
	return file->vtable->fileWrite(file, src, size, count);
}

i32 CRK_FileSeek(CRK_File *file, i64 offset, CRK_FileSeekPos origin)
{
	return file->vtable->fileSeek(file, offset, origin);
}

i64 CRK_FileTell(CRK_File *file)
{
	return file->vtable->fileTell(file);
}

i32 CRK_FileClose(CRK_File *file)
{
	i32 ret;
	ret = file->vtable->fileClose(file);
	free(file);
	return ret;
}

u64 CRK_FileGetSize(CRK_File *file)
{
	return file->vtable->fileSize(file);
}