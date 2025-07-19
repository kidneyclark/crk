#ifndef CRK_STRING_H
#define CRK_STRING_H

#include "../com/defs.h"
#include "../mem/allocator.h"
#include <string>

namespace crk
{

using string_type =
    std::basic_string<char, std::char_traits<char>, mem::Allocator<char>>;

class string : public string_type
{
	static constexpr u64 generic_tag = crk::com::TagFromString("crkstr");
public:
	explicit string(u64 tag = generic_tag)
	    : crk::string_type(mem::Allocator<char>(tag))
	{
		if (tag == generic_tag && !crk::mem::IsTagRegistered(tag))
			crk::mem::RegisterTag(tag, "crk::string");
	}
};

} // namespace crk

#endif