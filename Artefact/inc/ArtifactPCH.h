#pragma once
// System includes
#include <windows.h>

// DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

// STL includes
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

// Link library dependencies
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

// Safely release a COM object.
template <typename T>
inline void SafeRelease(T& ptr) {
	if (ptr != NULL) {
		ptr->Release();
		ptr = NULL;
	}
}
