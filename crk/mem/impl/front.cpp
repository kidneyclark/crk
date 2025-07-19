#include "../front.h"


#include <assert.h>
#include <list>
#include <map>
#include <format>

struct _impl_mem_Chunk
{
	void *data;
	u64 size;
	u64 tag;
};

struct _impl_mem_Context
{
	std::list<_impl_mem_Chunk> chunkList;
	std::map<u64, const char *> tagMap;
};

COM_CONTEXT_IMPL(mem);

namespace crk::mem
{

Chunk AllocChunk(u64 size, u64 tag)
{
	assert((tag != 0) && "0 is an invalid tag!");
	::crk::mem::Context a;
	assert(size > 0);

	_impl_mem_Chunk chunk = {
	    .data = new u8[size],
	    .size = size,
	    .tag = tag,
	};
	g_CurrentContext->chunkList.push_back(chunk);

	return {.data = chunk.data, .size = chunk.size};
}

void FreeChunk(Chunk chunk, u64 tag)
{
	if (chunk.data == nullptr) return;
	auto it = g_CurrentContext->chunkList.begin();
	for (; it != g_CurrentContext->chunkList.end(); it++)
	{
		if (it->data != chunk.data)
			continue;

		// assert(it->size == chunk.size);
		if (it->tag != tag)
		{
			auto tagName = g_CurrentContext->tagMap.find(tag);
			if (tagName == g_CurrentContext->tagMap.end())
				assert(false && "Not valid memory chunk tag!");
			else
				assert(false && tagName->second);
		}

		delete[] ((u8 *)it->data);

		g_CurrentContext->chunkList.erase(it);

		return;
	}
}

Chunk ReallocChunk(Chunk chunk, u64 size, u64 tag)
{
	Chunk new_chunk = AllocChunk(size, tag);
	memcpy(new_chunk.data, chunk.data, new_chunk.size < chunk.size ? new_chunk.size : chunk.size);
	FreeChunk(chunk, tag);
	return new_chunk;
}

void Dump(std::ostream &out)
{
	int count = 1;
	out << "Dumping memory module!\n";
	if (g_CurrentContext->chunkList.empty())
		goto L_Empty;
	out << std::format("Total chunks left behind: {}\n", g_CurrentContext->chunkList.size());
	out << "Chunk info: \n";
	for (auto &l : g_CurrentContext->chunkList)
	{
		auto tagName = g_CurrentContext->tagMap.find(l.tag);
		if (tagName != g_CurrentContext->tagMap.end())
			out << std::format("\t#{}: ({}, {}) : {}\n", count++,
			        l.data, l.size, tagName->second);
		else
			out << std::format("\t#{}: ({}, {}) : {}\n", count++,
			        l.data, l.size, l.tag);
	}
	goto L_End;
L_Empty:
	out << "Nothing to dump!\n";
L_End:
	out.flush();
}

void RegisterTag(u64 tag, const char *name)
{
	assert(name != nullptr);
	auto tagName = g_CurrentContext->tagMap.find(tag);
	assert((tagName == g_CurrentContext->tagMap.end()) &&
	       "Already existing tag");
	g_CurrentContext->tagMap[tag] = name;
}

bool IsTagRegistered(u64 tag)
{
	auto tagName = g_CurrentContext->tagMap.find(tag);
	return (tagName != g_CurrentContext->tagMap.end());
}

}