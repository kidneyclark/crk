#include "../front.hh"

// opengl
#include "opengl/renderer.hh"

namespace crk::gfx
{


void RendererCreate(IBaseRenderer **renderer, RendererSettings *settings)
{
	*renderer = new OpenGLRenderer(settings);
}

void RendererRelease(IBaseRenderer **renderer)
{
	delete *renderer;
}

} // namespace crk::gfx