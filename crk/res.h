#ifndef CRK_RES_H
#define CRK_RES_H

#include "com.h"

#include "io.h"

typedef enum CRK_ResourceType
{
	CRK_ResourceType_NONE = 0,
	CRK_ResourceType_CUSTOM,
	CRK_ResourceType_CONFLICT,
	CRK_ResourceType_DICT,
	CRK_ResourceType_MAX,
} CRK_ResourceType;

typedef struct CRK_Resource
{
	CRK_File *file;
	const char *name;
	const char *extension;
	CRK_ResourceType type;
} CRK_Resource;

// adding resources

// adds all subfiles in subdirectory .
// higher priority names override ones with lower priorities.
extern CRKDECLSPEC void CRKCALL CRK_AddWorkingDirectory(const char *path, int priority);
// add files inside Compact Resource File (CRF).
// higher priority names override ones with lower priorities.
extern CRKDECLSPEC void CRKCALL CRK_AddCRF(const char *path, int priority);

#include "io.h"
// higher priority names override ones with lower priorities.
extern CRKDECLSPEC void CRKCALL CRK_ResourceGetFile(const char *name, CRK_Resource *resource);

// image type

typedef struct CRK_Image
{
	u8 *data;
	u16 components;
} CRK_Image;

extern CRKDECLSPEC void CRKCALL CRK_ResourceToImage(CRK_Resource *resource, CRK_Image *image);
extern CRKDECLSPEC void CRKCALL CRK_ResourceImageDestroy(CRK_Image *image);

// dict type
#include "dict.h"

extern CRKDECLSPEC void CRKCALL CRK_ResourceToImage(CRK_Resource *resource, CRK_Image *image);
extern CRKDECLSPEC void CRKCALL CRK_ResourceImageDestroy(CRK_Image *image);

#endif