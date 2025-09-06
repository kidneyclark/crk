#ifndef CRK_DICT_H
#define CRK_DICT_H

#include "com.h"

typedef struct CRK_Dict CRK_Dict;

extern CRKDECLSPEC CRK_Dict *CRKCALL CRK_DictCreate();
extern CRKDECLSPEC void CRKCALL CRK_DictDestroy(CRK_Dict *dict);

extern CRKDECLSPEC void CRKCALL CRK_DictAddString(CRK_Dict *dict, const char *key, const char *value);
extern CRKDECLSPEC void CRKCALL CRK_DictAddValue(CRK_Dict *dict, const char *key, double value);
extern CRKDECLSPEC void CRKCALL CRK_DictRemove(CRK_Dict *dict, const char *key);
// null if invalid or is a value.
extern CRKDECLSPEC const char *CRKCALL CRK_DictGetString(CRK_Dict *dict, const char *key);
// null if invalid or is a string.
extern CRKDECLSPEC double CRKCALL CRK_DictGetValue(CRK_Dict *dict, const char *key);
extern CRKDECLSPEC bool CRKCALL CRK_DictIsString(CRK_Dict *dict, const char *key);

#endif