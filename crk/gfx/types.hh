#ifndef CRK_GFX_TYPES_H
#define CRK_GFX_TYPES_H

#include "../com/types.h"
#include <tuple>

namespace crk::gfx
{

using Color = std::tuple<f32, f32, f32, f32>;

enum ClearMask
{
	ClearNone = 0,
	ClearColor = 1,
	ClearDepth = 2,
	ClearStencil = 4,
};

struct RendererSettings
{
	void *backendLoadFunc {nullptr};
};

class IBaseRenderer
{
public:
	virtual ~IBaseRenderer() = default;
	virtual void Clear(ClearMask mask) = 0;
	virtual void SetClearColor(Color color) = 0;
	virtual void SetClearDepth(f32 depth) = 0;
	virtual void SetClearStencil(i32 stencil) = 0;
};

} // namespace crk::gfx

#endif