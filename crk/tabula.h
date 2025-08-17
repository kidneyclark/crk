#ifndef CRK_TABULA_H
#define CRK_TABULA_H

#include "com.h"

// dynamic string storage

typedef struct CRK_Tabula CRK_Tabula;

// the tabula reexpands in 'blockSize' bytes intervals.
// this keeps all pointers valid, theres no realloc, theres no need to worry.
extern CRKDECLSPEC CRK_Tabula *CRKCALL CRK_TabulaCreate(u32 blockSize);
// tabula rasa
extern CRKDECLSPEC void CRKCALL CRK_TabulaDestroy(CRK_Tabula *tab);
// find string and return ptr to it; NULL if not found.
// O(logN) because theres a binary tree behind this.
extern CRKDECLSPEC const char *CRKCALL CRK_TabulaFind(CRK_Tabula *tab, const char *str);
// returns NULL if there was a problem.
extern CRKDECLSPEC const char *CRKCALL CRK_TabulaAdd(CRK_Tabula *tab, const char *str);

extern CRKDECLSPEC CRK_Tabula *CRKCALL CRK_TabulaGetGlobal();

#endif