#ifndef CRK_GFX_H
#define CRK_GFX_H

#include "com.h"


// shader
typedef struct CRK_Shader CRK_Shader;

extern CRKDECLSPEC CRK_Shader *CRKCALL CRK_ShaderCreate(const char *path);
extern CRKDECLSPEC void CRKCALL CRK_ShaderDestroy(CRK_Shader *shader);

// mesh
typedef struct CRK_Mesh CRK_Mesh;
// mesh general
extern CRKDECLSPEC void CRKCALL CRK_MeshRender();
// mesh creators
extern CRKDECLSPEC CRK_Mesh *CRKCALL CRK_Quad2DCreate();

#endif