#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

/*--------------------------------------*/
/*| STANDARD LIBRARY REPLACEMENTS INIT |*/
/*--------------------------------------*/

typedef unsigned int size_t;
typedef unsigned int uintptr_t;

#define __NULL ((void *)0)

/* __malloc and __free */

#define __MALLOC_BLOCK_SIZE (sizeof(__MallocBlock) - sizeof(char))
#define __offsetof(type, member) ((size_t) &((type *)0)->member)

typedef struct __MallocBlock {
    size_t size;
    struct __MallocBlock* next;
    int free;
    char data[1];
} __MallocBlock;

__MallocBlock* freeList = __NULL;

/* variadic */

typedef char* __va_list;
#define __va_start(ap, param) (ap = (char*)&param + sizeof(param))
#define __va_arg(ap, type) (*(type*)((ap += sizeof(type)) - sizeof(type)))
#define __va_end(ap) (ap = __NULL)

/* math constants */

#define PI 3.14159265358979323846
#define HALF_PI 1.57079632679489661923
#define TWO_PI 6.28318530717958647692

/* Helper functions */

void __int2str(int value, char* buffer);
void __float2str(float value, char* buffer, int precision);
void __ptr2str(void* ptr, char* buffer);
