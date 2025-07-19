#ifndef CRK_GFX_IMPL_OPENGL_RENDERER_H
#define CRK_GFX_IMPL_OPENGL_RENDERER_H

#include "../../types.hh"

#include <glad/glad.h>

#include <array>
#include <cstddef>
#include <iostream>
#include <tuple>
#include <utility>

using namespace crk::gfx;

template <auto &&_fn, size_t _tupleSize, typename _tupleType>
inline constexpr void fnTupleUnpack(_tupleType &&tuple)
{
	// epic compile time unpacking
	[&]<size_t... _P>(std::index_sequence<_P...>) {
		_fn(std::get<_P>(std::forward<_tupleType>(tuple))...);
	}(std::make_index_sequence<_tupleSize>{});
}

class OpenGLRenderer : public IBaseRenderer
{
	Color cachedClearColor = {0, 0, 0, 0};
	f32 cachedClearDepth = 1;
	i32 cachedClearStencil = 0;

public:
	OpenGLRenderer(RendererSettings *settings)
	{
		gladLoadGLLoader((GLADloadproc)settings->backendLoadFunc);
	}

	void Clear(ClearMask mask)
	{
		i32 maskGL = 0;
		if (mask & ClearMask::ClearColor)
			maskGL |= GL_COLOR_BUFFER_BIT;
		if (mask & ClearMask::ClearDepth)
			maskGL |= GL_DEPTH_BUFFER_BIT;
		if (mask & ClearMask::ClearStencil)
			maskGL |= GL_STENCIL_BUFFER_BIT;
		glClear(maskGL);
	}

	void SetClearColor(Color color)
	{
		if (color != cachedClearColor)
		{
			cachedClearColor = color;
			fnTupleUnpack<glClearColor, 4>(color);
		}
	}

	void SetClearDepth(f32 depth)
	{
		if (depth != cachedClearDepth)
		{
			cachedClearDepth = depth;
			glClearDepthf(depth);
		}
	}

	void SetClearStencil(i32 stencil)
	{
		if (stencil != cachedClearStencil)
		{
			cachedClearStencil = stencil;
			glClearStencil(stencil);
		}
	}
};

#endif