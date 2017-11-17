//=============================================================================
//
// ���ʃC���N���[�h�p�v���R���p�C���ς݃w�b�_�[ [stdafx.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

//*****************************************************************************
// �w�b�_�t�@�C��
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
// �O���[�o���֐���`
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
	// �Q�ƃ|�C���^�̃N���X���ƈ����֐��̋A���N���X����r
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

