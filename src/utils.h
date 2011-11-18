#include <node.h>
#include <v8.h>

#ifndef NODE_LIVESTREAMING_UTILS
#define NODE_LIVESTREAMING_UTILS

namespace livestreaming {

#ifndef countof
#ifdef _countof
#define countof _countof
#else
#define countof(a) (sizeof(a) / sizeof(*(a)))
#endif
#endif

}; // livestreaming

#endif // NODE_LIVESTREAMING_UTILS
