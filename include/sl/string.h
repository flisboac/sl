/**
 * @file include/sl/string.h
 */

#ifndef SL_STRING_H_
#define SL_STRING_H_

#include "sl.h"


typedef struct sl_Cstring_ sl_Cstring;

struct sl_Cstring_ {

	sl_Stream stream;
	size_t sz;
	size_t cap;
	char* ptr;
};


#endif
