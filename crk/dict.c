#include "dict.h"

#include "tabula.h"
#include "mem.h"

#define DICT_TABULA_BLOCK_SIZE 512

typedef struct DictFindNode
{
	u32 pair;
	u32 left;
	u32 right;
} DictFindNode;

// memory is stored in the tabula.
typedef struct DictPair
{
	const char *key;
	const char *value;
	double fvalue;
} DictPair;

typedef struct CRK_Dict 
{
	CRK_Tabula *tabula;
	DictPair *pairs;
	u32 pairCount;
	DictFindNode *findTree;
} CRK_Dict;

CRK_Dict *CRK_DictCreate()
{
	CRK_Dict *dict = CRK_Malloc(sizeof *dict);
	dict->tabula = CRK_TabulaCreate(DICT_TABULA_BLOCK_SIZE);
	dict->pairCount = 0;
	dict->pairs = NULL;
	dict->findTree = NULL;
	return dict;
}

void CRK_DictDestroy(CRK_Dict *dict)
{
	if (dict->pairs != NULL)
		CRK_Free(dict->pairs);
	if (dict->findTree != NULL)
		CRK_Free(dict->findTree);
	CRK_TabulaDestroy(dict->tabula);
	CRK_Free(dict);
}

void AddPair(CRK_Dict *dict, DictPair *pair)
{
	int newIdx = dict->pairCount++;
	dict->pairs = CRK_Realloc(dict->pairs, dict->pairCount * sizeof(DictPair));
	dict->findTree = CRK_Realloc(dict->findTree, dict->pairCount * sizeof(DictPair));
	dict->pairs[newIdx] = *pair;
	dict->findTree[newIdx].pair = newIdx;
}

void CRK_DictAddString(CRK_Dict *dict, const char *key, const char *value)
{
	// does not add if already exists
	CRK_TabulaAdd(dict->tabula, key);
	CRK_TabulaAdd(dict->tabula, value);
}

void CRK_DictAddValue(CRK_Dict *dict, const char *key, double value);

void CRK_DictRemove(CRK_Dict *dict, const char *key)
{
	// not implemented
}

DictPair *GetPair(CRK_Dict *dict, const char *key)
{

}

const char *CRK_DictGetString(CRK_Dict *dict, const char *key)
{
	DictPair *pair = GetPair(dict, key);
	return pair->value;
}

double CRK_DictGetValue(CRK_Dict *dict, const char *key)
{
	DictPair *pair = GetPair(dict, key);
	return pair->fvalue;
}

bool CRK_DictIsString(CRK_Dict *dict, const char *key)
{
	return CRK_DictGetString(dict, key) != NULL;
}