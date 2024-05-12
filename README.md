C Standard Library Drop-In Replacement Functions

Drop-in replacement functions using win32 api to replace equivalent stdlib functions, macro definitions, and a couple of typedefs. The affected headers are math.h, string.h, stdio.h, stdlib.h and stddef.h

If you aim at using that elusive -nostdlib flag in gcc, these could be for you!


HEADER:


The following macros are defined, and replace the ones in the standard library:


    __NULL                                                              replaces NULL
    __offsetof()                                                        replaces offsetof()


An (incomplete) implemetation of variadic definitions:


    __va_list                                                           replaces va_list
    __va_start                                                          replaces va_start
    __va_arg                                                            replaces va_arg
    __va_end                                                            replaces va_end


These typedef are explicitly defined to avoid having to include stddef.h:


typedef unsigned int size_t;
typedef unsigned int uintptr_t;


CODE:


The included drop-in replacement functions are as follows:


    math.h:
    -------------------------------------------------------------------------------------------------------------------


    __pow()                                                             replaces pow()
    __fabs()                                                            replaces fabs()
    __atan2()                                                           replaces atan2()
    __asin()                                                            replaces asin()
    __sin()                                                             replaces sin()
    __cos()                                                             replaces cos()


    string.h:
    -------------------------------------------------------------------------------------------------------------------


    __strlen()                                                          replaces strlen()
    __strcpy()                                                          replaces strcpy()
    __strcat()                                                          replaces strcat()
    __strcmp()                                                          replaces strcmp()
    __strdup()                                                          replaces strdup()
    __memset()                                                          replaces memset()
    __memcpy                                                            replaces memcpy()


    stdio.h:
    -------------------------------------------------------------------------------------------------------------------


    __vsnprintf()                                                       replaces vsnprintf()
    __sprintf()                                                         replaces sprintf()
    __fprintf()                                                         replaces fprintf()
    __printf()                                                          replaces printf()


    stdlib.h:
    -------------------------------------------------------------------------------------------------------------------


    __malloc()                                                          replaces malloc()
    __free()                                                            replaces free()


There are also some helper functions included:


    void __int2str(int value, char* buffer) {}                          Converts int to string.
    void __float2str(float value, char* buffer, int precision) {}       Converts float to string
    void __ptr2str(void* ptr, char* buffer) {}                          Converts memory address pointer to string
    void __str_out(const char* message) {}                              Uses win32 api to output text to console


What is the purpose of these, why not just use the standard library?


My goal is to generate as small executables as possible. When making demo scene productions, there are sometimes rules in demo- and intro competitions that limit the size a demo scene production can be. This is also why I am using the win32 api.

A few common size limitations can be 64kb, 40kb, and 4kb. This is the total size a self contained executable is allowed to be, excluding api's accessible directly in a newly installed operating system.

These size limitations will sometimes force demo scene developers to be creative, and this is one of the creative ways I aim to utilize for this challenge!


How to use them?


As these are drop-in replacements, just use them in the same way you would use the standard library functions.

You can try these functions out by building like so:

        gcc -o cstdrep.exe cstdrep.c -m32 -nostdlib -lkernel32 "-Wl,-e,__start"

and then add the appropriate function calls to main_entry().

There are tons of size related optimizations that can be used with gcc, but be careful using the -flto flag with this one as it can possibly break the variadic functions. It's a sneaky one and can appear to work under certain conditions but will likely make kernel32.dll segfault under others. You have been warned.


Are these feature complete?


As I have only included the functionality I need for my current project, some things may be lacking compared to their standard library relatives.

One example is __printf(). In its current form it only supports %i, %d, %f, %s, and %p.

Similar restrictions may or may not apply for other functions, but my goal here is not to make full replacements for the standard library.
