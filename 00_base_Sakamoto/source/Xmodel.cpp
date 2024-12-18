//============================================
//
//	Xモデルの管理 [Xmodel.cpp]
//	Author:sakamoto kai
//
//============================================
#include "XModel.h"
#include "renderer.h"
#include "manager.h"

//静的メンバ変数宣言
int CXModel::m_NumAll = 0;

char CXModel::c_apModelname[MAX_MODEL][128] = {};

//====================================================================
//コンストラクタ
//====================================================================
CXModel::CXModel()
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		m_apXModel[nCntModel].m_pBuffMat = nullptr;
		m_apXModel[nCntModel].m_dwNumMat = NULL;
		m_apXModel[nCntModel].m_pMesh = nullptr;
	}
}

//====================================================================
//デストラクタ
//====================================================================
CXModel::~CXModel()
{

}

//====================================================================
//読み込み
//====================================================================
HRESULT CXModel::Load(void)
{
	////デバイスの取得
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	//{

	//	//Xファイルの読み込み
	//	if (FAILED(D3DXLoadMeshFromX(c_apModelname[nCntModel],
	//		D3DXMESH_SYSTEMMEM,
	//		pDevice,
	//		nullptr,
	//		&m_apXModel[nCntModel].m_pBuffMat,
	//		nullptr,
	//		&m_apXModel[nCntModel].m_dwNumMat,
	//		&m_apXModel[nCntModel].m_pMesh)))
	//	{
	//		if (c_apModelname[nCntModel] == nullptr)
	//		{
	//			m_NumAll = nCntModel;
	//			return S_OK;
	//		}
	//		else
	//		{
	//			return E_FAIL;
	//		}
	//	}
	//}

	return S_OK;
}

//====================================================================
//破棄
//====================================================================
void CXModel::Unload(void)
{
	for (int nCntModel = 0; nCntModel < m_NumAll; nCntModel++)
	{
		//バッファの破棄
		if (m_apXModel[nCntModel].m_pBuffMat != nullptr)
		{
			m_apXModel[nCntModel].m_pBuffMat->Release();
			m_apXModel[nCntModel].m_pBuffMat = nullptr;
		}

		//メッシュの破棄
		if (m_apXModel[nCntModel].m_pMesh != nullptr)
		{
			m_apXModel[nCntModel].m_pMesh->Release();
			m_apXModel[nCntModel].m_pMesh = nullptr;
		}
	}
}

//====================================================================
//Xモデルの番号指定
//====================================================================
int CXModel::Regist(const char *pFilename)
{
	for (int nCntModel = 0; nCntModel < m_NumAll; nCntModel++)
	{
		if (strcmp(&c_apModelname[nCntModel][0], pFilename) == 0)
		{
			return nCntModel;
		}
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//Xファイルの読み込み
	if (SUCCEEDED(D3DXLoadMeshFromX(pFilename,
		D3DXMESH_SYSTEMMEM,
		m_pDevice,
		nullptr,
		&m_apXModel[m_NumAll].m_pBuffMat,
		nullptr,
		&m_apXModel[m_NumAll].m_dwNumMat,
		&m_apXModel[m_NumAll].m_pMesh)))
	{
		if (strcmp(&c_apModelname[m_NumAll][0], "") == 0)
		{
			strcpy(&c_apModelname[m_NumAll][0], pFilename);
			//c_apModelname[m_NumAll][0] = *pFilename;
			m_NumAll++;
			return m_NumAll - 1;
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

//====================================================================
//指定のテクスチャを返す
//====================================================================
void CXModel::GetXModelInfo(LPD3DXBUFFER *pBuffMat, DWORD *dwNumMat, LPD3DXMESH *pMesh, int nIdx)
{
	*pBuffMat = m_apXModel[nIdx].m_pBuffMat;
	*dwNumMat = m_apXModel[nIdx].m_dwNumMat;
	*pMesh = m_apXModel[nIdx].m_pMesh;
}