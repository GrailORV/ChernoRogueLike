//=============================================================================
//
// �f�o�b�O���[�h�̏��� [debugMode.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "mode.h"

class CModel;

//*********************************************************
// �f�o�b�O���[�h�N���X
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
