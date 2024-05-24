
#include "allstdreplfunc.h"

/*------------*/
/* Prototypes */
/*------------*/

/* type conversion */
void stdr_i2str(int value, char* buffer); // int to string
void stdr_f2str(float value, char* buffer, int precision); // float to string. depends on stdr_strcpy, stdr_i2str, stdr_strcat
void stdr_l2hex(unsigned long value, char* buffer); // long to hex
void stdr_l2str(long value, char* buffer); // long to string
void stdr_ptr2str(void* ptr, char* buffer); // memory pointer to string
void stdr_sz_t2hex(stdr_size_t value, char* hex, stdr_size_t hex_size); // size_t to hex
void stdr_sz_t2str(stdr_size_t value, char* buffer); // size_t to string
void stdr_str_out(const char* message); // output string to console - depends on stdr_strlen
char* stdr_str2c(const char* str, int c); // string to char
long stdr_str2l(const char* str, char** endptr, int base); // string to long
void stdr_ui2hex(unsigned int value, char* buffer); // uint to hex
void stdr_ui2str(stdr_size_t value, char* buffer); // uint to string
void stdr_ul2hex(unsigned long value, char* buffer); // ulong to hex
void stdr_ul2str(unsigned long value, char* buffer); // ulong to string
void stdr_uc2str(unsigned char value, char* buffer); // uchar to string
void stdr_us2str(unsigned short value, char* buffer); // ushort to string
/* ctype.h */
int stdr_isdigit(int c);
int stdr_isspace(int c);
int stdr_isxdigit(int c);
/* math.h */
float stdr_asin(float x);
float stdr_atan2(float y, float x); // depends on stdr_fabs
float stdr_cos(float x);
float stdr_fabs(float x);
float stdr_pow(float base, int exp);
float stdr_sin(float x);
float stdr_sqrt(float x);
float stdr_tanf(float x);
/* stdio.h */
WIN_VOID stdr_fclose(WIN_FILE* stream); // depends on stdr_free
WIN_INT stdr_feof(WIN_FILE* stream);
WIN_INT stdr_fgetc(WIN_FILE* stream); // depends on stdr_read
WIN_PCHR stdr_fgets(WIN_PCHR str, WIN_INT n, WIN_FILE* stream); // depends on stdr_read
WIN_INT stdr_fprintf(WIN_HANDLE file, WIN_PCCHR format, ...); // depends on stdr_vsnprintf
WIN_INT stdr_fscanf(WIN_FILE* stream, WIN_PCCHR format, ...); // depends on stdr_vsnprintf
WIN_FILE* stdr_fopen(WIN_PCCHR filename, WIN_PCCHR mode); // depends on stdr_strcmp, stdr_malloc
WIN_SIZE_T stdr_fread(WIN_PVOID ptr, WIN_SIZE_T size, WIN_SIZE_T count, WIN_FILE* stream);
WIN_INT stdr_fseek(WIN_FILE* stream, long offset, int whence);
WIN_LONG stdr_ftell(WIN_FILE* stream); // depends on stdr_printf
WIN_SIZE_T stdr_fwrite(WIN_PCVOID ptr, WIN_SIZE_T size, WIN_SIZE_T count, WIN_FILE* stream); // depends on stdr_printf
WIN_VOID stdr_perror(WIN_PCCHR s); // depends on stdr_printf
int stdr_printf(const char* format, ...); // depends on stdr_ul2hex, stdr_strlen, stdr_strcpy, stdr_ul2str, stdr_sz_t2str, stdr_us2str, stdr_uc2str, stdr_i2str, stdr_f2str,  stdr_ptr2str, stdr_ui2str, stdr_ui2hex, stdr_str_out
WIN_VOID stdr_rewind(WIN_FILE* stream);
int stdr_sprintf(char* buffer, const char* format, ...); // depends on stdr_ul2str, stdr_strlen, stdr_strcpy
int stdr_sscanf_hex(const char* str, unsigned long* value);
int stdr_vsnprintf(char* buffer, stdr_size_t count, const char* format, stdr_va_list args); // depends on stdr_i2str, stdr_strlen, stdr_strcpy, stdr_f2str
/* stdlib.h */
void* stdr_calloc(stdr_size_t num, stdr_size_t size); // depends on stdr_malloc
void stdr_exit(int status);
void stdr_free(void* ptr); // depends on stdr_offsetof
WIN_PVOID stdr_malloc(WIN_SIZE_T size);
stdr_size_t stdr_malloc_block_size(void* ptr); // depends on stdr_offsetof
void* stdr_realloc(void* ptr, stdr_size_t new_size); // depends on stdr_malloc, stdr_malloc_block_size, stdr_free, stdr_memcpy
/* string.h */
void* stdr_memcpy(void* dst, const void* src, stdr_size_t n);
int stdr_memcmp(const void* s1, const void* s2, stdr_size_t n);
void* stdr_memset(void* s, int c, stdr_size_t n);
char* stdr_strcat(char* dst, const char* src);
char* stdr_strchr(const char* str, int c);
int stdr_strcmp(const char* str1, const char* str2);
char* stdr_strcpy(char* dst, const char* src);
char* stdr_strdup(const char* str); // depends on stdr_strlen, stdr_malloc
stdr_size_t stdr_strlen(const char* str);
int stdr_strncmp(const char* s1, const char* s2, stdr_size_t n);
char* stdr_strrchr(const char* str, int c);
char* stdr_strstr(const char* haystack, const char* needle);
/* unistd.h */
WIN_SIZE_T stdr_read(WIN_PVOID ptr, WIN_SIZE_T count, WIN_FILE* stream);
/* wchar.h */
stdr_size_t stdr_wcslen(const stdr_wchar_t* s);
stdr_size_t stdr_wcstombs(char* dst, const stdr_wchar_t* src, stdr_size_t max);

/*-------------------*/
/* Library Functions */
/*-------------------*/

/* type conversion */
void stdr_i2str(int value, char* buffer) {
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
void stdr_f2str(float value, char* buffer, int precision) {
    if (precision < 0) precision = 6;

    if (value == 0.0f) {
        stdr_strcpy(buffer, "0.0");
        return;
    }

    int negative = (value < 0);
    if (negative) value = -value;

    int int_part = (int)value;
    float frac_part = value - int_part;

    char int_buffer[32];
    stdr_i2str(int_part, int_buffer);
    char frac_buffer[32];
    frac_part *= stdr_pow(10, precision);
    stdr_i2str((int)frac_part, frac_buffer);

    if (negative) {
        stdr_strcpy(buffer, "-");

        stdr_strcat(buffer, int_buffer);
    } else {
        stdr_strcpy(buffer, int_buffer);

    }
    stdr_strcat(buffer, ".");
    stdr_strcat(buffer, frac_buffer);
}
void stdr_l2hex(unsigned long value, char* buffer) {
    const char* hex_digits = "0123456789ABCDEF";
    int index = 0;

    if (value == 0) {
        buffer[index++] = '0';
    }

    int num_digits = 0;
    char reverse_hex[50];

    while (value != 0) {
        reverse_hex[num_digits++] = hex_digits[value & 0xF];
        value >>= 4;
    }

    for (int i = num_digits - 1; i >= 0; i--) {
        buffer[index++] = reverse_hex[i];
    }

    buffer[index] = '\0';
}
void stdr_l2str(long value, char* buffer) {
    if (value < 0) {
        *buffer++ = '-';
        value = -value;
    }

    char reverse[20];
    int i = 0;
    if (value == 0) {
        reverse[i++] = '0';
    }

    while (value > 0) {
        reverse[i++] = (value % 10) + '0';
        value /= 10;
    }

    while (i > 0) {
        *buffer++ = reverse[--i];
    }
    *buffer = '\0';
}
void stdr_ptr2str(void* ptr, char* buffer) {
    // dep stdr_printf
    const char* hex_digits = "0123456789ABCDEF";
    stdr_uintptr_t value = (stdr_uintptr_t)ptr;
    char* p = buffer;
    *p++ = '0';
    *p++ = 'x';
    for (int i = (sizeof(void*) * 2) - 1; i >= 0; i--) {
        *p++ = hex_digits[(value >> (i * 4)) & 0xF];
    }
    *p = '\0';
}
void stdr_sz_t2hex(stdr_size_t value, char* hex, stdr_size_t hex_size) {
    const char* hex_digits = "0123456789ABCDEF";
    hex[hex_size - 1] = '\0';
    for (int i = hex_size - 2; i >= 0; --i) {
        hex[i] = hex_digits[value & 0xF];
        value >>= 4;
    }
}
void stdr_sz_t2str(stdr_size_t value, char* buffer) {
    char temp[32];
    int i = 0, j = 0;

    do {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);

    temp[i] = '\0';

    for (j = 0, i--; i >= 0; i--, j++) {
        buffer[j] = temp[i];
    }
    buffer[j] = '\0';
}
void stdr_str_out(WIN_PCCHR message) {
    WIN_HANDLE hConsole = __GetStdHandle(WIN_STD_OUTPUT_HANDLE);
    if (hConsole != WIN_INVALID_HANDLE_VALUE) {
        WIN_DWORD written;
        __WriteConsoleA(hConsole, message, stdr_strlen(message), &written, STDR_NULL);
    }
}
char* stdr_str2c(const char* str, int c) {
    while (*str != '\0') {
        if (*str == (char)c) {
            return (char*)str;
        }
        str++;
    }
    return STDR_NULL;
}
long stdr_str2l(const char* str, char** endptr, int base) {
    if (base < 0 || base == 1 || base > 36) {
        if (endptr) {
            *endptr = (char*)str;
        }
        return 0;
    }

    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r' || *str == '\f' || *str == '\v') {
        str++;
    }

    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    long result = 0;
    while (*str) {
        int digit;
        if (*str >= '0' && *str <= '9') {
            digit = *str - '0';
        } else if (*str >= 'A' && *str <= 'Z') {
            digit = *str - 'A' + 10;
        } else if (*str >= 'a' && *str <= 'z') {
            digit = *str - 'a' + 10;
        } else {
            break;
        }

        if (digit >= base) {
            break;
        }

        result = result * base + digit;
        str++;
    }

    if (endptr) {
        *endptr = (char*)str;
    }

    return sign * result;
}
void stdr_ui2hex(unsigned int value, char* buffer) {
    const char* hex_digits = "0123456789ABCDEF";
    int index = 0;

    if (value == 0) {
        buffer[index++] = '0';
    }

    int num_digits = 0;
    char reverse_hex[50];

    while (value != 0) {
        reverse_hex[num_digits++] = hex_digits[value & 0xF];
        value >>= 4;
    }

    for (int i = num_digits - 1; i >= 0; i--) {
        buffer[index++] = reverse_hex[i];
    }

    buffer[index] = '\0';
}
void stdr_ui2str(stdr_size_t value, char* buffer) {
    char temp[32];
    int i = 0, j = 0;

    do {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);

    temp[i] = '\0';

    for (j = 0, i--; i >= 0; i--, j++) {
        buffer[j] = temp[i];
    }
    buffer[j] = '\0';
}
void stdr_ul2hex(unsigned long value, char* buffer) {
    const char* hex_digits = "0123456789ABCDEF";
    char temp[17];
    int i = 0;

    if (value == 0) {
        temp[i++] = '0';
    }

    while (value > 0) {
        temp[i++] = hex_digits[value & 0xF];
        value >>= 4;
    }

    temp[i] = '\0';

    int j = 0;
    while (i > 0) {
        buffer[j++] = temp[--i];
    }
    buffer[j] = '\0';
}
void stdr_ul2str(unsigned long value, char* buffer) {
    const char* hex_digits = "0123456789ABCDEF";
    int index = 0;

    if (value == 0) {
        buffer[index++] = '0';
    }

    int num_digits = 0;
    char reverse_hex[50];

    while (value != 0) {
        reverse_hex[num_digits++] = hex_digits[value & 0xF];
        value >>= 4;
    }

    for (int i = num_digits - 1; i >= 0; i--) {
        buffer[index++] = reverse_hex[i];
    }

    buffer[index] = '\0';
}
void stdr_uc2str(unsigned char value, char* buffer) {
    char temp[4];
    int i = 0;
    do {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    } while (value > 0);
    temp[i] = '\0';
    for (int j = 0; j < i; j++) {
        buffer[j] = temp[i - j - 1];
    }
    buffer[i] = '\0';
}
void stdr_us2str(unsigned short value, char* buffer) {
    char temp[6];
    int i = 0;
    do {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    } while (value > 0);
    temp[i] = '\0';

    for (int j = 0; j < i; j++) {
        buffer[j] = temp[i - j - 1];
    }
    buffer[i] = '\0';
}
/* ctype.h */
int stdr_isdigit(int c) {
    return (c >= '0' && c <= '9');
}
int stdr_isspace(int c) {
    return (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v');
}
int stdr_isxdigit(int c) {
    return ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'f') ||
            (c >= 'A' && c <= 'F'));
}
/* math.h */
float stdr_asin(float x) {
    if (x > 1.0 || x < -1.0) return 0.0;
    float result = x;
    float term = x;
    float x2 = x * x;
    for (int n = 1; n < 10; ++n) {
        term *= (x2 * (2 * n - 1) * (2 * n - 1)) / ((2 * n) * (2 * n + 1));
        result += term;
    }
    return result;
}
float stdr_atan2(float y, float x) {
    if (x == 0.0) {
        if (y > 0.0) return STDR_HALF_PI;
        if (y < 0.0) return -STDR_HALF_PI;
        return 0.0;
    }
    float abs_y = stdr_fabs(y), abs_x = stdr_fabs(x);
    float a = (abs_x - abs_y) / (abs_x + abs_y);
    float angle = STDR_PI / 4.0 - STDR_PI / 4.0 * a;
    if (x < 0.0) {
        if (y < 0.0) return -STDR_PI + angle;
        return STDR_PI - angle;
    }
    return y < 0.0 ? -angle : angle;
}
float stdr_cos(float x) {
    while (x > STDR_PI) x -= STDR_TWO_PI;
    while (x < -STDR_PI) x += STDR_TWO_PI;

    float result = 1.0;
    float term = 1.0;
    float x2 = x * x;
    int sign = -1;
    for (int n = 2; n <= 20; n += 2) {
        term *= x2 / ((n - 1) * n);
        result += sign * term;
        sign = -sign;
    }
    return result;
}
float stdr_fabs(float x) {
    return x < 0 ? -x : x;
}
float stdr_pow(float base, int exp) {
    float result = 1.0;
    while (exp > 0) {
        result *= base;
        exp--;
    }
    return result;
}
float stdr_sin(float x) {
    while (x > STDR_PI) x -= STDR_TWO_PI;
    while (x < -STDR_PI) x += STDR_TWO_PI;

    float result = 0.0;
    float term = x;
    float x2 = x * x;
    int sign = 1;
    for (int n = 1; n <= 19; n += 2) {
        result += sign * term;
        term *= x2 / ((n + 1) * (n + 2));
        sign = -sign;
    }
    return result;
}
float stdr_sqrt(float x) {
    float y = 1.0;
    float epsilon = 0.000001; // Precision level
    float x0 = x;

    while (x - y > epsilon) {
        x = (x + y) / 2;
        y = x0 / x;
    }
    return x;
}
float stdr_tanf(float x) {
    // Constants for a polynomial approximation
    const float a = 0.3333314036f;
    const float b = 0.1333923995f;
    const float c = 0.0533740603f;
    const float d = 0.0245650893f;
    const float e = 0.0029671978f;
    const float f = 0.0002729889f;

    float x2 = x * x;
    return x * (1 + x2 * (a + x2 * (b + x2 * (c + x2 * (d + x2 * (e + x2 * f))))));
}
/* stdio.h */
WIN_VOID stdr_fclose(WIN_FILE* stream) {
    if (stream) {
        __FlushFileBuffers((WIN_HANDLE)stream->hFile);
        __CloseHandle((WIN_HANDLE)stream->hFile);
        stdr_free(stream);
    }
}
WIN_INT stdr_feof(WIN_FILE* stream) {
    WIN_DWORD dwCurrentPos = __SetFilePointer((WIN_HANDLE)stream->hFile, 0, STDR_NULL, WIN_FILE_CURRENT);
    WIN_DWORD dwFileSize = __GetFileSize((WIN_HANDLE)stream->hFile, STDR_NULL);
    return dwCurrentPos >= dwFileSize;
}
WIN_INT stdr_fgetc(WIN_FILE* stream) {
    WIN_CHR c;
    if (stdr_read(&c, 1, stream->hFile) == 1) {
        return (WIN_UCHR)c;
    } else {
        return WIN_EOF;
    }
}
WIN_PCHR stdr_fgets(WIN_PCHR str, WIN_INT n, WIN_FILE* stream) {
    WIN_INT i = 0;
    WIN_CHR c;
    while (i < n - 1) {
        if (stdr_read(&c, 1, stream->hFile) != 1) {
            break;
        }
        str[i++] = c;
        if (c == '\n') {
            break;
        }
    }
    str[i] = '\0';
    return i > 0 ? str : STDR_NULL;
}
WIN_INT stdr_fprintf(WIN_HANDLE file, WIN_PCCHR format, ...) {
    WIN_CHR buffer[1024];
    stdr_va_list args;
    stdr_va_start(args, format);
    WIN_INT ret = stdr_vsnprintf(buffer, 0, format, args);
    stdr_va_end(args);

    WIN_DWORD written;
    __WriteFile(file, buffer, stdr_strlen(buffer), &written, STDR_NULL);
    return ret;
}
WIN_INT stdr_fscanf(WIN_FILE* stream, WIN_PCCHR format, ...) {
    WIN_CHR buffer[1024];
    stdr_size_t i = 0;
    WIN_INT c;
    while (i < sizeof(buffer) - 1) {
        c = stdr_read((WIN_HANDLE)&buffer[i], 1, stream->hFile);

        if (c == 0 || buffer[i] == '\n') {
            break;
        }
        i++;
    }
    buffer[i] = '\0';

    stdr_va_list args;
    stdr_va_start(args, format);
    WIN_INT ret = stdr_vsnprintf(STDR_NULL, 0, format, args);
    stdr_va_end(args);
    return ret;
}
WIN_FILE* stdr_fopen(WIN_PCCHR filename, WIN_PCCHR mode) {

    WIN_DWORD dwDesiredAccess = 0;
    WIN_DWORD dwCreationDisposition = WIN_OPEN_EXISTING;

    if (stdr_strcmp(mode, "rb") == 0) {
        dwDesiredAccess = WIN_GENERIC_READ;
    } else if (stdr_strcmp(mode, "wb") == 0) {
        dwDesiredAccess = WIN_GENERIC_WRITE;
        dwCreationDisposition = WIN_CREATE_ALWAYS;
    } else if (stdr_strcmp(mode, "r+b") == 0) {
        dwDesiredAccess = WIN_GENERIC_READ | WIN_GENERIC_WRITE;
        dwCreationDisposition = WIN_OPEN_ALWAYS;
    }

    WIN_HANDLE hFile = __CreateFileA(filename, dwDesiredAccess, WIN_FILE_SHARE_READ, STDR_NULL, dwCreationDisposition, WIN_FILE_ATTRIBUTE_NORMAL, STDR_NULL);
    if (hFile == WIN_INVALID_HANDLE_VALUE) {
        return STDR_NULL;
    }

    WIN_FILE* file = (WIN_FILE*)stdr_malloc(sizeof(WIN_FILE));
    if (!file) {
        __CloseHandle((WIN_HANDLE)hFile);
        return STDR_NULL;
    }

    file->hFile = hFile;
    return file;
}
WIN_SIZE_T stdr_fread(WIN_PVOID ptr, WIN_SIZE_T size, WIN_SIZE_T count, WIN_FILE* stream) {
    if (size == 0 || count == 0) return 0;

    WIN_SIZE_T total_size = size * count;

    WIN_DWORD bytesRead;
    if (!__ReadFile((WIN_HANDLE)stream->hFile, ptr, (WIN_DWORD)total_size, &bytesRead, STDR_NULL)) {
        return -1;
    }

    stdr_size_t elements_read = bytesRead / size;
    return elements_read;
}
WIN_INT stdr_fseek(WIN_FILE* stream, WIN_LONG offset, WIN_INT whence) {
    WIN_DWORD dwMoveMethod;
    switch (whence) {
        case WIN_SEEK_SET: dwMoveMethod = WIN_FILE_BEGIN; break;
        case WIN_SEEK_CUR: dwMoveMethod = WIN_FILE_CURRENT; break;
        case WIN_SEEK_END: dwMoveMethod = WIN_FILE_END; break;
        default: return -1;
    }
    if (__SetFilePointer((WIN_HANDLE)stream->hFile, offset, STDR_NULL, dwMoveMethod) == WIN_INVALID_SET_FILE_POINTER && __GetLastError() != WIN_NO_ERROR) {
        return -1;
    }
    return 0;
}
WIN_LONG stdr_ftell(WIN_FILE* stream) {
    if (!stream || !stream->hFile) {
        stdr_printf("Error: Invalid file stream or handle.\n");
        return -1;
    }

    WIN_DWORD dwPos = __SetFilePointer((WIN_HANDLE)stream->hFile, 0, STDR_NULL, WIN_FILE_CURRENT);
    if (dwPos == WIN_INVALID_SET_FILE_POINTER) {
        WIN_DWORD dwError = __GetLastError();
        stdr_printf("SetFilePointer failed with error %lu\n", dwError);
        return -1;
    }
    return (WIN_LONG)dwPos;
}
WIN_SIZE_T stdr_fwrite(WIN_PCVOID ptr, WIN_SIZE_T size, WIN_SIZE_T count, WIN_FILE* stream) {
    WIN_SIZE_T total_size = size * count;
    WIN_DWORD bytesWritten;
    WIN_BOOL result = __WriteFile((WIN_HANDLE)stream->hFile, ptr, (WIN_DWORD)total_size, &bytesWritten, STDR_NULL);
    if (!result || bytesWritten < total_size) {
        stdr_printf("Failed to write expected bytes: expected %zu, wrote %lu\n", total_size, bytesWritten);
        return bytesWritten / size;
    }
    return count;
}
WIN_VOID stdr_perror(WIN_PCCHR s) {
    WIN_DWORD errorMessageID = __GetLastError();
    if (errorMessageID == 0) {
        stdr_printf("%s: No error\n", s);
        return;
    }

    WIN_PCHR messageBuffer = STDR_NULL;
    __FormatMessageA(
        WIN_FORMAT_MESSAGE_ALLOCATE_BUFFER | WIN_FORMAT_MESSAGE_FROM_SYSTEM | WIN_FORMAT_MESSAGE_IGNORE_INSERTS,
        STDR_NULL, errorMessageID, __MAKELANGID(WIN_LANG_NEUTRAL, WIN_SUBLANG_DEFAULT), (WIN_PCHR)&messageBuffer, 0, STDR_NULL);

    stdr_printf("%s: %s\n", s, messageBuffer);
    __LocalFree(messageBuffer);
}
int stdr_printf(const char* format, ...) {
    char buffer[1024];
    char num_buffer[64];
    const char* p = format;
    char* b = buffer;
    stdr_va_list args;
    stdr_va_start(args, format);

    while (*p) {
        if (*p == '%') {
            ++p;
            int width = 0;
            int precision = 0;
            int zero_pad = 0;

            if (*p == '0') {
                zero_pad = 1;
                ++p;
            }

            while (*p >= '0' && *p <= '9') {
                width = width * 10 + (*p - '0');
                ++p;
            }

            if (*p == '.') {
                ++p;
                while (*p >= '0' && *p <= '9') {
                    precision = precision * 10 + (*p - '0');
                    ++p;
                }
            }
            if (*p == 'l') {
                ++p;
                if (*p == 'X') {
                    unsigned long value = stdr_va_arg(args, unsigned long);
                    stdr_ul2hex(value, num_buffer);
                    int len = stdr_strlen(num_buffer);
                    int pad = (width > len) ? width - len : 0;
                    if (zero_pad) {
                        while (pad-- > 0) *b++ = '0';
                    }
                    stdr_strcpy(b, num_buffer);
                    b += len;
                } else if (*p == 'u') {
                    unsigned long value = stdr_va_arg(args, unsigned long);
                    stdr_ul2str(value, num_buffer);
                    int len = stdr_strlen(num_buffer);
                    int pad = (width > len) ? width - len : 0;
                    if (zero_pad) {
                        while (pad-- > 0) *b++ = '0';
                    }
                    stdr_strcpy(b, num_buffer);
                    b += len;
                }
            } else if (*p == 'z') {
                ++p;
                if (*p == 'u') {
                    stdr_size_t value = stdr_va_arg(args, stdr_size_t);
                    stdr_sz_t2str(value, num_buffer);
                    int len = stdr_strlen(num_buffer);
                    int pad = (width > len) ? width - len : 0;
                    if (zero_pad) {
                        while (pad-- > 0) *b++ = '0';
                    }
                    stdr_strcpy(b, num_buffer);
                    b += len;
                }
            } else if (*p == 'h') {
                ++p;
                if (*p == 'u') {
                    unsigned short value = (unsigned short)stdr_va_arg(args, unsigned int);
                    stdr_us2str(value, num_buffer);
                    int len = stdr_strlen(num_buffer);
                    int pad = (width > len) ? width - len : 0;
                    if (zero_pad) {
                        while (pad-- > 0) *b++ = '0';
                    }
                    stdr_strcpy(b, num_buffer);
                    b += len;
                } else if (*p == 'h') {
                    ++p;
                    if (*p == 'u') {
                        unsigned char value = (unsigned char)stdr_va_arg(args, unsigned int);
                        stdr_uc2str(value, num_buffer);
                        int len = stdr_strlen(num_buffer);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        stdr_strcpy(b, num_buffer);
                        b += len;
                    }
                }
            } else {
                switch (*p) {
                    case 'i':
                    case 'd': {
                        int value = stdr_va_arg(args, int);
                        stdr_i2str(value, num_buffer);
                        int len = stdr_strlen(num_buffer);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        stdr_strcpy(b, num_buffer);
                        b += len;
                        break;
                    }
                    case 'f': {
                        float value = stdr_va_arg(args, float);
                        stdr_f2str((float)value, num_buffer, precision);
                        int len = stdr_strlen(num_buffer);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        stdr_strcpy(b, num_buffer);
                        b += len;
                        break;
                    }
                    case 's': {
                        const char* str = stdr_va_arg(args, const char*);
                        int len = stdr_strlen(str);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        stdr_strcpy(b, str);
                        b += len;
                        break;
                    }
                    case 'c': {
                        int char_value = stdr_va_arg(args, int);
                        *b++ = (char)char_value;
                        break;
                    }
                    case 'p': {
                        void* ptr = stdr_va_arg(args, void*);
                        stdr_ptr2str(ptr, num_buffer);
                        int len = stdr_strlen(num_buffer);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        stdr_strcpy(b, num_buffer);
                        b += len;
                        break;
                    }
                    case 'u': {
                        unsigned int value = stdr_va_arg(args, unsigned int);
                        stdr_ui2str(value, num_buffer);
                        int len = stdr_strlen(num_buffer);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        stdr_strcpy(b, num_buffer);
                        b += len;
                        break;
                    }
                    case 'X': {
                        unsigned int value = stdr_va_arg(args, unsigned int);
                        stdr_ui2hex(value, num_buffer);
                        int len = stdr_strlen(num_buffer);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        stdr_strcpy(b, num_buffer);
                        b += len;
                        break;
                    }
                    default:
                        *b++ = '%'; *b++ = *p;
                        break;
                }
            }
        } else {
            *b++ = *p;
        }
        p++;
    }

    *b = '\0';
    stdr_va_end(args);

    stdr_str_out(buffer);
    return stdr_strlen(buffer);
}
WIN_VOID stdr_rewind(WIN_FILE* stream) {
    __SetFilePointer((WIN_HANDLE)stream->hFile, 0, STDR_NULL, WIN_FILE_BEGIN);
}
int stdr_sprintf(char* buffer, const char* format, ...) {
    char num_buffer[64];
    const char* p = format;
    char* b = buffer;
    stdr_va_list args;
    stdr_va_start(args, format);

    while (*p) {
        if (*p == '%') {
            ++p;
            int width = 0;
            while (*p >= '0' && *p <= '9') {
                width = width * 10 + (*p - '0');
                ++p;
            }
            if (*p == 'l') {
                ++p;
                if (*p == 'X' || *p == 'x') {
                    unsigned long value = stdr_va_arg(args, unsigned long);
                    stdr_ul2str(value, num_buffer);
                    int num_len = stdr_strlen(num_buffer);
                    stdr_strcpy(b, num_buffer);
                    b += num_len;
                }
            } else {
                *b++ = '%';
                *b++ = *p;
            }
        } else {
            *b++ = *p;
        }
        ++p;
    }

    *b = '\0';
    stdr_va_end(args);
    return stdr_strlen(buffer);
}
int stdr_sscanf_hex(const char* str, unsigned long* value) {
    unsigned long result = 0;
    int count = 0;
    while (*str) {
        char c = *str++;
        int digit = (c >= '0' && c <= '9') ? c - '0' :
                    (c >= 'a' && c <= 'f') ? c - 'a' + 10 :
                    (c >= 'A' && c <= 'F') ? c - 'A' + 10 : -1;
        if (digit == -1) break;
        result = result * 16 + digit;
        count++;
    }
    *value = result;
    return count ? 1 : 0;
}
int stdr_vsnprintf(char* buffer, stdr_size_t count, const char* format, stdr_va_list args) {
    char num_buffer[64];
    const char* p = format;
    char* b = buffer;
    stdr_size_t remaining = count;
    stdr_size_t required = 0;

    while (*p) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'i':
                case 'd': {
                    int value = stdr_va_arg(args, int);
                    stdr_i2str(value, num_buffer);
                    stdr_size_t len = stdr_strlen(num_buffer);
                    required += len;
                    if (buffer && remaining > len) {
                        stdr_strcpy(b, num_buffer);
                        b += len;
                        remaining -= len;
                    } else {
                        if (buffer) remaining = 1;
                    }
                    break;
                }
                case 'f': {
                    float value = stdr_va_arg(args, float);
                    stdr_f2str((float)value, num_buffer, 6);
                    stdr_size_t len = stdr_strlen(num_buffer);
                    required += len;
                    if (buffer && remaining > len) {
                        stdr_strcpy(b, num_buffer);
                        b += len;
                        remaining -= len;
                    } else {
                        if (buffer) remaining = 1;
                    }
                    break;
                }
                case 's': {
                    const char* str = stdr_va_arg(args, const char*);
                    stdr_size_t len = stdr_strlen(str);
                    required += len;
                    if (buffer && remaining > len) {
                        stdr_strcpy(b, str);
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
/* stdlib.h */
void* stdr_calloc(stdr_size_t num, stdr_size_t size) {
    stdr_size_t total_size = num * size;

    void* ptr = stdr_malloc(total_size);
    if (!ptr) {
        return STDR_NULL;
    }

    unsigned char* byte_ptr = (unsigned char*)ptr;
    for (stdr_size_t i = 0; i < total_size; i++) {
        byte_ptr[i] = 0;
    }

    return ptr;
}
WIN_VOID stdr_exit(WIN_INT status) {
    __ExitProcess((WIN_UINT)status);
}
void stdr_free(void* ptr) {
    if (!ptr) return;

    stdr_MallocBlock* block = (stdr_MallocBlock*)((char*)ptr - stdr_offsetof(stdr_MallocBlock, data));
    block->free = 1;

    stdr_MallocBlock* current = freeList;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += STDR_MALLOC_BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
            // stdr_printf("", );
        }
        current = current->next;
    }
}
WIN_PVOID stdr_malloc(WIN_SIZE_T size) {
    stdr_MallocBlock *block, *last;
    stdr_size_t totalSize = size + STDR_MALLOC_BLOCK_SIZE;

    if (freeList == STDR_NULL) {
        block = (stdr_MallocBlock*)__VirtualAlloc(STDR_NULL, totalSize, WIN_MEM_COMMIT | WIN_MEM_RESERVE, WIN_PAGE_READWRITE);
        block->size = size;
        block->next = STDR_NULL;
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

    block = (stdr_MallocBlock*)__VirtualAlloc(STDR_NULL, totalSize, WIN_MEM_COMMIT | WIN_MEM_RESERVE, WIN_PAGE_READWRITE);
    block->size = size;
    block->next = STDR_NULL;
    block->free = 0;
    last->next = block;
    return block->data;
}
stdr_size_t stdr_malloc_block_size(void* ptr) {
    if (!ptr) return 0;

    stdr_MallocBlock* block = (stdr_MallocBlock*)((char*)ptr - stdr_offsetof(stdr_MallocBlock, data));
    return block->size;
}
void* stdr_realloc(void* ptr, stdr_size_t new_size) {
    if (!ptr) {
        return stdr_malloc(new_size);
    }

    if (new_size == 0) {
        stdr_free(ptr);
        return STDR_NULL;
    }

    void* new_ptr = stdr_malloc(new_size);
    if (!new_ptr) {
        return STDR_NULL;
    }

    stdr_size_t old_size = stdr_malloc_block_size(ptr);
    stdr_size_t copy_size = (old_size < new_size) ? old_size : new_size;
    stdr_memcpy(new_ptr, ptr, copy_size);
    stdr_free(ptr);

    return new_ptr;
}
/* string.h */
void* stdr_memcpy(void* dst, const void* src, stdr_size_t n) {
    char* d = (char*)dst;
    const char* s = (const char*)src;

    while (n--) {
        *d++ = *s++;
    }

    return dst;
}
int stdr_memcmp(const void* s1, const void* s2, stdr_size_t n) {
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;

    for (stdr_size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}
void* stdr_memset(void* s, int c, stdr_size_t n) {
    unsigned char* p = (unsigned char*)s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}
char* stdr_strcat(char* dst, const char* src) {
    char* dst_ptr = dst;

    while (*dst_ptr != '\0') {
        dst_ptr++;
    }

    while (*src != '\0') {
        *dst_ptr = *src;
        dst_ptr++;
        src++;
    }

    *dst_ptr = '\0';

    return dst;
}
char* stdr_strrchr(const char* str, int c) {
    const char *last = STDR_NULL;
    while (*str != '\0') {
        if (*str == (char)c) {
            last = str;
        }
        str++;
    }
    return (char*)last;
}
int stdr_strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}
char* stdr_strcpy(char* dst, const char* src) {
    char* dst_ptr = dst;

    while (*src != '\0') {
        *dst_ptr = *src;
        dst_ptr++;
        src++;
    }

    *dst_ptr = '\0';
    return dst;
}
char* stdr_strdup(const char* str) {
    stdr_size_t len = stdr_strlen(str) + 1;
    char* copy = (char*)stdr_malloc(len);
    if (copy) {
        for (stdr_size_t i = 0; i < len; i++) {
            copy[i] = str[i];
        }
    }
    return copy;
}
stdr_size_t stdr_strlen(const char* str) {
    stdr_size_t length = 0;

    while (str[length] != '\0') {
        length++;
    }
    return length;
}
int stdr_strncmp(const char* s1, const char* s2, stdr_size_t n) {
    while (n-- && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (n == (stdr_size_t)-1) ? 0 : *(unsigned char*)s1 - *(unsigned char*)s2;
}
char* stdr_strchr(const char* str, int c) {
    while (*str != '\0') {
        if (*str == (char)c) {
            return (char*)str;
        }
        str++;
    }
    return STDR_NULL;
}
char* stdr_strstr(const char* haystack, const char* needle) {
    if (!*needle) {
        return (char*)haystack;
    }
    char* p1 = (char*)haystack;
    while (*p1) {
        char* p1Begin = p1, * p2 = (char*)needle;
        while (*p1 && *p2 && *p1 == *p2) {
            p1++;
            p2++;
        }
        if (!*p2) {
            return p1Begin;
        }
        p1 = p1Begin + 1;
    }
    return STDR_NULL;
}
/* unistd.h */
WIN_SIZE_T stdr_read(WIN_PVOID ptr, WIN_SIZE_T count, WIN_FILE* stream) {
    WIN_DWORD bytesRead;
    if (!__ReadFile((WIN_HANDLE)stream->hFile, ptr, (WIN_DWORD)count, &bytesRead, STDR_NULL)) {
        return -1;
    }
    return bytesRead;
}
/* wchar.h */
stdr_size_t stdr_wcslen(const stdr_wchar_t* s) {
    const stdr_wchar_t* p = s;
    while (*p) {
        p++;
    }
    return p - s;
}
stdr_size_t stdr_wcstombs(char* dst, const stdr_wchar_t* src, stdr_size_t max) {
    stdr_size_t count = 0;
    while (count < max && *src) {
        if (*src < 0x80) {
            *dst++ = (char)*src++;
        } else if (*src < 0x800) {
            if (count + 1 >= max) break;
            *dst++ = (char)(0xC0 | (*src >> 6));
            *dst++ = (char)(0x80 | (*src & 0x3F));
            src++;
            count++;
        } else {
            if (count + 2 >= max) break;
            *dst++ = (char)(0xE0 | (*src >> 12));
            *dst++ = (char)(0x80 | ((*src >> 6) & 0x3F));
            *dst++ = (char)(0x80 | (*src & 0x3F));
            src++;
            count += 2;
        }
        count++;
    }
    *dst = '\0';
    return count;
}

/*-------------*/
/* ENTRY POINT */
/*-------------*/

int main_entry() {

    /* TEST CASES */

    /* stdr_strcat() */
    char cat1[20] = "Hello ";
    char cat2[20] = "World";
    stdr_strcat(cat1, cat2);
    stdr_printf("stdr_strcat(\"Hello \", \"World\"): %s\n", cat1);

    /* stdr_strpy() */
    char src[20] = "Hello World";
    char dst[20];
    stdr_strcpy(dst, src);
    stdr_printf("stdr_strcpy(\"Hello World\"): %s\n", dst);

    /* stdr_strlen() */
    stdr_printf("stdr_strlen(\"Hello World\"): %i\n", stdr_strlen("Hello World"));

    /* stdr_strout() - win */
    stdr_printf("stdr_str_out(\"Hello World\"): ");
    stdr_str_out("Hello World\n");

    /* stdr_malloc() */
    stdr_size_t num_elements = 10;
    stdr_size_t element_size = sizeof(int);
    int* array = (int*)stdr_malloc(num_elements * element_size);

    if (array) {
        for (stdr_size_t i = 0; i < num_elements; i++) {
            array[i] = (int)i;
        }
        stdr_str_out("stdr_malloc() result: ");

        for (stdr_size_t i = 0; i < num_elements; i++) {
            stdr_printf("%d ", array[i]);
        }
        stdr_str_out("\n");

        stdr_free(array);

        int* new_array = (int*)stdr_malloc(num_elements * element_size);

        if (new_array) {
            stdr_str_out("Reallocated after stdr_free(): ");
            for (stdr_size_t i = 0; i < num_elements; i++) {
                new_array[i] = (int)(i + 10);
                stdr_printf("%d ", new_array[i]);
            }
            stdr_str_out("\n");

            stdr_free(new_array);
        } else {
            stdr_str_out("stdr_malloc() after stdr_free(): Memory allocation failed\n");
        }
    } else {
        stdr_str_out("stdr_malloc(): Memory allocation failed\n");
    }





    /* stdr_scanf_hex() */
    unsigned long num;
    char* hexString = "1A3F";
    int result = stdr_sscanf_hex(hexString, &num);
    if (result == 1) {
        stdr_printf("stdr_sscanf_hex(\"1A3F\", &num) dec: %u\n", num);
    } else {
        stdr_printf("stdr_sscanf_hex(): Failed to parse number\n");
    }

    return 0;
}
void _start() {
    bootstrap_win32_api();
    main_entry();
    stdr_exit(0);
}
