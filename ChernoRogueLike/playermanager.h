//=============================================================================
//
// �v���C���[�}�l�[�W���[�̏��� [playermanager.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "scene.h"
#include "player.h"

//*****************************************************************************
// �\����
//*****************************************************************************


class CPlayer;
//*********************************************************
// �v���C���[�}�l�[�W���[�N���X
//*********************************************************
class CPlayerManager
{
public:
	CPlayerManager();
	~CPlayerManager();

	HRESULT Init(void);
	void Uninit(void);	

	CPlayer *GetPlayer(void) { return m_pPlayer; }

protected:
	CPlayer * m_pPlayer;
};