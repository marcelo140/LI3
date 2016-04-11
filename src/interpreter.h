#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "fatglobal.h"
#include "products.h"
#include "clients.h"

/* void present(PRODUCTSET ps); */
int interpreter(FATGLOBAL fat, PRODUCTCAT pcat, CLIENTCAT ccat); 

#endif
