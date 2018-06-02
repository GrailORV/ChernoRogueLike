//=============================================================================
//
// ���j���[���� [Menu.h]
// Author : Ueno
//
//=============================================================================
#pragma once

#include "main.h"
#include "scene.h"

class CScene2D;
class CInventory;

//*********************************************************
// ���j���[�N���X
//*********************************************************
class CMenu : public CScene
{
public:
	CMenu(int nPriority = 3,OBJTYPE objType = OBJTYPE_NONE);
	~CMenu();

	//���j���[�̑I������
	typedef enum 
	{
		SELECT_ITEM = 0,
		SELECT_STATUS,
		SELECT_SHOP,
		SELECT_CLOSE,
		SELECT_TEST,
		SELECT_TEST2,
		SELECT_MAX
	}MENU_SELECT;

	//���j���[��ʂ̏��
	typedef enum
	{
		STATE_START = 0,
		STATE_SELECT,
		STATE_USING,
		STATEMAX
	}MENU_STATE;

	static CMenu *Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);

	HRESULT Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CreateMenuWindow(int nType, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	HRESULT MakeVertexBuffer(void) {};

	void BindTexture(const char* texID);

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; };
	D3DXVECTOR3 GetPosition() { return m_pos; };

	void SetPivot(D3DXVECTOR3 pivot) { m_pivot = pivot; }
	D3DXVECTOR3 GetPivot(void) { return m_pivot; }

	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRotation() { return m_rot; };

	void SetColor(D3DXCOLOR color) { m_color = color; }
	D3DXCOLOR GetColor(void) { return m_color; }

	void SetType(int nType) { m_nType = nType; }

	void KeyControl(void);

private:
	ComPtr<IDirect3DTexture9> m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	ComPtr<IDirect3DIndexBuffer9> m_pIdxBuff;

	D3DXMATRIX m_mtxWorld;		// ���[���h�}�g���b�N�X

	UINT m_column, m_row;	 // �񐔁A�s��
	UINT m_numFace, m_runIndex, m_numVertex;
	D3DXVECTOR3 m_pos;		 // �ʒu
	D3DXVECTOR3 m_pivot;	 // ��_
	D3DXVECTOR3 m_rot;		 // ����
	D3DXVECTOR3 m_move;		 // 
	D3DXVECTOR3 m_rotDest;   // 
	D3DXVECTOR4 m_uv;		 // 
	D3DXCOLOR m_color;		 // �F
	MENU_STATE m_menuState;	 // ���j���[�̏��
	float m_fWidth;			 // ���̒���
	float m_fHeight;		 // �c�̒���
	int m_nType;			 // ���
	int m_nMenuIndex;		 // ���j���[�̃J�[�\���ԍ�

	CScene2D* m_pMenuWindow;				// ���j���[�g
	CScene2D* m_apMenuSelect[SELECT_MAX];	// ���j���[�̑I����
	CScene2D* m_pMenuCursor;				// ���j���[�̑I���A�C�R��	
	CInventory* m_pInventory;				// �C���x���g��

	static const char *m_apTextureName[];	// �e�N�X�`��
};