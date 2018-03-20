//=============================================================================
//
// �Q�[�����[�h�����̌p���p�N���X [mode.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "stdafx.h"
#include "scene.h"

//*********************************************************
// �Q�[�����[�h�����N���X
//*********************************************************
class CMode : public CScene
{
public:
	CMode(std::string ModeName);
	virtual ~CMode();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void OnLostDevice(void) = 0;

	void SetPosition(Vector3 pos) {}
	Vector3 GetPosition(void) { return Vector3(0.0f, 0.0f, 0.0f); }

	void SetRotation(Vector3 pos) {}
	Vector3 GetRotation(void) { return Vector3(0.0f, 0.0f, 0.0f); }

	std::string GetModeName() { return m_name; }

protected:
	const std::string m_name;

};