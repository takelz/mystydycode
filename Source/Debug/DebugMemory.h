#ifndef __DEBUG_MEMORY_H__
#define __DEBUG_MEMORY_H__

#include <new>
#include <crtdbg.h>

//-----
// É}ÉNÉç
//-----
#ifdef _DEBUG
#define NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define NEW new
#endif // _DEBUG

#define SAFE_DELETE(p) do { if(p) { delete p; p = NULL; } } while(0)
#define SAFE_DELETE_ARRAY(p) do { if(p) { delete[] p; p = NULL; } } while(0)


#endif // __DEBUG_MEMORY_H__