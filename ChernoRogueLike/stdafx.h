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

#define DIRECTINPUT_VERSION (0x800)
#include <dinput.h>

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

template <typename T>
void SafeUninitDelete(T& ptr)
{
	if (ptr)
	{
		ptr->Uninit();
		delete ptr;
		ptr = NULL;
	}
}

