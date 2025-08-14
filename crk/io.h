#ifndef CRK_IO_H
#define CRK_IO_H

#include "com.h"

// io file handle
typedef struct CRK_File CRK_File;

typedef enum CRK_FileAccess
{
	CRK_FileAccess_NONE = 0,
	CRK_FileAccess_READ = 1,
	CRK_FileAccess_WRITE = 2,
	CRK_FileAccess_BINARY = 4,
} CRK_FileAccess;

typedef enum CRK_FileSeekPos
{
	CRK_FileSeekPos_SET = 0,
	CRK_FileSeekPos_CUR,
	CRK_FileSeekPos_END,
} CRK_FileSeekPos;

// DISK FILE functions

// open disk file
extern CRKDECLSPEC CRK_File *CRKCALL CRK_FileOpen(const char *path,
                                                  CRK_FileAccess access);

#ifndef CRK_IO_NO_MEMSTREAM
// MEMSTREAM FILE functions
#include "memstream.h"

// create memory stream as file.
// flexible: reallocs memory if buffer overflows.
extern CRKDECLSPEC CRK_File *CRKCALL CRK_FileAsMemStreamCreate(u64 size,
                                                               bool flexible);

// link preexisting memstream to a file.
// you lose ownership of the memstream,
// now the file is the owner until its freed
// or UNLINKED.
extern CRKDECLSPEC CRK_File *CRKCALL
CRK_FileAsMemStreamLink(CRK_MemStream *stream, bool flexible);

// unlink memstream from file.
// file must have a memstream, if not
// it will return NULL.
// you regain ownership of the memstream,
// and the file is closed.
extern CRKDECLSPEC CRK_MemStream *CRKCALL
CRK_FileAsMemStreamUnlink(CRK_File *file);

// get a read-only view of the memstream data.
extern CRKDECLSPEC const CRK_MemStream *CRKCALL
CRK_FileAsMemStreamGet(CRK_File *file);
// dump file contents into a memstream.
// file is NOT closed after dump.
extern CRKDECLSPEC CRK_MemStream *CRKCALL
CRK_FileDumpAsMemStream(CRK_File *file);
#endif

// GENERIC FILE FUNCTIONS

// read file
extern CRKDECLSPEC u64 CRKCALL CRK_FileRead(CRK_File *file, void *dst, u64 size,
                                            u64 count);
// write file
extern CRKDECLSPEC u64 CRKCALL CRK_FileWrite(CRK_File *file, const void *src,
                                             u64 size, u64 count);
// seek file pos
extern CRKDECLSPEC i32 CRKCALL CRK_FileSeek(CRK_File *file, i64 offset,
                                            CRK_FileSeekPos origin);
// tell file pos
extern CRKDECLSPEC i64 CRKCALL CRK_FileTell(CRK_File *file);
// close file; file store its corresponding device
extern CRKDECLSPEC i32 CRKCALL CRK_FileClose(CRK_File *file);
// get file size
extern CRKDECLSPEC u64 CRKCALL CRK_FileGetSize(CRK_File *file);

#endif