#ifndef SLDEFS_H_
#define SLDEFS_H_

#if !defined(sl_STATIC) && defined(_WIN32)
#   define sl_FIMPORT __declspec(dllimport)
#   define sl_FEXPORT __declspec(dllexport)
#   define sl_DIMPORT sl_FIMPORT
#   define sl_DEXPORT sl_FEXPORT
#else
#   define sl_FIMPORT extern
#   define sl_FEXPORT
#   define sl_DIMPORT sl_FIMPORT
#   define sl_DEXPORT
#endif


#if defined(sl_BUILDING)
#   define sl_API sl_FEXPORT
#   define sl_DATA sl_DEXPORT
#   if defined(sl_STATIC)
#       define sl_IAPI static
#       define sl_IDATA
#   else
#       if defined(__GNUC__) && ((__GNUC__*100 + __GNUC_MINOR__) >= 302) && \
                defined(__ELF__)
#           define sl_IAPI __attribute__((visibility("hidden"))) extern
#           define sl_IDATA sl_IAPI
#       else
#           define sl_IAPI extern
#           define sl_IDATA extern
#       endif
#   endif
#else
#   define sl_API sl_FIMPORT
#   define sl_DATA sl_DIMPORT
#endif


#endif
