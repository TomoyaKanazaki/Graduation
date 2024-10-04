//========================================
//
//シューティングアクション[objectX.h]
//Author：森川駿弥
//
//========================================
#include "objectX.h"
#include "renderer.h"
#include "manager.h"

//========================================
//コンストラクタ
//========================================
CObjectX::CObjectX(int nPriority) :
		CObject(nPriority),
		m_pTexture(nullptr),						//テクスチャへのポインタ
		m_pMesh(nullptr),							//メッシュ(頂点情報)へのポインタ
		m_pBuffMat(nullptr),						//マテリアルへのポインタ
		m_dwNumMat(0),								//マテリアルの数
		m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),		//位置
		m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),		//向き
		m_vtxMin(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	//モデルの最小値
		m_vtxMax(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//モデルの最大値											
{
}

//========================================
//デストラクタ
//========================================
CObjectX::~CObjectX()
{

}

//========================================
//初期化
//========================================
HRESULT CObjectX::Init(void)
{
	//ローカル変数宣言
	D3DXMATERIAL *pMat;
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのポインタ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//モデルの最小値を初期化
	m_vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

	//モデルの最大値を初期化
	m_vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntTex = 0; nCntTex < (int)m_dwNumMat; nCntTex++)
	{//マテリアルの数分回す
		if (pMat[nCntTex].pTextureFilename != nullptr)
		{//テクスチャファイル名が存在する
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntTex].pTextureFilename, &m_pTexture);
		}
	}

	//頂点数の取得
	nNumVtx = m_pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//すべての頂点を比較してモデル最小値・最大値を抜き出す
		if (m_vtxMax.x < vtx.x)
		{
			m_vtxMax.x = vtx.x;
		}
		if (m_vtxMin.x > vtx.x)
		{
			m_vtxMin.x = vtx.x;
		}
		if (m_vtxMax.y < vtx.y)
		{
			m_vtxMax.y = vtx.y;
		}
		if (m_vtxMin.y > vtx.y)
		{
			m_vtxMin.y = vtx.y;
		}
		if (m_vtxMax.z < vtx.z)
		{
			m_vtxMax.z = vtx.z;
		}
		if (m_vtxMin.z > vtx.z)
		{
			m_vtxMin.z = vtx.z;
		}

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += dwSizeFVF;
	}

	//頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();

	//成功を返す
	return S_OK;
}

//========================================
//終了
//========================================
void CObjectX::Uninit(void)
{
	//メッシュの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	//マテリアルの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}
	//テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	//オブジェクト(自分自身)の破棄
	Release();
}

//========================================
//更新
//========================================
void CObjectX::Update(void)
{
}

//========================================
//描画
//========================================
void CObjectX::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//位置取得
	D3DXVECTOR3 pos = GetPosition();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{//マテリアルの数分回す
		//マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture); //テクスチャを使用してないときはnullptr

		//モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===========================================
//オブジェクトの外部ファイル
//===========================================
void Load(void)
{
}

//========================================
//3Dオブジェクトの生成
//========================================
void CObjectX::SetVertex(void)
{

}

//========================================
//3Dオブジェクトの生成
//========================================
void CObjectX::SetSize(float fWidht, float fHeight)
{

}

//========================================
//3Dオブジェクトの生成
//========================================
void CObjectX::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
