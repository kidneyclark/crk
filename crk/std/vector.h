#ifndef CRK_VECTOR_H
#define CRK_VECTOR_H

#include "../com/defs.h"
#include "../mem/allocator.h"
#include <vector>

namespace crk
{

template <typename T>
class vector : public std::vector<T>
{
	static constexpr u64 generic_tag = crk::com::TagFromString("vector");
public:
	explicit vector(u64 tag = generic_tag)
	    : std::vector<T>(mem::Allocator<T>(tag))
	{
		if (tag == generic_tag && !crk::mem::IsTagRegistered(tag))
			crk::mem::RegisterTag(tag, "crk::vector");
	}
};

} // namespace crk

#endif