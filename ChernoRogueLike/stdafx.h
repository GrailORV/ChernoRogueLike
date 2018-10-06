//=============================================================================
//
// 共通インクルード用プリコンパイル済みヘッダー [stdafx.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include <Windows.h>
#include <d3dx9.h>
#include <typeinfo>
#include <algorithm>
#include <memory>
#include <string>
#include <wrl/client.h>

#define DIRECTINPUT_VERSION (0x800)
#include <dinput.h>

using namespace Microsoft::WRL;

#define Vector3 D3DXVECTOR3
#define Vector2 D3DXVECTOR2
#define Matrix D3DXMATRIX
#define Quaternion D3DXQUATERNION

#define KEYBOARD_UP DIK_UP | DIK_W
#define KEYBOARD_DOWN DIK_DOWN | DIK_S
#define KEYBOARD_LEFT DIK_LEFT | DIK_A
#define KEYBOARD_RIGHT DIK_RIGHT | DIK_D

//*****************************************************************************
// グローバル関数定義
//*****************************************************************************
template <typename T>
void SafeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = NULL;
	}
}

template <typename T>
void SafeRelease(T& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}

template <typename T>
void SafeUninit(T& ptr)
{
	if (ptr)
	{
		ptr->Uninit();
		ptr = NULL;
	}
}

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw hr;
	}
}