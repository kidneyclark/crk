#include "tabula.h"

#include <string.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct TabulaBlock
{
	struct TabulaBlock *next;
	u64 head;
	char block[1];
} TabulaBlock;

typedef struct TabulaFindNode
{
	const char *str;
	// -1 is null
	i64 left;
	// -1 is null
	i64 right;
} TabulaFindNode;

typedef struct CRK_Tabula
{
	TabulaBlock *blockList;
	TabulaFindNode *findTree;
	u64 findSize;
	u64 findCapacity;
	u64 blockSize;
} CRK_Tabula;

CRK_Tabula *CRK_TabulaCreate(u32 blockSize)
{
	CRK_Tabula *tab;
	tab = malloc(sizeof *tab);
	tab->blockList = NULL;
	tab->findTree = NULL;
	tab->findSize = 0;
	tab->findCapacity = 0;
	tab->blockSize = blockSize;
	return tab;
}

void CRK_TabulaDestroy(CRK_Tabula *tab)
{ 
	void *tmp;
	while (tab->blockList != NULL)
	{
		tmp = tab->blockList;
		tab->blockList = tab->blockList->next;
		free(tmp);
	}
	if (tab->findTree != NULL)
		free(tab->findTree);
	free(tab);
}

const char *CRK_TabulaFind(CRK_Tabula *tab, const char *str)
{
	TabulaFindNode *it;
	int i, next;
	it = tab->findTree;
	// it would be imposible to get more than
	// tab->findSize depth :)
	for (i = 0; it != NULL && i < tab->findSize; i++)
	{
		int cmp = strcmp(it->str, str);
		if (cmp == 0)
			return it->str;
		if (cmp < 0)
			next = it->left;
		else
			next = it->right;
		if (next == -1)
			break;
		it = &tab->findTree[next];
	}
	return NULL;
}

#include <assert.h>

const char *CRK_TabulaAdd(CRK_Tabula *tab, const char *str)
{
	TabulaBlock *block;
	TabulaFindNode *findNode;
	i32 strsize, i;
	char *ret;

	strsize = strlen(str);
	assert(strsize >= tab->blockSize && "string too big for tabula");
	if (tab->blockList == NULL || tab->blockSize - tab->blockList->head <= strsize)
	{
		block = malloc(offsetof(TabulaBlock, block) + tab->blockSize);
		block->next = tab->blockList;
		block->head = 0;
		tab->blockList = block;
	}
	block = tab->blockList;

	ret = block->block + block->head;
	memcpy(block->block + block->head, str, strsize + 1); 
	//block->block[block->head + strsize] = '\0';
	block->head += strsize + 1;

	// expand if needed
	assert (tab->findSize <= tab->findCapacity && "something weird happened");
	if (tab->findSize == tab->findCapacity)
	{
		tab->findTree = realloc(tab->findTree, tab->findSize * 2);
		tab->findCapacity = tab->findSize * 2;
	}

	// add to find tree
	findNode = tab->findTree;
	i64 *next;
	for (i = 0; findNode != NULL && i < tab->findSize; i++)
	{
		int cmp = strcmp(findNode->str, str);
		if (cmp == 0)
			//assert (false && "string already exists in tabula smh");
			return ret;
		if (cmp < 0)
			next = &findNode->left;
		else
			next = &findNode->right;
		if (*next == -1)
		{
			*next = tab->findSize;
			break;
		}
		findNode = &tab->findTree[*next];
	}
	assert (i != tab->findSize && "tabula tree is bugged or smth");

	findNode = &tab->findTree[tab->findSize];
	findNode->str = ret;
	findNode->left = -1;
	findNode->right = -1;
	tab->findSize++;

	return ret;
}

static CRK_Tabula *globalTabula = NULL;

CRK_Tabula *CRK_TabulaGetGlobal()
{
	if (globalTabula == NULL)
		globalTabula = CRK_TabulaCreate(1 << 16);
	return globalTabula;
}