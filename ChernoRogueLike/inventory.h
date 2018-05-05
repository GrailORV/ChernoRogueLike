//=============================================================================
//
// �C���x���g���̏��� [inventory.h]
// Author : Ueno
//
//=============================================================================
#pragma once

#include "main.h"
#include "scene.h"

class CScene2D;
class CItem;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_INVENTORY	10	//�C���x���g���̍ő吔
#define MAX_HEADS	2	//�A�C�e���̍ő區�ڐ�

//*********************************************************
// ���j���[�N���X
//*********************************************************
class CInventory : public CScene
{
public:
	CInventory(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	~CInventory();

	//�C���x���g���̏��
	typedef enum
	{
		STATE_START = 0,
		STATE_SELECT,
		STATE_USING,
		STATE_MAX
	}INVENTORY_STATE;

	//�C���x���g���̍���
	typedef enum
	{
		SELECT_USE = 0,
		SELECT_EQUIP,
		SELECT_DROP,
		SELECT_THROW,
		SELECT_BACK,
		SELECT_MAX
	}INVENTORY_SELECT;

	//�C���x���g���̏��(�\����)
	typedef struct
	{
		int m_nType;
		int m_nHaves;
	}Inventory;

	static CInventory *Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);

	HRESULT Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);

	HRESULT MakeVertexBuffer(void) {};

	//�e�N�X�`���̐ݒ�
	void BindTexture(const char* texID);

	//�|�W�V�����̐ݒ�E�擾
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; };
	D3DXVECTOR3 GetPosition() { return m_pos; };

	//�s�{�b�g�|�C���g�̐ݒ�E�擾
	void SetPivot(D3DXVECTOR3 pivot) { m_pivot = pivot; }
	D3DXVECTOR3 GetPivot(void) { return m_pivot; }

	//���e�[�V�����̐ݒ�E�擾
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRotation() { return m_rot; };

	//�J���[�̐ݒ�E�擾
	void SetColor(D3DXCOLOR color) { m_color = color; }
	D3DXCOLOR GetColor(void) { return m_color; }

	void SetType(int nType) { m_nType = nType; }

	//�L�[����
	void KeyControl(void);

	// �C���x���g���N���X�̏�Ԃ𒲂ׂ�
	void CheckState(void);

	//�C���x���g���̓ǂݍ���
	void LoadInventory(void);

	//�C���x���g���̍X�V
	void WriteInventory(void);

private:
	ComPtr<IDirect3DTexture9> m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	ComPtr<IDirect3DIndexBuffer9> m_pIdxBuff;

	D3DXMATRIX m_mtxWorld;		// ���[���h�}�g���b�N�X

	UINT m_column, m_row;
	UINT m_numFace, m_runIndex, m_numVertex;
	D3DXVECTOR3 m_pos;		 // �ʒu
	D3DXVECTOR3 m_pivot;	 // ��_
	D3DXVECTOR3 m_rot;		 // ����
	D3DXVECTOR3 m_move;		 // 
	D3DXVECTOR3 m_rotDest;   // 
	D3DXVECTOR4 m_uv;		 // 
	D3DXCOLOR m_color;		 // �F
	float m_fWidth;			 // ���̒���
	float m_fHeight;		 // �c�̒���
	int m_nType;			 // ���
	int m_nColumnCount;		 // ��̃J�E���g
	int m_nRowCount;		 // �s�̃J�E���g
	int m_nInventoryIndex;	 // �C���x���g���̃J�[�\���ԍ�

	Inventory m_inventory[MAX_INVENTORY]; //�C���x���g���̏��
	INVENTORY_SELECT m_inventorySelect;	  //�C���x���g���̍���
	INVENTORY_STATE  m_inventoryState;	  //�C���x���g���̏��

	CScene2D* m_pInventoryWindow;			// �C���x���g���g
	CScene2D* m_pItemMenuWIndow;			// �A�C�e���̘g
	CItem* m_pItem;
	static const char *m_apTextureName[];	// �e�N�X�`��
};
