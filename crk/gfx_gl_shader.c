#include "gfx_gl_internal.h"

#include "res.h"
#include <assert.h>

CRK_Shader *CRK_ShaderCreate(const char *path)
{
	CRK_File *file = CRK_GetResourceFile(path);
	assert(file != NULL && "Invalid Resource File!");
	const char *ext = CRK_GetPathExtension(path);
	assert(strcmp(file != NULL && "Invalid Resource File!");
	CRK_MemStream *fileData = CRK_FileDumpAsMemStream(file);
	CRK_FileClose(file);



	CRK_MemStreamDestroy(fileData);
}

void CRK_ShaderDestroy(CRK_Shader *shader)
{

}