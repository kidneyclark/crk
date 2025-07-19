#ifndef CRK_TYPES_H
#define CRK_TYPES_H

#include "../std/string.h"

namespace crk::res
{

struct Resource
{
	void *data;

	template <typename T> T &As() const
	{
		return *reinterpret_cast<T *>(data);
	}
};

} // namespace crk::res

#endif