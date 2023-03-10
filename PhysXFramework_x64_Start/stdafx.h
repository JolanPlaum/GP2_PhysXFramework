// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here

// disable min and max to allow use of algorithm versions (before including windows.h)
//PhysX:you must suppress its definition of the macros min and max, as these are common C++ method names in the SDK
#define NOMINMAX

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <wchar.h>
#include <crtdbg.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <vector>
#include <algorithm>
#include <codeanalysis\warnings.h>


#pragma region
//Direct3D Header Files:
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

//DirectXMath Header Files:
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
using namespace DirectX;

//**FX11 (Helper for loading Effects (D3DX11))
//https://github.com/microsoft/FX11
#include <d3dx11effect.h>
#pragma comment(lib, "Effects11.lib")


//*DirectXTex (Helper for loading Textures (D3DX11))
//https://github.com/microsoft/DirectXTex
#pragma warning(push)
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include <DirectXTex.h>
#pragma warning(pop)
#pragma comment(lib, "DirectXTex.lib")
#pragma endregion DIRECTX INCLUDES


//XInput >> Xbox Controller support
#include <Xinput.h>
#pragma comment(lib, "XINPUT9_1_0.LIB")

//NVIDIA PhysX
//https://gameworksdocs.nvidia.com/PhysX/4.0/documentation/PhysXGuide/Index.html
#pragma region
#pragma warning(push)
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include <PxPhysicsAPI.h>
#pragma warning(pop)
using namespace physx;

#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")

#pragma endregion PHYSX INCLUDES

//FMOD (v1.10)
//https://www.fmod.com/
#pragma region
#pragma warning(push)
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include "fmod.hpp" //Precompiled Header
#include "fmod_errors.h"
#pragma warning(pop)

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "fmodL64_vc.lib")
#else
#pragma comment(lib, "fmod64_vc.lib")
#endif
#pragma endregion FMOD INCLUDES


// the one that holds the D3D11 data
#include "Locator.h"


#pragma region
//*****************************************************************************
//Declare templates for releasing interfaces and deleting objects 
//*****************************************************************************
template<class Interface>
inline void SafeRelease(Interface& pInterfaceToRelease)
{
	if (pInterfaceToRelease != 0)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = 0;
	}
}
template<class T>
inline void SafeDelete(T& pObjectToDelete)
{
	if (pObjectToDelete != nullptr)
	{
		delete(pObjectToDelete);
		pObjectToDelete = nullptr;
	}
}

template<typename T>
inline void Clamp(T& value, T hi, T lo)
{
	if (value > hi)
		value = hi;

	if (value < lo)
		value = lo;
}
#pragma endregion TEMPLATES & MACROS