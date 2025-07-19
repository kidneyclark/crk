#ifndef CRK_MAP_H
#define CRK_MAP_H

#include <map>

namespace std
{

struct str_less
{
	bool operator()(const char *lhs, const char *rhs) const
	{
		return strcmp(lhs, rhs) < 0;
	}
};

template <typename T> using string_map = map<const char *, T, str_less>;

} // namespace std

#endif