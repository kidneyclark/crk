#ifndef CRK_COM_DEFS_H
#define CRK_COM_DEFS_H

#define COM_CONTEXT_DECL(x)                                                    \
	namespace crk::x                                                           \
	{                                                                          \
	using Context = void *;                                                    \
	Context CreateContext();                                                   \
	void DestroyContext();                                                     \
	void SetContext(Context ctx);                                              \
	Context GetContext();                                                      \
	}

#define COM_CONTEXT_IMPL(x)                                                    \
	static _impl_##x##_Context *g_CurrentContext = nullptr;                    \
	crk::x::Context crk::x::CreateContext()                                    \
	{                                                                          \
		if (g_CurrentContext != nullptr)                                       \
			return g_CurrentContext;                                           \
		g_CurrentContext = new _impl_##x##_Context;                            \
		return (void *)g_CurrentContext;                                       \
	}                                                                          \
	void crk::x::DestroyContext()                                              \
	{                                                                          \
		delete g_CurrentContext;                                               \
	}                                                                          \
	void crk::x::SetContext(crk::x::Context ctx)                               \
	{                                                                          \
		g_CurrentContext = reinterpret_cast<_impl_##x##_Context *>(ctx);       \
	}                                                                          \
	crk::x::Context crk::x::GetContext()                                       \
	{                                                                          \
		return reinterpret_cast<void *>(g_CurrentContext);                     \
	}

// #define COM_4CHAR_TAG(a, b, c, d) ((a << 24) | (b << 16) | (c << 8) | (d <<
// 0))

#include "types.h"
#include <utility>
namespace crk::com
{

template <size_t N> consteval u64 TagFromString(const char (&string)[N])
{
	static_assert((N <= 9) && "String must have no more than 8 characters");
	// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function#FNV_hash_parameters
	constexpr u64 fnv_prime = 1099511628211ull;
	constexpr u64 fnv_offset_basis = 14695981039346656037ull;
	// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function#FNV-1a_hash
	u64 hash = fnv_offset_basis;
	[&]<size_t... p>(std::index_sequence<p...>) {
		((hash = hash ^ static_cast<u64>(string[p]), hash = hash * fnv_prime),
		 ...);
	}(std::make_index_sequence<N - 1>{});
	return hash;
}

} // namespace crk::com

#define COM_REGISTER_CRK(x) mem_RegisterTag((crk::x##_tag), "crk::" #x);

#endif