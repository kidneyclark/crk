#ifndef CRK_GFX_H
#define CRK_GFX_H

#include "types.hh"

namespace crk::gfx
{

void RendererCreate(IBaseRenderer **renderer, RendererSettings *settings);
void RendererRelease(IBaseRenderer **renderer);

}

#endif