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
#include <memory>
#include <string>
#include <wrl/client.h>

#define DIRECTINPUT_VERSION (0x800)
#include <dinput.h>

using namespace Microsoft::WRL;

//*****************************************************************************
// �֐��ʖ���`
//*****************************************************************************
using Vector2 = D3DXVECTOR2;
using Vector3 = D3DXVECTOR3;
using Vector4 = D3DXVECTOR4;
using Matrix = D3DXMATRIX;
using Quaternion = D3DXQUATERNION;

using Color = D3DCOLOR;
using XColor = D3DXCOLOR;

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

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw hr;
	}
}