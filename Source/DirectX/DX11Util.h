/**
 * @file DX11Util.h
 * @brief DX11”Ä—p
 * @author ryota.ssk
 */
#ifndef __DX11_UTIL_H__
#define __DX11_UTIL_H__

#include <d3d11.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")


#if (_WIN64)
#if (WINVER == _WIN32_WINNT_WIN10)
#if (_MSC_VER >= 1910) // VS2017
#if (_DEBUG)
#pragma comment(lib, "DirectXTex\\DirectXTex_vs2017_Win10d.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex_vs2017_Win10.lib")
#endif // _DEBUG
#elif(_MCS_VER >= 1900) // VS2015
#if (_DEBUG)
#pragma comment(lib, "DirectXTex\\DirectXTex_vs2015_Win10d.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex_vs2015_Win10.lib")
#endif // _DEBUG
#endif // _MSC_VER
#else
#if (_MSC_VER >= 1910) // VS2017
#if (_DEBUG)
#pragma comment(lib, "DirectXTex\\DirectXTex_vs2017d.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex_vs2017.lib")
#endif // _DEBUG
#elif(_MSC_VER >= 1900) // VS2015
#if (_DEBUG)
#pragma comment(lib, "DirectXTex\\DirectXTex_vs2015d.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex_vs2015.lib")
#endif // _DEBUG
#endif // _MSC_VER
#endif // _WIN32_WINNT_WIN10
#else
#if (_DEBUG)
#pragma comment(lib, "DirectXTex\\DirectXTex_vs2017_x86d.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex_vs2017_x86.lib")
#endif // _DEBUG
//#error "x86‚ÌDirectXTex.lib‚ð—pˆÓ‚µ‚Ä‚­‚¾‚³‚¢"
#endif // _WIN64


#define SAFE_RELEASE(p) do{ if(p) { p->Release(); p = NULL; } }while(0)
#define SAFE_DELETE(p) do { if(p) { delete p; p = NULL; } } while(0)
#define SAFE_DELETE_ARRAY(p) do { if(p) { delete[] p; p = NULL; } } while(0)

struct D3DCOLORVALUEU
{
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
};


class DX11Graphics;
class DX11Light;

//! @name Get function
/// @{
ID3D11Device *GetDX11Device();
ID3D11DeviceContext *GetDX11DeviceContext();
DX11Graphics *GetDX11Graphics();
DX11Light *GetDX11Light();
/// @}


HRESULT DX11Init(HWND hWnd, UINT width, UINT height, bool fullscreen);
void DX11Uninit();
void DX11BeforeRender(const float ClearColorRGBA[]);
void DX11AfterRender();


#endif // __DX11_UTIL_H__