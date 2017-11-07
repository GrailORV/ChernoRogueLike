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

#define DIRECTINPUT_VERSION (0x800)
#include <dinput.h>

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

