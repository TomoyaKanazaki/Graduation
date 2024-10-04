//=======================================
//
//ポリゴン処理[idxmeshfield.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================
#include "idxmeshfield.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//========================================
// 定数定義
//========================================
namespace
{
const char* IDXMESHFIELD_TEX = "data\\TEXTURE\\block001.jpg";	// エフェクトの最大数
}

//=======================================
//静的メンバ変数
//=======================================
LPDIRECT3DTEXTURE9 CIdxMeshField::m_pTexture = NULL;			//テクスチャへのポインタ

//=======================================
//コンストラクタ
//=======================================
CIdxMeshField::CIdxMeshField() :
	m_pVtx(),								//頂点バッファへのポインタ
	m_pIdxBuffMeshField(nullptr),			//インデックスバッファへのポインタ
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	//位置
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	//向き
	m_mtxWorld()							//ワールドマトリックス
{
}

//=======================================
//デストラクタ
//=======================================
CIdxMeshField::~CIdxMeshField()
{

}

//=======================================
//読み込み
//=======================================
HRESULT CIdxMeshField::Load(void)
{
	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		IDXMESHFIELD_TEX,
		&m_pTexture)))
	{// 失敗を返す
		return E_FAIL;
	}

	//成功を返す
	return S_OK;
}

//=======================================
//破棄
//=======================================
void CIdxMeshField::Unload(void)
{
	//テクスチャ破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=======================================
//生成
//=======================================
CIdxMeshField *CIdxMeshField::Create(void)
{
	//インデックスメッシュのポインタ
	CIdxMeshField *pIdxMeshField = nullptr;

	//テクスチャのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	if (pIdxMeshField == nullptr)
	{//nullptrの時
		//インスタンス生成
		pIdxMeshField = new CIdxMeshField;

		//初期化
		pIdxMeshField->Init();

		//プレイヤーテクスチャの割り当て
		pIdxMeshField->BindTexture(pTexture->Regist(IDXMESHFIELD_TEX));
	}

	return pIdxMeshField;
}

//=======================================
// 初期化処理
//=======================================
HRESULT CIdxMeshField::Init(void)
{
	m_pos = D3DXVECTOR3(-1000.0f, 0.0f, 1000.0f);

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CObject3D::MeshVertex();

	return S_OK;
}
 
//=======================================
// 終了処理
//=======================================
void CIdxMeshField::Uninit(void)
{
	CObject3D::Uninit();

	//頂点バッファの破棄
	if (m_pIdxBuffMeshField != NULL)
	{
		m_pIdxBuffMeshField->Release();
		m_pIdxBuffMeshField = NULL;
	}
}

//=======================================
// 更新処理
//=======================================
void CIdxMeshField::Update(void)
{
}

//=======================================
// 描画処理
//=======================================
void CIdxMeshField::Draw(void)
{
	//CRenderer型のポインタ
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtx, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture); //テクスチャを使用してないときはNULL

	//ポリゴンの描画								 
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		9,		// 用意した頂点の数
		0,
		12);	// ポリゴンの数

	//テクスチャの設定
	pDevice->SetTexture(0, NULL); //テクスチャを使用してないときはNULL
}
