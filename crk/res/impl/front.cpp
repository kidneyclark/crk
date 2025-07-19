#include "../front.h"

#include "../../io/front.h"
#include "../../std/map.h"
#include "../../util/tabula.h"
#include <optional>
#include <cassert>

using namespace crk::res;

struct _impl_res_Reg
{
	crk::string type {};
	crk::string path {};
	size_t count {0};
	std::optional<Resource> resource {std::nullopt};
};

struct _impl_res_Context
{
	static constexpr u64 tag = crk::com::TagFromString("res_ctx");

	_impl_res_Context()
	    : resources(tag)
	{
		if (!crk::mem::IsTagRegistered(tag))
			crk::mem::RegisterTag(tag, "Resource Context");
	}

	crk::string_map<_impl_res_Reg> resources;
};

COM_CONTEXT_IMPL(res)

crk::Lifetime<Resource> LoadResource(const char *name)
{
	CreateContext();

	auto &ctx = g_CurrentContext;

	auto reg = ctx->resources.find(name);
	assert((reg == ctx->resources.end()) && "Non existant resource");	

	reg->second.count++;
	return crk::Lifetime<Resource>(&reg->second.resource, []{});
}