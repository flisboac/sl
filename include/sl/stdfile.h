
#ifndef SL_STDFILE_H_
#define SL_STDFILE_H_

#include "sl.h"
#include <stdio.h>

typedef struct sl_Stdfile_ sl_Stdfile;


struct sl_Stdfile_ {

	sl_Stream stream;
	FILE* file;
	const char* path;
};


#endif /* SL_STDFILE_H_ */
