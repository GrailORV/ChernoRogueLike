//=============================================================================
//
// テクスチャ管理処理 [textureManager.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include <vector>
#include "textureManager.h"
#include "renderer.h"
#include "WinApp.h"

//=============================================================================
// CTextureManagerコンストラクタ
//=============================================================================
CTextureManager::CTextureManager() :m_dwRef(0)
{
}

//=============================================================================
// CTextureManagerデストラクタ
//=============================================================================
CTextureManager::~CTextureManager()
{
	Release();
}

//=============================================================================
// 参照コピー
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
// 参照カウンタインクリメント
//=============================================================================
ULONG CTextureManager::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// 終了処理
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
// 初期化処理
//=============================================================================
HRESULT CTextureManager::Init(void)
{

	return S_OK;
}

//=============================================================================
// シーンごとのテクスチャ読み込み
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
// テクスチャの呼び出し
//=============================================================================
void CTextureManager::BindtextureFromString(std::string texID, IDirect3DTexture9** ppTex)
{
	m_texData[texID].Get()->QueryInterface(IID_IUnknown, (void**)ppTex);
}


//=============================================================================
// デバイスロスト時
//=============================================================================
void CTextureManager::OnLostDevice(void)
{
	for each (std::pair<std::string, ComPtr<IDirect3DTexture9>> p in m_texData)
	{
		p.second.Reset();
	};

	m_texData.clear();
}
