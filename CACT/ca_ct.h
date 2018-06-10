#pragma once

#if defined(_MSC_VER)
  #if _MSC_VER >= 1600
    #include "ca_ct_msvc.h"
  #else
    #error MSVC version too old
  #endif
#elif defined(__GNUC__) || defined(__GNUG__)
  #ifndef GCC_VERSION
    #define GCC_VERSION (__GNUC__ * 10000 \
                                  + __GNUC_MINOR__ * 100 \
                                  + __GNUC_PATCHLEVEL__)
  #endif //GCC_VERSION
  #if GCC_VERSION < 50000
    #include "ca_ct_gcc_98.h"
  #else
    #include "ca_ct_gcc_cxx11.h"
  #endif //GCC_VERSION
#else
  #include "ca_ct_gcc_98.h"
#endif
