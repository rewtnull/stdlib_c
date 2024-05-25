# stdr

**-- ABOUT --**


    stdr (C Standard Library Replacement) is a (fairly) modular subset replacement for a variety of
    C standard library functions. If you aim at using that elusive -nostdlib flag in gcc, and/or want
    to size optimize your executables, these could be for you!


    The goal of this project is to allow for drop-in replacements for various C Standard
    Library functions without the need to include a complete library; you can lift out and use only the
    functions you require for your project.


**-- PLATFORM DEPENDENCY --**


    There are a collection of low level functions included that are win32 api specific. These are,
    however, structured in such a way that they can be ported to other platforms without having to
    rewrite functions they depend on, or that depend on them.


    The platform specific functions make api calls directly from kernel32.dll. stdr uses a
    bootstrapping mechanism that allows for this, so there is no need to #include anything. It
    should also be failry easy to include code for any other dll you wish to use functions from.


    I have chosen to typedef various data types so to not create conflicts with other possible
    definitions. Any platform agnostic typedefs use a stdr_ prefix (STDR_ for macro definitions).
    As for platform dependent typedefs and macros, (only windows for now), I have chosen win_,
    and WIN_ prefixes respectively. This should make it  easy to identify any platform dependent
    functions and definitions.


    I am avoiding internal/cross dependencies when ever possible, but sometimes these are a
    necessity. I have left comments for the function prototypes that will give you an idea on
    what internal dependencies functions may have.


**-- TARGETED HEADERS --**


    The targeted headers are:


    ctype.h, math.h, stdio.h, stddef.h, stdlib.h, string.h, unistd.h, wchar.h


**-- HELPERS --**


    The following type conversion helper functions are included:


    void  stdr_i2str(int value, char* buffer);                                       int to string
    void  stdr_f2str(float value, char* buffer, int precision);                      float to string
    void  stdr_l2hex(unsigned long value, char* buffer);                             long to hex
    void  stdr_l2str(long value, char* buffer);                                      long to string
    void  stdr_ptr2str(void* ptr, char* buffer);                                     memory pointer to string
    void  stdr_sz_t2hex(stdr_size_t value, char* hex, stdr_size_t hex_size);         size_t to hex
    void  stdr_sz_t2str(stdr_size_t value, char* buffer);                            size_t to string
    char* stdr_str2c(const char* str, int c);                                        string to char
    long  stdr_str2l(const char* str, char** endptr, int base);                      string to long
    void  stdr_ui2hex(unsigned int value, char* buffer);                             uint to hex
    void  stdr_ui2str(stdr_size_t value, char* buffer);                              uint to string
    void  stdr_ul2hex(unsigned long value, char* buffer);                            ulong to hex
    void  stdr_ul2str(unsigned long value, char* buffer);                            ulong to string
    void  stdr_uc2str(unsigned char value, char* buffer);                            uchar to string
    void  stdr_us2str(unsigned short value, char* buffer);                           ushort to string


    There is also this:


    void stdr_str_out(const char* message);     - outputs string to console using
                                                  the kernel32.dll WriteConsoleA()


**-- HEADER --**


    The following macros are defined and replace the ones of the standard library:


    STDR_NULL                                                          replaces NULL
    stdr_offsetof()                                                    replaces offsetof()


    An (incomplete) implemetation of variadic definitions:


    stdr_va_list                                                       replaces va_list
    stdr_va_start                                                      replaces va_start
    stdr_va_arg                                                        replaces va_arg
    stdr_va_end                                                        replaces va_end


    For more information, browse stdr.h


**-- WIN32 API LOW-LEVEL FUNCTIONS --**


    These low-level functions are availble for use. Check the header for compatible data types:


    __GetModuleHandleA()
    __GetProcAddress()
    __GetStdHandle()
    __SetFilePointer()
    __FlushFileBuffers()
    __CreateFileA()
    __ReadFile()
    __WriteFile()
    __GetFileSize()
    __CloseHandle()
    __VirtualAlloc()
    __LocalFree()
    __FormatMessageA()
    __WriteConsoleA()
    __MAKELANGID()
    __GetLastError()
    __ExitProcess()


**--CODE--**


    The included drop-in replacement functions:


    math.h:
    -----------------------------------------------------------------------------------------------------------


        float stdr_asin(float x);
        float stdr_atan2(float y, float x);
        float stdr_cos(float x);
        float stdr_fabs(float x);
        float stdr_pow(float base, int exp);
        float stdr_sin(float x);
        float stdr_sqrt(float x);
        float stdr_tanf(float x);


    stdio.h:
    -----------------------------------------------------------------------------------------------------------


        * WIN_VOID   stdr_fclose(WIN_FILE* stream)
        * WIN_INT    stdr_feof(WIN_FILE* stream);
        * WIN_INT    stdr_fgetc(WIN_FILE* stream);
        * WIN_PCHR   stdr_fgets(WIN_PCHR str, WIN_INT n, WIN_FILE* stream);
        * WIN_INT    stdr_fprintf(WIN_HANDLE file, WIN_PCCHR format, ...);
        * WIN_INT    stdr_fscanf(WIN_FILE* stream, WIN_PCCHR format, ...);
        * WIN_FILE*  stdr_fopen(WIN_PCCHR filename, WIN_PCCHR mode);
        * WIN_SIZE_T stdr_fread(WIN_PVOID ptr, WIN_SIZE_T size, WIN_SIZE_T count, WIN_FILE* stream);
        * WIN_INT    stdr_fseek(WIN_FILE* stream, long offset, int whence);
        * WIN_LONG   stdr_ftell(WIN_FILE* stream);
        * WIN_SIZE_T stdr_fwrite(WIN_PCVOID ptr, WIN_SIZE_T size, WIN_SIZE_T count, WIN_FILE* stream);
        * WIN_VOID   stdr_perror(WIN_PCCHR s);
          int        stdr_printf(const char* format, ...);
        * WIN_VOID   stdr_rewind(WIN_FILE* stream);
          int        stdr_sprintf(char* buffer, const char* format, ...);
          int        stdr_sscanf_hex(const char* str, unsigned long* value);
          int        stdr_vsnprintf(char* buffer, stdr_size_t count, const char* format, stdr_va_list args);


    stdlib.h:
    -----------------------------------------------------------------------------------------------------------


          void*       stdr_calloc(stdr_size_t num, stdr_size_t size);
          void        stdr_exit(int status);
          void        stdr_free(void* ptr);
        * WIN_PVOID   stdr_malloc(WIN_SIZE_T size);
          stdr_size_t stdr_malloc_block_size(void* ptr);
          void*       stdr_realloc(void* ptr, stdr_size_t new_size);


    string.h:
    -----------------------------------------------------------------------------------------------------------


        void*       stdr_memcpy(void* dst, const void* src, stdr_size_t n);
        int         stdr_memcmp(const void* s1, const void* s2, stdr_size_t n);
        void*       stdr_memset(void* s, int c, stdr_size_t n);
        char*       stdr_strcat(char* dst, const char* src);
        char*       stdr_strchr(const char* str, int c);
        int         stdr_strcmp(const char* str1, const char* str2);
        char*       stdr_strcpy(char* dst, const char* src);
        char*       stdr_strdup(const char* str);
        stdr_size_t stdr_strlen(const char* str);
        int         stdr_strncmp(const char* s1, const char* s2, stdr_size_t n);
        char*       stdr_strrchr(const char* str, int c);
        char*       stdr_strstr(const char* haystack, const char* needle);


    unistd.h:
    -----------------------------------------------------------------------------------------------------------


        * WIN_SIZE_T stdr_read(WIN_PVOID ptr, WIN_SIZE_T count, WIN_FILE* stream);


    wchar.h:
    -----------------------------------------------------------------------------------------------------------


        stdr_size_t stdr_wcslen(const stdr_wchar_t* s);
        stdr_size_t stdr_wcstombs(char* dst, const stdr_wchar_t* src, stdr_size_t max);


    -----------------------------------------------------------------------------------------------------------


    * = Windows only


**-- U MAD, BRO? WHY NOT JUST USE THE STANDARD LIBRARY? --**


    My goal is to generate as small executables as possible. When making demo scene productions,
    there are sometimes rules in demo- and intro competitions that limit the size a demo scene
    production can be.

    A few common size limitations can be 64kb, 40kb, and 4kb. This is the total size a self contained
    executable is allowed to be, excluding api calls accessible directly in a newly installed
    operating system.

    These size limitations will sometimes force demo scene developers to be creative, and this is one
    of the creative ways I aim to utilize for this challenge!


**-- USAGE --**


    As these are drop-in replacements, just use them in the same way you would use the standard library
    functions. Just copy any definitions and functions you want to use with your code and go from there.


    To try this out you can build it like so:

            gcc -o stdr.exe stdr.c -m32 -nostdlib -lkernel32 "-Wl,-e,__start"

    I have included some test cases in main_entry(), but you can of course add more if you wish to try
    various functions out.


    There are tons of size related optimization flags that can be used with gcc, but be careful using
    the -flto flag with this one as it can possibly break the variadic functions. It's a sneaky one
    and can appear to work under certain conditions but will likely make kernel32.dll segfault under
    others. There is no error handling here if api call arguments would be null when they shouldn't.
    You have been warned.


    As the scope is focused on size optimization over efficiency, I strongly recommend you to do your
    own benchmark tests to compare these with stdlib


**-- Are these feature complete? --**


    As I have only included the functionality I need for my current project, some things may be missing
    or lacking compared to their standard library relatives. My goal here is not to make a full
    replacement to the standard library.


**License**


    This project is released under the MIT license. If you are a demo scener and choose to use any of
    this code in your productions, additional conditions apply as follows: Greetings to dMG and the mighty
    crews of diViNE sTYLERs, UP ROUGH, and TiTAN in your production! :D


Cheerios!
