//============================================
//
//	オブジェクトXファイル [objectX.cpp]
//	Author:sakamoto kai
//
//============================================
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
#include "Xmodel.h"
#include "game.h"
#include "objmeshField.h"
#include "texture.h"

#define POLYDON_SIZE (200.0f)

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CObjectX>* CObjectX::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CObjectX::CObjectX(int nPriority) :CObject(nPriority)
{
	m_IdxModelNumber = 0;

	for (int nCnt = 0; nCnt < 64; nCnt++)
	{
		m_IdxTextureNumber[nCnt] = -1;
	}

	m_pTexture = nullptr;
	ZeroMemory(&m_pMesh, sizeof(m_pMesh));
	ZeroMemory(&m_pBuffMat, sizeof(m_pBuffMat));
	m_dwNumMat = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scaling = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	ZeroMemory(&m_StateMat,sizeof(m_StateMat));
	m_MatColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bUseColor = false;
	m_bUseMtx = false;
	m_UseMultiMatrix = nullptr;
	//m_bMultiMatrix = false;
}

//====================================================================
//デストラクタ
//====================================================================
CObjectX::~CObjectX()
{

}

//====================================================================
//生成処理
//====================================================================
CObjectX *CObjectX::Create(const char* pModelName)
{
	CObjectX *pObjectX = nullptr;

	if (pObjectX == nullptr)
	{
		//オブジェクト3Dの生成
		pObjectX = new CObjectX();
	}

	//オブジェクトの初期化処理
	if (FAILED(pObjectX->Init(pModelName)))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pObjectX;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CObjectX::Init()
{
	return S_OK;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CObjectX::Init(const char* pModelName)
{
	//Xファイルの割り当て
	CXModel* pXModel = CManager::GetInstance()->GetXModel();
	m_IdxModelNumber = pXModel->Regist(pModelName);
	pXModel->GetXModelInfo(&m_pBuffMat, &m_dwNumMat, &m_pMesh, m_IdxModelNumber);

	// モデル情報を取得
	CXModel::XModel* pXmodel = CManager::GetInstance()->GetXModel()->GetXModel(m_IdxModelNumber);

	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//マテリアルデータへのポインタを所得する
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != nullptr)
		{
			m_IdxTextureNumber[nCntMat] = pTexture->Regist(pMat[nCntMat].pTextureFilename);
		}
	}

	SizeVtxSet();

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CObjectX>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CObjectX::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	SetDeathFlag(true);
}

//====================================================================
//nullptrにする処理
//====================================================================
void CObjectX::SetNULL(void)
{
	if (m_pTexture != nullptr)
	{
		//メモリを開放する
		delete[] m_pTexture;
		m_pTexture = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CObjectX::Update(void)
{
	// Do Nothing
}

//====================================================================
//描画処理
//====================================================================
void CObjectX::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//Xモデルの取得
	CXModel::XModel *pXmodel = CManager::GetInstance()->GetXModel()->GetXModel(m_IdxModelNumber);
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&m_mtxWorld, m_Scaling.x, m_Scaling.y, m_Scaling.z);

	if (m_bUseMtx == true)
	{
		m_mtxWorld = m_SetMtx;
	}
	else
	{
		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	}

	if (m_UseMultiMatrix != nullptr && CScene::GetInstance()->GetMode() == CScene::MODE_GAME)
	{
		//算出したマトリクスをかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			m_UseMultiMatrix);
	}
	
	//ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを所得
	m_pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを所得する
	pMat = (D3DXMATERIAL*)pXmodel->m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pXmodel->m_dwNumMat; nCntMat++)
	{
		//// Initの移動のためコメントアウト（問題起きたら修正）
		//if (pMat[nCntMat].pTextureFilename != nullptr)
		//{
		//	m_IdxTextureNumber[nCntMat] = pTexture->Regist(pMat[nCntMat].pTextureFilename);
		//}

		//マテリアルの設定
		if (m_bUseColor == true)
		{
			m_StateMat.MatD3D.Diffuse = m_MatColor;
			m_pDevice->SetMaterial(&m_StateMat.MatD3D);
		}
		else
		{
			m_pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		}

		//テクスチャの設定
		//pDevice->SetTexture(0, m_pTexture[nCntMat]);
		if (m_IdxTextureNumber[nCntMat] != -1)
		{
			m_pDevice->SetTexture(0, pTexture->GetAddress(m_IdxTextureNumber[nCntMat]));
		}
		else
		{
			m_pDevice->SetTexture(0, nullptr);
		}

		//モデル(パーツ)の描画
		pXmodel->m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	m_pDevice->SetMaterial(&matDef);
}

void CObjectX::SetUseMultiMatrix(D3DXMATRIX* Set)
{
	m_UseMultiMatrix = Set;
}

//====================================================================
//描画処理
//====================================================================
void CObjectX::Draw(float fAlpha)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//Xモデルの取得
	CXModel::XModel* pXmodel = CManager::GetInstance()->GetXModel()->GetXModel(m_IdxModelNumber);
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL* pMat;				//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_bUseMtx == true)
	{
		m_mtxWorld = m_SetMtx;
	}
	else
	{
		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	}

	//ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを所得
	m_pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを所得する
	pMat = (D3DXMATERIAL*)pXmodel->m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pXmodel->m_dwNumMat; nCntMat++)
	{
		m_StateMat.MatD3D.Diffuse.r = pMat[nCntMat].MatD3D.Diffuse.r;
		m_StateMat.MatD3D.Diffuse.g = pMat[nCntMat].MatD3D.Diffuse.g;
		m_StateMat.MatD3D.Diffuse.b = pMat[nCntMat].MatD3D.Diffuse.b;
		m_StateMat.MatD3D.Diffuse.a = fAlpha;
		m_pDevice->SetMaterial(&m_StateMat.MatD3D);

		//テクスチャの設定
		//pDevice->SetTexture(0, m_pTexture[nCntMat]);
		if (m_IdxTextureNumber[nCntMat] != -1)
		{
			m_pDevice->SetTexture(0, pTexture->GetAddress(m_IdxTextureNumber[nCntMat]));
		}
		else
		{
			m_pDevice->SetTexture(0, nullptr);
		}

		//モデル(パーツ)の描画
		pXmodel->m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	m_pDevice->SetMaterial(&matDef);
}

//====================================================================
//モデル情報の割り当て
//====================================================================
void CObjectX::SetXModelIdx(int Idx)
{
	m_IdxModelNumber = Idx;
}

//====================================================================
//モデル情報の割り当て
//====================================================================
void CObjectX::SetTextureIdx(int nCnt, int Idx)
{
	m_IdxTextureNumber[nCnt] = Idx;
}

//====================================================================
//モデル情報の割り当て
//====================================================================
void CObjectX::BindFile(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pdwNumMat)
{
	////デバイスの取得
	//LPDIRECT3DDEVICE9 pDevice =CManager::GetInstance()->GetRenderer()->GetDevice();
	//D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	////情報の割り当て
	//m_pMesh = pMesh;
	//m_pBuffMat = pBuffMat;
	//m_dwNumMat = pdwNumMat;

	////Xモデルの取得
	//CXModel::XModel *pXmodel =CManager::GetInstance()->GetXModel()->GetXModel(GetIdxXModel());

	////テクスチャのメモリを確保
	//if (m_pTexture == nullptr)
	//{
	//	//メモリを確保する
	//	m_pTexture = new LPDIRECT3DTEXTURE9[pXmodel->m_dwNumMat];
	//}

	////マテリアル情報に対するポインタを所得
	//pMat = (D3DXMATERIAL*)pXmodel->m_pBuffMat->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)pXmodel->m_dwNumMat; nCntMat++)
	//{
	//	if (pMat[nCntMat].pTextureFilename != nullptr)
	//	{
	//		//テクスチャの読み込み
	//		D3DXCreateTextureFromFile(pDevice,
	//			pMat[nCntMat].pTextureFilename,
	//			&m_pTexture[nCntMat]);
	//	}
	//	else
	//	{
	//		m_pTexture[nCntMat] = nullptr;
	//	}
	//}
}

//====================================================================
//頂点座標の設定
//====================================================================
void CObjectX::SetVerTex(void)
{

}

//====================================================================
//テクスチャ座標の設定
//====================================================================
void CObjectX::SetAnim(D3DXVECTOR2 Tex)
{

}

//====================================================================
//頂点カラーの設定
//====================================================================
void CObjectX::SetColor(D3DXCOLOR col)
{

}

//====================================================================
//頂点情報のサイズを設定
//====================================================================
void CObjectX::SizeVtxSet(void)
{
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE* pVtxBuff;		//頂点バッファへのポインタ
	D3DXVECTOR3 RetrunPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	//Xモデルの取得
	CXModel::XModel* pXmodel = CManager::GetInstance()->GetXModel()->GetXModel(m_IdxModelNumber);

	//頂点数を所得
	nNumVtx = pXmodel->m_pMesh->GetNumVertices();

	//頂点フォーマットのサイズを所得
	dwSizeFVF = D3DXGetFVFVertexSize(pXmodel->m_pMesh->GetFVF());

	//頂点バッファをロック
	pXmodel->m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

		if (vtxMin.x > vtx.x)
		{
			vtxMin.x = vtx.x;
		}
		if (vtxMin.y > vtx.y)
		{
			vtxMin.y = vtx.y;
		}
		if (vtxMin.z > vtx.z)
		{
			vtxMin.z = vtx.z;
		}

		if (vtxMax.x < vtx.x)
		{
			vtxMax.x = vtx.x;
		}
		if (vtxMax.y < vtx.y)
		{
			vtxMax.y = vtx.y;
		}
		if (vtxMax.z < vtx.z)
		{
			vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	//頂点フォーマットのサイズ分ポインタを進める
	}

	if (-vtxMin.x < vtxMax.x)
	{
		m_size.x = vtxMax.x;
	}
	else
	{
		m_size.x = -vtxMin.x;
	}

	if (-vtxMin.y < vtxMax.y)
	{
		m_size.y = vtxMax.y;
	}
	else
	{
		m_size.y = -vtxMin.y;
	}

	if (-vtxMin.z < vtxMax.z)
	{
		m_size.z = vtxMax.z;
	}
	else
	{
		m_size.z = -vtxMin.z;
	}

	//頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();
}

//====================================================================
//中心から一番遠い頂点を基準に矩形の判定用の幅と高さを割り出す
//====================================================================
void CObjectX::SwapSize(void)
{
	float fSwap;

	fSwap = m_size.x;
	m_size.x = m_size.z;
	m_size.z = fSwap;
}

//==========================================
// リストの取得
//==========================================
CListManager<CObjectX>* CObjectX::GetList(void)
{
	return m_pList;
}
