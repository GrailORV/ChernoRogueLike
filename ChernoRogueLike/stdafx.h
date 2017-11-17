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
#include <string>
#include <wrl/client.h>

#define DIRECTINPUT_VERSION (0x800)
#include <dinput.h>

using namespace Microsoft::WRL;


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

template <typename T, class C>
void SafeDelete(T& ptr, void(C::*func)())
{
	// 参照ポインタのクラス名と引数関数の帰属クラス名比較
	const type_info& tID = typeid(T);
	std::string sT = tID.name();
	const type_info& cID = typeid(C);
	std::string sC = cID.name();

	if (ptr)
	{
		(ptr->*func)();
		delete ptr;
		ptr = NULL;
	}
}

