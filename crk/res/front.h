#ifndef CRK_RES_H
#define CRK_RES_H

#include "../com/defs.h"
#include "../std/string.h"
#include "../util/refcounted.h"
#include "types.h"

COM_CONTEXT_DECL(res)

namespace crk::res
{

template <typename T> RefCounted<T> &&LoadResource(const char *name)
{
}

} // namespace crk::res

#endif