#include "cstdrep.h"

/*---------------------------------*/
/*| STANDARD LIBRARY REPLACEMENTS |*/
/*---------------------------------*/

/* math.h */

double __pow(double base, int exp) {
    /* math.h pow replacement */

    double result = 1.0;
    while (exp > 0) {
        result *= base;
        exp--;
    }
    return result;
}

double __fabs(double x) {
    /* math.h fabs replacement */

    return x < 0 ? -x : x;
}

double __atan2(double y, double x) {
    /* math.h atan2 replacement */

    if (x == 0.0) {
        if (y > 0.0) return HALF_PI;
        if (y < 0.0) return -HALF_PI;
        return 0.0;
    }
    double abs_y = __fabs(y), abs_x = __fabs(x);
    double a = (abs_x - abs_y) / (abs_x + abs_y);
    double angle = PI / 4.0 - PI / 4.0 * a;
    if (x < 0.0) {
        if (y < 0.0) return -PI + angle;
        return PI - angle;
    }
    return y < 0.0 ? -angle : angle;
}

double __asin(double x) {
    /* math.h asin replacement */

    if (x > 1.0 || x < -1.0) return 0.0;
    double result = x;
    double term = x;
    double x2 = x * x;
    for (int n = 1; n < 10; ++n) {
        term *= (x2 * (2 * n - 1) * (2 * n - 1)) / ((2 * n) * (2 * n + 1));
        result += term;
    }
    return result;
}

double __sin(double x) {
    /* math.h sin replacement */

    while (x > PI) x -= TWO_PI;
    while (x < -PI) x += TWO_PI;

    double result = 0.0;
    double term = x;
    double x2 = x * x;
    int sign = 1;
    for (int n = 1; n <= 19; n += 2) {
        result += sign * term;
        term *= x2 / ((n + 1) * (n + 2));
        sign = -sign;
    }
    return result;
}

double __cos(double x) {
    /* math.h cos replacement */

    while (x > PI) x -= TWO_PI;
    while (x < -PI) x += TWO_PI;

    double result = 1.0;
    double term = 1.0;
    double x2 = x * x;
    int sign = -1;
    for (int n = 2; n <= 20; n += 2) {
        term *= x2 / ((n - 1) * n);
        result += sign * term;
        sign = -sign;
    }
    return result;
}

/* string.h */

size_t __strlen(const char* str) {
    /* string.h strlen replacement */

    return lstrlenA(str);
}

char* __strcpy(char* dest, const char* src) {
    /* string.h strcpy replacement */

    return lstrcpyA(dest, src);
}

char* __strcat(char* dest, const char* src) {
    /* string.h strcat replacement */

    return lstrcatA(dest, src);
}

int __strcmp(const char* str1, const char* str2) {
    /* string.h strcmp replacement */

    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

/* helpers */

void __int2str(int value, char* buffer) {
    /* int to string helper */

    char temp[32];
    int i = 0, j = 0;
    int is_negative = (value < 0);
    if (is_negative) {
        value = -value;
    }
    do {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);
    if (is_negative) {
        temp[i++] = '-';
    }
    temp[i] = '\0';

    for (j = 0, i--; i >= 0; i--, j++) {
        buffer[j] = temp[i];
    }
    buffer[j] = '\0';
}

void __float2str(float value, char* buffer, int precision) {
    /* float to string helper */

    if (precision < 0) precision = 6;

    if (value == 0.0f) {
        __strcpy(buffer, "0.0");
        return;
    }

    int negative = (value < 0);
    if (negative) value = -value;

    int int_part = (int)value;
    float frac_part = value - int_part;

    char int_buffer[32];
    __int2str(int_part, int_buffer);

    char frac_buffer[32];
    frac_part *= __pow(10, precision);
    __int2str((int)frac_part, frac_buffer);

    if (negative) {
        __strcpy(buffer, "-");
        __strcat(buffer, int_buffer);
    } else {
        __strcpy(buffer, int_buffer);
    }
    __strcat(buffer, ".");
    __strcat(buffer, frac_buffer);
}

void __ptr2str(void* ptr, char* buffer) {
    /* memory address pointer to string helper */

    const char* hex_digits = "0123456789ABCDEF";
    uintptr_t value = (uintptr_t)ptr;
    char* p = buffer;
    *p++ = '0';
    *p++ = 'x';
    for (int i = (sizeof(void*) * 2) - 1; i >= 0; i--) {
        *p++ = hex_digits[(value >> (i * 4)) & 0xF];
    }
    *p = '\0';
}

/* stdio.h */

void __str_out(const char* message) {
    /* output string to console */

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE) {
        DWORD written;
        WriteConsoleA(hConsole, message, __strlen(message), &written, __NULL);
    }
}

int __vsnprintf(char* buffer, size_t count, const char* format, __va_list args) {
    /* stdio.h vsnprintf replacement */

    char num_buffer[64];
    const char* p = format;
    char* b = buffer;
    size_t remaining = count;
    size_t required = 0;

    while (*p) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'i':
                case 'd': {
                    int value = __va_arg(args, int);
                    __int2str(value, num_buffer);
                    size_t len = __strlen(num_buffer);
                    required += len;
                    if (buffer && remaining > len) {
                        __strcpy(b, num_buffer);
                        b += len;
                        remaining -= len;
                    } else {
                        if (buffer) remaining = 1;
                    }
                    break;
                }
                case 'f': {
                    double value = __va_arg(args, double);
                    __float2str((float)value, num_buffer, 6);
                    size_t len = __strlen(num_buffer);
                    required += len;
                    if (buffer && remaining > len) {
                        __strcpy(b, num_buffer);
                        b += len;
                        remaining -= len;
                    } else {
                        if (buffer) remaining = 1;
                    }
                    break;
                }
                case 's': {
                    const char* str = __va_arg(args, const char*);
                    size_t len = __strlen(str);
                    required += len;
                    if (buffer && remaining > len) {
                        __strcpy(b, str);
                        b += len;
                        remaining -= len;
                    } else {
                        if (buffer) remaining = 1;
                    }
                    break;
                }
                default:
                    required += 2;
                    if (buffer && remaining > 2) {
                        *b++ = '%';
                        *b++ = *p;
                        remaining -= 2;
                    } else {
                        if (buffer) remaining = 1;
                    }
                    break;
            }
        } else {
            required++;
            if (buffer && remaining > 1) {
                *b++ = *p;
                remaining--;
            } else {
                if (buffer) remaining = 1;
            }
        }
        p++;
    }

    if (buffer && remaining > 0) {
        *b = '\0';
    }

    return required;
}

int __sprintf(char* buffer, const char* format, ...) {
    /* stdio.h sprintf replacement */

    char num_buffer[64];
    const char* p = format;
    char* b = buffer;
    __va_list args;
    __va_start(args, format);

    while (*p) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'i':
                case 'd': {
                    int value = __va_arg(args, int);
                    __int2str(value, num_buffer);
                    __strcpy(b, num_buffer);
                    b += __strlen(num_buffer);
                    break;
                }
                case 'f': {
                    double value = __va_arg(args, double);
                    __float2str((float)value, num_buffer, 6);
                    __strcpy(b, num_buffer);
                    b += __strlen(num_buffer);
                    break;
                }
                case 's': {
                    const char* str = __va_arg(args, const char*);
                    __strcpy(b, str);
                    b += __strlen(str);
                    break;
                }
                case 'p': {
                    void* ptr = __va_arg(args, void*);
                    __ptr2str(ptr, num_buffer);
                    __strcpy(b, num_buffer);
                    b += __strlen(num_buffer);
                    break;
                }
                default:
                    *b++ = '%';
                    *b++ = *p;
                    break;
            }
        } else {
            *b++ = *p;
        }
        p++;
    }

    *b = '\0';
    __va_end(args);

    return __strlen(buffer);
}

int __printf(const char* format, ...) {
    /* stdio.h printf replacement */

    char buffer[1024];
    char num_buffer[64];
    const char* p = format;
    char* b = buffer;
    __va_list args;
    __va_start(args, format);

    while (*p) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'i':
                case 'd': {
                    int value = __va_arg(args, int);
                    __int2str(value, num_buffer);
                    __strcpy(b, num_buffer);
                    b += __strlen(num_buffer);
                    break;
                }
                case 'f': {
                    double value = __va_arg(args, double);
                    __float2str((float)value, num_buffer, 6);
                    __strcpy(b, num_buffer);
                    b += __strlen(num_buffer);
                    break;
                }
                case 's': {
                    const char* str = __va_arg(args, const char*);
                    __strcpy(b, str);
                    b += __strlen(str);
                    break;
                }
                case 'p': {
                    void* ptr = __va_arg(args, void*);
                    __ptr2str(ptr, num_buffer);
                    __strcpy(b, num_buffer);
                    b += __strlen(num_buffer);
                    break;
                }
                default:
                    *b++ = '%';
                    *b++ = *p;
                    break;
            }
        } else {
            *b++ = *p;
        }
        p++;
    }

    *b = '\0';
    __va_end(args);

    __str_out(buffer);
    return __strlen(buffer);
}

int __fprintf(HANDLE file, const char* format, ...) {
    /* stdio.h fprintf replacement */

    char buffer[1024];
    __va_list args;
    __va_start(args, format);
    int ret = __vsnprintf(buffer, 0, format, args);
    __va_end(args);

    DWORD written;
    WriteFile(file, buffer, __strlen(buffer), &written, __NULL);
    return ret;
}

/* stdlib.h */

void* __malloc(size_t size) {
    /* stdlib.h malloc replacement */

    __MallocBlock *block, *last;
    size_t totalSize = size + __MALLOC_BLOCK_SIZE;

    if (freeList == __NULL) {
        block = (__MallocBlock*)VirtualAlloc(__NULL, totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (!block) {
            __printf("VirtualAlloc failed\n");
            return __NULL;
        }
        block->size = size;
        block->next = __NULL;
        block->free = 0;
        freeList = block;
        return block->data;
    }

    last = freeList;
    block = freeList;
    while (block) {
        if (block->free && block->size >= size) {
            block->free = 0;
            return block->data;
        }
        last = block;
        block = block->next;
    }

    block = (__MallocBlock*)VirtualAlloc(__NULL, totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!block) {
        __printf("VirtualAlloc failed\n");
        return __NULL;
    }
    block->size = size;
    block->next = __NULL;
    block->free = 0;
    last->next = block;
    return block->data;
}

void __free(void* ptr) {
    /* stdlib.h free replacement */

    if (!ptr) return;

    __MallocBlock* block = (__MallocBlock*)((char*)ptr - __offsetof(__MallocBlock, data));
    block->free = 1;

    __MallocBlock* current = freeList;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += __MALLOC_BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}

void* __memset(void* s, int c, size_t n) {
    /* string.h memset replacement */

    unsigned char* p = (unsigned char*)s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

void* __memcpy(void* dest, const void* src, size_t n) {
    /* string.h memcpy replacement */

    char* d = (char*)dest;
    const char* s = (const char*)src;

    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

/* string.h continued */

char* __strdup(const char* str) {
    /* string.h strdup replacement */

    size_t len = __strlen(str) + 1;
    char* copy = (char*)__malloc(len);
    if (copy) {
        for (size_t i = 0; i < len; i++) {
            copy[i] = str[i];
        }
    }
    return copy;
}

void main_entry() {
    char* sometext = "Hello World";

    __printf("sometext: %s\n", sometext);
    __printf("__strlen(sometext): %i\n", __strlen(sometext));
}

void _start() {
    main_entry();
    ExitProcess(0);
}
