/**
 * @file include/sl/env.h
 */

#ifndef SL_ENV_H_
#define SL_ENV_H_

#include "sldefs.h"
#include "sl/types.h"

/*
 * [ C O N V E R S I O N ]
 */


/*
 * [ C T R A I T S ]
 */

sl_API sl_Ctrait* sl_findctrait(sl_Env* E, const char* name);

#endif
