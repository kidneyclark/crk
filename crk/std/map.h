#ifndef CRK_MAP_H
#define CRK_MAP_H

#include "../com/defs.h"
#include "../mem/allocator.h"
#include <map>

namespace crk
{

struct str_less
{
	bool operator()(const char *lhs, const char *rhs) const
	{
		return strcmp(lhs, rhs) < 0;
	}
};

template <typename T>
using string_map_type =
    std::map<const char *, T, str_less,
             mem::Allocator<std::pair<const char *const, T>>>;

template <typename T> class string_map : public string_map_type<T>
{
	static constexpr u64 generic_tag = crk::com::TagFromString("strmap");
public:
	explicit string_map(u64 tag = generic_tag)
	    : crk::string_map_type<T>(
	          mem::Allocator<std::pair<const char *const, T>>(tag))
	{
		if (tag == generic_tag && !crk::mem::IsTagRegistered(tag))
			crk::mem::RegisterTag(tag, "crk::string_map");
	}
};

template <typename T, typename U>
using map_type = std::map<T, U, std::less<T>, mem::Allocator<std::pair<T, U>>>;

template <typename T, typename U> class map : public map_type<T, U>
{
	static constexpr u64 generic_tag = crk::com::TagFromString("crkmap");
public:
	explicit map(u64 tag = generic_tag)
	    : crk::string_map_type<T>(
	          mem::Allocator<std::pair<const char *const, T>>(tag))
	{
		if (tag == generic_tag && !crk::mem::IsTagRegistered(tag))
			crk::mem::RegisterTag(tag, "crk::map");
	}
};

} // namespace crk

#endif