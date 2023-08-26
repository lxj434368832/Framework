#pragma once

#define BEGIN_NAMESPACE namespace mqwUtils{
#define END_NAMESPACE }

//cross plateform
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
#  include <windows.h>
#  include <Shlwapi.h>
#  ifndef EXPORTAPI
#    define EXPORTAPI STDAPI
#    define EXPORTAPI_(type) STDAPI_(type)
#  endif
#else
#include <cstdint>
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned char       BOOLEAN;
typedef char                CHAR;
typedef unsigned char       UCHAR;
typedef unsigned char*		PUCHAR;
typedef short               SHORT;
typedef unsigned short      USHORT;
typedef unsigned short*		PUSHORT;
typedef long                LONG;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef unsigned long*		LPDWORD;
typedef unsigned long       ULONG;
typedef long long           LONGLONG;
typedef unsigned long long  ULONGLONG;
typedef ULONGLONG*			PULONGLONG;
typedef unsigned long       SIZE_T;
typedef LONGLONG            USN;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int*		PUINT;
typedef int16_t				INT16;
typedef int32_t				INT32;
typedef int64_t				INT64;
typedef int32_t				HRESULT;
typedef uint16_t			UINT16;
typedef	uint32_t			UINT32;
typedef uint64_t			UINT64;
typedef intptr_t			INT_PTR;
typedef uintptr_t			UINT_PTR;
typedef float               FLOAT;
typedef float*				PFLOAT;
typedef char*				LPSTR;
typedef const char*			LPCSTR;
typedef wchar_t             WCHAR;
typedef wchar_t*			LPWSTR;
typedef const wchar_t*		LPCWSTR;
typedef void                VOID;
typedef void*				LPVOID;
typedef void*				HMODULE;
typedef void*				HKEY;
typedef void*				HHANDLE;

#define interface struct
#define WINAPI				__attribute__((stdcall))
#define STDAPICALLTYPE		__attribute__((stdcall))
#define STDAPI				extern "C" HRESULT __attribute__((stdcall))
#define STDAPI_(type)		extern "C" __attribute__((visibility("default"))) type __attribute__((stdcall))
#define __stdcall			__attribute__((stdcall))
#define CALLBACK			__attribute__((stdcall))
#define STDMETHODCALLTYPE	__attribute__((stdcall))
#define STDMETHODIMP_(type) type STDMETHODCALLTYPE
#define EXPORTAPI			extern "C" HRESULT __attribute__((stdcall))
#define EXPORTAPI_(type)	extern "C" __attribute__((visibility("default"))) type __attribute__((stdcall))

#define __declspec(x)
#define __cdecl
#define TRUE                true
#define FALSE               false
#define INVALID_HANDLE_VALUE   -1
#define _MAX_PATH       260 /* max. length of full pathname */
#define MAX_PATH        260


#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#include <errno.h>
#define ERROR_SUCCESS 0
#endif
//end cross plateform
