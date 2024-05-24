/*--------------------------------------*/
/*| STANDARD LIBRARY REPLACEMENTS INIT |*/
/*--------------------------------------*/

#define STDR_NULL ((void *)0)
#define stdr_offsetof(type, member) ((stdr_size_t) &((type *)0)->member)

#define STDR_MALLOC_BLOCK_SIZE (sizeof(stdr_MallocBlock) - sizeof(char))

/* variadic */
typedef char* stdr_va_list;
#define stdr_va_start(ap, param) (ap = (char*)&param + sizeof(param))
#define stdr_va_arg(ap, type) (*(type*)((ap += sizeof(type)) - sizeof(type)))
#define stdr_va_end(ap) (ap = STDR_NULL)

/* math constants */
#define STDR_PI 3.14159265358979323846
#define STDR_HALF_PI 1.57079632679489661923
#define STDR_TWO_PI 6.28318530717958647692

typedef enum { stdr_false, stdr_true } stdr_bool;
typedef unsigned int stdr_size_t;
typedef unsigned int stdr_uintptr_t;
typedef unsigned long stdr_uint32_t;
typedef unsigned short stdr_wchar_t;

typedef struct stdr_MallocBlock {
    stdr_size_t size;
    struct stdr_MallocBlock* next;
    int free;
    char data[1];
} stdr_MallocBlock;
stdr_MallocBlock* freeList = STDR_NULL;

/*--------------------*/
/*| WINDOWS SPECIFIC |*/
/*--------------------*/

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#define WIN_FALSE 0
#define WIN_TRUE 1
#define WIN_NO_ERROR 0

#define WIN_MEM_COMMIT 0x1000
#define WIN_MEM_RESERVE 0x2000
#define WIN_PAGE_READWRITE 0x04

#define WIN_STD_OUTPUT_HANDLE ((WIN_DWORD)-11)
#define WIN_INVALID_HANDLE_VALUE ((WIN_HANDLE)(WIN_PLONG)-1)

#define WIN_FILE_BEGIN 0
#define WIN_FILE_CURRENT 1
#define WIN_FILE_END 2
#define WIN_FILE_SHARE_READ 1
#define WIN_FILE_ATTRIBUTE_NORMAL 0x80
#define WIN_INVALID_SET_FILE_POINTER ((WIN_SIZE_T)-1)

#define WIN_SEEK_SET 0
#define WIN_SEEK_CUR 1
#define WIN_SEEK_END 2
#define WIN_EOF (-1)

#define WIN_GENERIC_READ 0x80000000
#define WIN_GENERIC_WRITE 0x40000000

#define WIN_CREATE_ALWAYS 2
#define WIN_OPEN_ALWAYS 4
#define WIN_OPEN_EXISTING 3

#define WIN_FORMAT_MESSAGE_IGNORE_INSERTS 0x00000200
#define WIN_FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#define WIN_FORMAT_MESSAGE_FROM_SYSTEM 0x00001000

#define WIN_SUBLANG_DEFAULT 1
#define WIN_LANG_NEUTRAL 0

typedef char WIN_CHR;
typedef char* WIN_PCHR;
typedef const char* WIN_PCCHR;
typedef unsigned char WIN_UCHR;
typedef int WIN_INT;
typedef int WIN_BOOL;
typedef unsigned int WIN_SIZE_T;
typedef unsigned int WIN_UINT;
typedef unsigned short WIN_WORD;
typedef void WIN_VOID;
typedef void* WIN_PVOID;
typedef void* WIN_HANDLE;
typedef const void* WIN_PCVOID;
typedef long WIN_LONG;
typedef long* WIN_PLONG;
typedef unsigned long WIN_DWORD;

typedef struct WIN_FILE WIN_FILE;
struct WIN_FILE {
    WIN_HANDLE hFile;
    WIN_INT fd;
};

typedef WIN_HANDLE (__stdcall *GetModuleHandleA_t)(WIN_PCCHR);
typedef WIN_PVOID (__stdcall *GetProcAddress_t)(WIN_HANDLE, WIN_PCCHR);
typedef WIN_BOOL (__stdcall *WriteConsoleA_t)(WIN_HANDLE, WIN_PCCHR, WIN_DWORD, WIN_DWORD*, WIN_PVOID);
typedef WIN_HANDLE (__stdcall *GetStdHandle_t)(WIN_DWORD);
typedef WIN_VOID (__stdcall *ExitProcess_t)(WIN_UINT);
typedef WIN_BOOL (__stdcall *FlushFileBuffers_t)(WIN_HANDLE);
typedef WIN_BOOL (__stdcall *CloseHandle_t)(WIN_HANDLE);
typedef WIN_PVOID (__stdcall *VirtualAlloc_t)(WIN_PVOID, WIN_SIZE_T, WIN_DWORD, WIN_DWORD);
typedef WIN_DWORD (__stdcall *SetFilePointer_t)(WIN_HANDLE, WIN_LONG, WIN_PLONG, WIN_DWORD);
typedef WIN_HANDLE (__stdcall *CreateFileA_t)(WIN_PCCHR, WIN_DWORD, WIN_DWORD, WIN_PVOID, WIN_DWORD, WIN_DWORD, WIN_HANDLE);
typedef WIN_BOOL (__stdcall *ReadFile_t)(WIN_HANDLE, WIN_PVOID, WIN_DWORD, WIN_DWORD*, WIN_PVOID);
typedef WIN_BOOL (__stdcall *WriteFile_t)(WIN_HANDLE, WIN_PCVOID, WIN_DWORD, WIN_DWORD*, WIN_PVOID);
typedef WIN_DWORD (__stdcall *GetFileSize_t)(WIN_HANDLE, WIN_DWORD*);
typedef WIN_HANDLE (__stdcall *LocalFree_t)(WIN_HANDLE);
typedef WIN_DWORD (__stdcall *GetLastError_t)(WIN_VOID);
typedef WIN_WORD (__stdcall *MAKELANGID_t)(WIN_WORD, WIN_WORD);
typedef WIN_DWORD (__stdcall *FormatMessageA_t)(WIN_DWORD, WIN_PCVOID, WIN_DWORD, WIN_DWORD, WIN_PCHR, WIN_DWORD, stdr_va_list*);

__declspec(dllimport) WIN_HANDLE __stdcall GetModuleHandleA(WIN_PCCHR lpModuleName);
__declspec(dllimport) WIN_PVOID __stdcall GetProcAddress(WIN_HANDLE hModule, WIN_PCCHR lpProcName);

GetModuleHandleA_t __GetModuleHandleA;
GetProcAddress_t __GetProcAddress;
GetStdHandle_t __GetStdHandle;
SetFilePointer_t __SetFilePointer;
FlushFileBuffers_t __FlushFileBuffers;
CreateFileA_t __CreateFileA;
ReadFile_t __ReadFile;
WriteFile_t __WriteFile;
GetFileSize_t __GetFileSize;
CloseHandle_t __CloseHandle;
VirtualAlloc_t __VirtualAlloc;
LocalFree_t __LocalFree;
FormatMessageA_t __FormatMessageA;
WriteConsoleA_t __WriteConsoleA;
MAKELANGID_t __MAKELANGID;
GetLastError_t __GetLastError;
ExitProcess_t __ExitProcess;

void bootstrap_win32_api() {
    /* Bang win32 api functions without includes */

    WIN_HANDLE kernel32 = GetModuleHandleA("kernel32.dll");
    __GetProcAddress = (GetProcAddress_t)GetProcAddress(kernel32, "GetProcAddress");
    __GetStdHandle = (GetStdHandle_t)__GetProcAddress(kernel32, "GetStdHandle");
    __SetFilePointer = (SetFilePointer_t)__GetProcAddress(kernel32, "SetFilePointer");
    __FlushFileBuffers = (FlushFileBuffers_t)__GetProcAddress(kernel32, "FlushFileBuffers");
    __CreateFileA = (CreateFileA_t)__GetProcAddress(kernel32, "CreateFileA");
    __ReadFile = (ReadFile_t)__GetProcAddress(kernel32, "ReadFile");
    __WriteFile = (WriteFile_t)__GetProcAddress(kernel32, "WriteFile");
    __GetFileSize = (GetFileSize_t)__GetProcAddress(kernel32, "GetFileSize");
    __CloseHandle = (CloseHandle_t)__GetProcAddress(kernel32, "CloseHandle");
    __VirtualAlloc = (VirtualAlloc_t)__GetProcAddress(kernel32, "VirtualAlloc");
    __LocalFree = (LocalFree_t)__GetProcAddress(kernel32, "LocalFree");
    __FormatMessageA = (FormatMessageA_t)__GetProcAddress(kernel32, "FormatMessageA");
    __MAKELANGID = (MAKELANGID_t)__GetProcAddress(kernel32, "MAKELANGID");
    __WriteConsoleA = (WriteConsoleA_t)__GetProcAddress(kernel32, "WriteConsoleA");
    __GetLastError = (GetLastError_t)__GetProcAddress(kernel32, "GetLastError");
    __ExitProcess = (ExitProcess_t)__GetProcAddress(kernel32, "ExitProcess");
}
