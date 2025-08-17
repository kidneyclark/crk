#ifndef CRK_GFX_INTERNAL_H
#define CRK_GFX_INTERNAL_H

#include "gfx.h"

#include <glad/glad.h>	


struct GLUniform
{
	int index;
	int size;
	GLenum type;
	const char *name;
};

struct GLAttribute
{
	int index;
	int size;
	GLenum type;
	const char *name;
};

typedef struct CRK_Shader
{
	const char *name;
	GLuint progID;
	bool linked;

} CRK_Shader;

#endif