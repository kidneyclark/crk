#ifndef CRK_MEM_FRONT_H
#define CRK_MEM_FRONT_H

#include "types.h"
#include <cstdio>
#include <ostream>

#include "../com/defs.h"

COM_CONTEXT_DECL(mem)

namespace crk::mem
{

Chunk AllocChunk(u64 size, u64 tag);
void FreeChunk(Chunk chunk, u64 tag);
Chunk ReallocChunk(Chunk chunk, u64 size, u64 tag);

template <typename T, typename... Args>
T *Alloc(u64 tag, Args&&... args)
{
	auto chunk = AllocChunk(sizeof(T), tag);
	T *ret = new(chunk.data) T(std::forward<Args>(args)...);
	return ret;
}

template <typename T>
void Free(T *ptr, u64 tag)
{
	ptr->~T();
	FreeChunk(Chunk{.data = ptr, .size = sizeof(T)}, tag);
}

template <typename T>
Array<T> AllocArray(u64 count, u64 tag)
{
	Chunk chunk = AllocChunk(count * sizeof(T), tag);
	return Array<T>{.array = (T*)chunk.data, .count = count};
}

template <typename T>
void FreeArray(Array<T> array, u64 tag)
{
	Chunk chunk = {.data = array.array, .size = array.count * sizeof(T)};
	FreeChunk(chunk, tag);
}

template <typename T>
Array<T> ReallocArray(Array<T> array, u64 count, u64 tag)
{
	Chunk chunk = {.data = array.array, .size = array.count * sizeof(T)};
	chunk = ReallocChunk(chunk, count * sizeof(T), tag);
	return Array<T>{.array = (T*)chunk.data, .count = count};
}

void Dump(std::ostream &out);

bool IsTagRegistered(u64 tag);
void RegisterTag(u64 tag, const char *name);
}

#endif