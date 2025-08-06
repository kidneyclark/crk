#ifndef CRK_GFX_H
#define CRK_GFX_H

#include "types.hh"

namespace crk::gfx
{

ModelData ModelDataCreateFromGLTF(const void* stream, size_t stream_size);
void RendererCreate(IBaseRenderer **renderer, RendererSettings *settings);
void RendererRelease(IBaseRenderer **renderer);

}

#endif