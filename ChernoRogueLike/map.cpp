//=============================================================================
//
// 3D�I�u�W�F�N�g�̏��� [scene3D.cpp]
// Author : 
//
//=============================================================================
#include "stdafx.h"

#include "map.h"
#include "manager.h"
#include "WinApp.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �ÓI�ϐ�
//*****************************************************************************
MapLocation CMap::m_respawnPoint(1, 1);
uint8_t** CMap::m_mapState = nullptr;
uint16_t CMap::m_mapMaxX = 3;
uint16_t CMap::m_mapMaxZ = 3;

//=============================================================================
// CPlane����
//=============================================================================
CMap *CMap::Create(const uint16_t MapSizeX, const uint16_t MapSizeZ)
{
	CMap *pMap;

	pMap = new CMap;
	pMap->Init(MapSizeX, MapSizeZ);

	return pMap;
}

//=============================================================================
// CPlane�R���X�g���N�^
//=============================================================================
CMap::CMap()
{
}

//=============================================================================
// CPlane�f�X�g���N�^
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMap::Init(const uint16_t MapSizeX, const uint16_t MapSizeZ)
{
	if (MapSizeX < 3)
	{
		m_mapMaxX = 3;
	}
	else
	{
		m_mapMaxX = MapSizeX;
	}

	if (MapSizeZ < 3)
	{
		m_mapMaxZ = 3;
	}
	else
	{
		m_mapMaxZ = MapSizeZ;
	}

	m_mapState = new uint8_t*[m_mapMaxX];
	for (UINT x = 0; x < m_mapMaxX; x++)
	{
		m_mapState[x] = new uint8_t[m_mapMaxZ];
		for (UINT z = 0; z < m_mapMaxZ; z++)
		{
			/*if (x == 0 || x == m_mapMaxX - 1 ||
				z == 0 || z == m_mapMaxZ - 1)
			{
				m_mapState[x][z] = 1;
			}
			else if (x == m_mapMaxX / 2 && z == m_mapMaxZ / 2)
			{
				m_mapState[x][z] = 4;
			}
			else
			{
				m_mapState[x][z] = 0;
			}*/
		}
	}

	m_respawnPoint = MapLocation(m_mapMaxX / 1.5, m_mapMaxZ / 1.5);

	m_fileName[0] = "data/MAP/TestMapAllIn.csv";

	LoadMapText(m_fileName[0]);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMap::Uninit(void)
{
	for (UINT x = 0; x < m_mapMaxX; x++)
	{
		if (!m_mapState[x])
		{
			continue;
		}
		delete[] m_mapState[x];
		m_mapState[x] = nullptr;
	}
	if (m_mapState)
	{
		delete[] m_mapState;
		m_mapState = nullptr;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CMap::Update(void)
{
	CDebugProc::Print("�}�b�v\n");
	for (UINT z = 0; z < m_mapMaxZ; z++)
	{
		for (UINT x = 0; x < m_mapMaxX; x++)
		{
			CDebugProc::Print("%d ", m_mapState[x][z]);
		}
		CDebugProc::Print("\n");
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMap::Draw(void)
{

}

//=============================================================================
// ���P�[�V��������}�b�v��Ԏ擾
//=============================================================================
uint8_t CMap::GetMapStateFromLocation(_In_ uint16_t x, _In_ uint16_t z)
{
	if (x >= m_mapMaxX || z >= m_mapMaxZ)
	{
		return 0;
	}

	return m_mapState[x][z];
}

//=============================================================================
// ���P�[�V��������}�b�v��ԃZ�b�g
//=============================================================================
void CMap::SetMapStateFromLocation(_In_ uint16_t x, _In_ uint16_t z, _In_  uint8_t state)
{
	if (x >= m_mapMaxX || z >= m_mapMaxZ)
	{
		return;
	}

	m_mapState[x][z] = state;
}

//=============================================================================
// �}�b�v�ǂݍ���
//=============================================================================
void CMap::LoadMapText(const char *FileName)
{
	// �t�@�C���\����
	FILE *fp;

	// �t�@�C���I�[�v��
	fopen_s(&fp, FileName, "r");

	if (!fp)
	{
		MessageBox(NULL, "�}�b�v�f�[�^��ǂݍ��߂܂���ł����B\n", NULL, MB_OK);
		return;
	}

	for (int nCntMapY = 0; nCntMapY < m_mapMaxZ; nCntMapY++)
	{
		for (int nCntMapX = 0; nCntMapX < m_mapMaxX; nCntMapX++)
		{
			fscanf_s(fp, "%d,", &m_MapState);
			m_mapState[nCntMapX][nCntMapY] = m_MapState;
		}
	}

	fclose(fp);
}