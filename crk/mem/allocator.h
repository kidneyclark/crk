#ifndef CRK_MEM_ALLOCATOR_H
#define CRK_MEM_ALLOCATOR_H

#include "../com/types.h"
#include "front.h"
#include <cstddef>

namespace crk::mem
{
// std compatible allocator
template <typename T> class Allocator
{
	u64 tag {0};
public:
	typedef T value_type;
	typedef value_type *pointer;
	typedef const value_type *const_pointer;
	typedef value_type &reference;
	typedef const value_type &const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	template <class U> struct rebind
	{
		typedef Allocator<U> other;
	};

	Allocator(Allocator<T> const &) noexcept
	{
	}
	template <class U> Allocator(Allocator<U> const &) noexcept
	{
	}
	~Allocator() = default;

	Allocator(u64 _tag) noexcept
		: tag(_tag)
	{
		if (crk::mem::GetContext() == nullptr)
			crk::mem::CreateContext();
	}

	value_type *allocate(std::size_t n)
	{
		// printf("mem_Allocate(%llu)\n", n * sizeof(value_type));
		return reinterpret_cast<value_type *>(
		    AllocChunk(n * sizeof(value_type), tag).data);
	}

	void deallocate(Allocator<T>::value_type *p, std::size_t n) noexcept
	{
		// printf("mem_Deallocate\n");
		FreeChunk((Chunk){p, n}, tag);
	}
	bool operator==(Allocator<T> const &a)
	{
		return this == &a;
	}
	bool operator!=(Allocator<T> const &a)
	{
		return !operator==(a);
	}
};
} // namespace crk::mem

#endif