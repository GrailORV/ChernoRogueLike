//=============================================================================
//
// ライトの処理 [light.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "stdafx.h"

//*********************************************************
// ライトクラス
//*********************************************************
class CLight
{
public:
	static const UINT NUM_LIGHT = 3;

public:
	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetLight(void);


private:
	D3DLIGHT9 m_aLight[NUM_LIGHT];		// ライト情報

};
