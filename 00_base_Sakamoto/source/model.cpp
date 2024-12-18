//============================================
//
//	モデルの処理 [model.cpp]
//	Author:sakamoto kai
//
//============================================
#include "model.h"
#include "manager.h"
#include "renderer.h"

//====================================================================
//コンストラクタ
//====================================================================
CModel::CModel()
{
	m_pMesh = nullptr;								//メッシュ(頂点情報)へのポインタ
	m_pBuffMat = nullptr;							//マテリアルへのポインタ
	m_dwNumMat = 0;								//マテリアルの数
	m_pTexture = nullptr;

	m_Startpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_Startrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_Scaling = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//拡大率
	m_pParent = nullptr;							//親モデルへのポインタ

	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_ColorType = COLORTYPE_FALSE;
	m_bDisp = true;
}

//====================================================================
//デストラクタ
//====================================================================
CModel::~CModel()
{

}

//====================================================================
//生成処理
//====================================================================
CModel *CModel::Create(const char *pModelName)
{
	CModel *pModel = nullptr;

	if (pModel == nullptr)
	{
		//モデルの生成
		pModel = new CModel();
	}

	//オブジェクトの初期化処理
	if (FAILED(pModel->Init(pModelName)))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pModel;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CModel::Init(const char *pModelName)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX(pModelName,
		D3DXMESH_SYSTEMMEM,
		m_pDevice,
		nullptr,
		&m_pBuffMat,
		nullptr,
		&m_dwNumMat,
		&m_pMesh);

	strcpy(&m_acModelName[0], pModelName);

	D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	//テクスチャのメモリを確保
	if (m_pTexture == nullptr)
	{
		//メモリを確保する
		m_pTexture = new LPDIRECT3DTEXTURE9[m_dwNumMat];
	}

	//マテリアル情報に対するポインタを所得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != nullptr)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(m_pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_pTexture[nCntMat]);
		}
		else
		{
			m_pTexture[nCntMat] = nullptr;
		}
	}

	//位置の設定

	//向きの設定

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CModel::Uninit(void)
{
	//バッファの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	//メッシュの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (m_pTexture[nCntMat] != nullptr)
		{
			m_pTexture[nCntMat]->Release();
		}
	}

	//テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		//メモリを確保する
		delete[] m_pTexture;
		m_pTexture = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CModel::Update(void)
{

}

//====================================================================
//描画処理
//====================================================================
void CModel::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&m_mtxWorld, m_Scaling.x, m_Scaling.y, m_Scaling.z);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMATRIX mtxParent;

	if (m_pParent != nullptr)
	{//親が存在する
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{//親が存在しない
		//現在(最新)のマトリックスを取得する[=プレイヤーのマトリックス]
		m_pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//算出したマトリクスをかけ合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);

	//ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	m_WorldPos = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

	//現在のマテリアルを所得
	m_pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを所得する
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		D3DCOLORVALUE color;

		switch (m_ColorType)
		{
		case CModel::COLORTYPE_FALSE:

			//マテリアルの設定
			m_pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			m_pDevice->SetTexture(0, m_pTexture[nCntMat]);

			break;
			
		case CModel::COLORTYPE_TRUE_ALL:

			color = pMat[nCntMat].MatD3D.Diffuse;
			pMat[nCntMat].MatD3D.Diffuse = m_Color;

			//マテリアルの設定
			m_pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			pMat[nCntMat].MatD3D.Diffuse = color;

			break;

		case CModel::COLORTYPE_TRUE_A:

			color = pMat[nCntMat].MatD3D.Diffuse;
			pMat[nCntMat].MatD3D.Diffuse.a = m_Color.a;

			//マテリアルの設定
			m_pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			pMat[nCntMat].MatD3D.Diffuse = color;

			//テクスチャの設定
			m_pDevice->SetTexture(0, m_pTexture[nCntMat]);

			break;
		}

		if (m_bDisp == true)
		{
			//モデル(パーツ)の描画
			m_pMesh->DrawSubset(nCntMat);
		}
	}
	//保存していたマテリアルを戻す
	m_pDevice->SetMaterial(&matDef);
}