//=============================================================================
//
// �f�o�b�O���� [debugproc.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �f�o�b�O�����N���X
//*****************************************************************************
class CDebugProc
{
private:
	static const u_int LENGTH_STRING_BUFF = 1024;
	static const DWORD FVF_DEBUG_PROC = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	struct VertexDebug
	{
		D3DXVECTOR3 position;
		D3DCOLOR color;
	};

public:
	CDebugProc();
	~CDebugProc();

	void Init(void);
	void Uninit(void);
	void Update(void);

	static void Print(char *fmt, ...);
	static void Draw(void);

	static bool IsEnableDisp(void) { return m_bDisp; }
	static void EnableDisp(bool bDisp) { m_bDisp = bDisp; }

private:
	static ComPtr<ID3DXFont> m_pFont;				// �t�H���g�ւ̃|�C���^
	static char m_aStr[LENGTH_STRING_BUFF];	// �f�o�b�O�\���̃o�b�t�@
	static const float m_fWidth;
	static const float m_fHeight;


	static ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;

	static bool m_bDisp;					// �f�o�b�O�\��ON/OFF

	static void MakeVertexBuffer(void);
};
