#include "mem.h"

typedef struct AllocInfo
{
	void *ptr;
	size_t size;
	const char *file;
	const char *line;
} AllocInfo;

typedef struct AllocStorage
{
	AllocInfo *alloc;
	u64 allocCount;
	
} AllocStorage;

static AllocStorage storage;
#include "thread.h"
static CRK_Mutex *storageMutex;

void *CRK_MallocInternal(size_t size, const char *file, const char *line)
{

}
void *CRK_ReallocInternal(void *ptr, size_t size, const char *file, const char *line);
void CRK_FreeInternal(void *ptr, const char *file, const char *line);