//=============================================================================
//
// デバッグモードの処理 [debugMode.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "mode.h"

class CModel;

//*********************************************************
// デバッグモードクラス
//*********************************************************
class CDebugMode : public CMode
{
public:
	CDebugMode();
	~CDebugMode();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void OnLostDevice(void);

private:
	CModel* m_pModel;

};
