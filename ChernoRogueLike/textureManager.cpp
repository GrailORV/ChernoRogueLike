//=============================================================================
//
// �e�N�X�`���Ǘ����� [textureManager.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include <vector>
#include "textureManager.h"
#include "renderer.h"
#include "WinApp.h"

//=============================================================================
// CTextureManager�R���X�g���N�^
//=============================================================================
CTextureManager::CTextureManager() :m_dwRef(0)
{
}

//=============================================================================
// CTextureManager�f�X�g���N�^
//=============================================================================
CTextureManager::~CTextureManager()
{
	Release();
}

//=============================================================================
// �Q�ƃR�s�[
//=============================================================================
HRESULT CTextureManager::QueryInterface(REFIID riid, void FAR* FAR* ppvObject)
{
	if (IsEqualIID(riid, IID_IUnknown))
	{
		*ppvObject = this;
		AddRef();
		return NOERROR;
	}
	return E_NOINTERFACE;
}

//=============================================================================
// �Q�ƃJ�E���^�C���N�������g
//=============================================================================
ULONG CTextureManager::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// �I������
//=============================================================================
ULONG CTextureManager::Release(void)
{
	if (--m_dwRef == 0)
	{
		delete this;
		return 0;
	}
	return m_dwRef;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTextureManager::Init(void)
{

	return S_OK;
}

//=============================================================================
// �V�[�����Ƃ̃e�N�X�`���ǂݍ���
//=============================================================================
void CTextureManager::LoadSceneTex(int mode)
{
	std::vector<std::string> fileList;
	HANDLE hFind;
	WIN32_FIND_DATA fd;

	std::string searchStr = "data/TEXTURE/*.*";

	hFind = FindFirstFile(searchStr.c_str(), &fd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do {
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			std::string fileName = "data/TEXTURE/";
			fileName += fd.cFileName;
			fileList.push_back(fileName);
		}
	} while (FindNextFile(hFind, &fd));

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	for (UINT nCntTex = 0; nCntTex < fileList.size(); nCntTex++)
	{
		std::string texID = fileList[nCntTex];
		texID.erase(texID.find('.'));
		texID.erase(0, 13);
		m_texData.insert(std::make_pair(texID, nullptr));

		HRESULT hr = D3DXCreateTextureFromFile(
			pDevice,
			fileList[nCntTex].c_str(),
			m_texData[texID].GetAddressOf()
		);
		if (FAILED(hr))
		{
			m_texData[texID].Reset();
		}
	}
}

//=============================================================================
// �e�N�X�`���̌Ăяo��
//=============================================================================
void CTextureManager::BindtextureFromString(std::string texID, IDirect3DTexture9** ppTex)
{
	m_texData[texID].Get()->QueryInterface(IID_IUnknown, (void**)ppTex);
}


//=============================================================================
// �f�o�C�X���X�g��
//=============================================================================
void CTextureManager::OnLostDevice(void)
{
	for each (std::pair<std::string, ComPtr<IDirect3DTexture9>> p in m_texData)
	{
		p.second.Reset();
	};

	m_texData.clear();
}
