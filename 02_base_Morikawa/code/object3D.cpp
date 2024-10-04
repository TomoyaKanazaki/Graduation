//========================================
//
//シューティングアクション[object3D.h]
//Author：森川駿弥
//
//========================================
#include "object3D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//========================================
// 定数定義
//========================================
namespace
{
const float FIELD_SIZE = 4000.0f;		// フィールドのサイズ
const float BILLBOARD_WIDTH = 40.0f;	// ビルボードの幅
const float BILLBOARD_HEIGHT = 40.0f;	// ビルボードの高さ
}

//========================================
//コンストラクタ
//========================================
CObject3D::CObject3D(int nPriority) :
	CObject(nPriority)
{
	m_pTexture = nullptr;		// テクスチャへのポインタ
	m_pVtxBuff = nullptr;		// 頂点情報へのポインタ
	m_mtxWorld;					// ワールドマトリックス
	m_nIdxTexture = 0;
}

//========================================
//デストラクタ
//========================================
CObject3D::~CObject3D()
{

}

//========================================
//オブジェクト生成
//========================================
CObject3D *CObject3D::Create()
{
	CObject3D *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{//pObject3Dがnullptrの時
		//3Dオブジェクトの生成
		pObject3D = new CObject3D;

		//初期化
		pObject3D->Init();
	}

	//ポインタを返す
	return pObject3D;
}

//========================================
//テクスチャ割り当て
//========================================
void CObject3D::BindTexture(int pTexture)
{
	m_nIdxTexture = pTexture;
}

//========================================
//初期化
//========================================
HRESULT CObject3D::Init(void)
{
	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (pDevice != nullptr)
	{
		//頂点バッファの生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr)))
		{// 失敗を返す
			return E_FAIL;
		}

		//頂点情報
		SetVertex3D();
	}

	//成功を返す
	return S_OK;
}

//========================================
//終了
//========================================
void CObject3D::Uninit(void)
{
	//頂点情報の終了
	if (m_pVtxBuff != nullptr)
	{//m_pVtxBuffがnullptrじゃないとき
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//オブジェクト(自分自身)の破棄
	Release();
}

//========================================
//更新
//========================================
void CObject3D::Update(void)
{
	//頂点情報
	SetVertex3D();
}

//========================================
//描画
//========================================
void CObject3D::Draw(void)
{
	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//CTexture型のポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// 位置・向き取得
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 Rot = GetRot();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture)); //テクスチャを使用してないときはnullptr

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
//3Dポリゴンの頂点情報
//========================================
void CObject3D::SetVertex3D(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-FIELD_SIZE, 0.0f, FIELD_SIZE);
	pVtx[1].pos = D3DXVECTOR3(FIELD_SIZE, 0.0f, FIELD_SIZE);
	pVtx[2].pos = D3DXVECTOR3(-FIELD_SIZE, 0.0f, -FIELD_SIZE);
	pVtx[3].pos = D3DXVECTOR3(FIELD_SIZE, 0.0f, -FIELD_SIZE);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(10.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 10.0f);
	pVtx[3].tex = D3DXVECTOR2(10.0f, 10.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//========================================
//起伏の当たり判定
//========================================
void CObject3D::Collision(void)
{
	//D3DXVECTOR3 vec1, vec2, nor;

	//float fHeight;		//求める高さ

	//if ()
	//{
	//	//外積
	//	D3DXVec3Cross(&nor, &vec1, &vec2);

	//	//法線を正規化する
	//	D3DXVec3Normalize(&nor, &nor);

	//	//内積
	//	(pos.x - vtx0.x) * nor.x + (fHeight - vtx0.y) * nor.y + (pos.z - vtx0.z) * nor.z = 0.0f;

	//	if (nor.y != 0.0f)
	//	{
	//		fHeight;
	//	}
	//}
}

//========================================
//メッシュフィールドの頂点情報
//========================================
void CObject3D::MeshVertex(void)
{
	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (pDevice != nullptr)
	{
		//頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 9,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			NULL);

		//頂点情報へのポインタ
		VERTEX_3D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntZ = 0; nCntZ < 3; nCntZ++)
		{
			for (int nCntX = 0; nCntX < 3; nCntX++)
			{
				//頂点座標の設定
				pVtx[nCntX + nCntZ * 3].pos = D3DXVECTOR3(nCntX * 1000.0f, 0.0f, nCntZ * -1000.0f);

				//法線の設定
				pVtx[nCntX + nCntZ * 3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//頂点カラーの設定
				pVtx[nCntX + nCntZ * 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//テクスチャ座標の設定
				pVtx[nCntX + nCntZ * 3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		//インデックスバッファの生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&m_pIdxBuff,
			NULL);

		//インデックス情報へのポインタ
		WORD *pIdx;

		//インデックスバッファをロックし、頂点情報へのポインタを取得
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntIdx = 0; nCntIdx < 2; nCntIdx++)
		{
			for (int nCntIdx2 = 0; nCntIdx2 < 3; nCntIdx2++)
			{
				pIdx[(nCntIdx2 * 2) + (nCntIdx * 8) + 0] = 3 + nCntIdx2 * 1 + nCntIdx * 3;
				pIdx[(nCntIdx2 * 2) + (nCntIdx * 8) + 1] = 0 + nCntIdx2 * 1 + nCntIdx * 3;
			}
		}

		//縮退
		pIdx[6] = 2;
		pIdx[7] = 6;

		//頂点番号データの設定
		//pIdx[0] = 3;
		//pIdx[1] = 0;
		//pIdx[2] = 4;
		//pIdx[3] = 1;
		//pIdx[4] = 5;
		//pIdx[5] = 2;
		//pIdx[6] = 2;
		//pIdx[7] = 6;
		//pIdx[8] = 6;
		//pIdx[9] = 3;
		//pIdx[10] = 7;
		//pIdx[11] = 4;
		//pIdx[12] = 8;
		//pIdx[13] = 5;

		//インデックスバッファをアンロック
		m_pIdxBuff->Unlock();
	}
}

//========================================
// フィールドの取得
//========================================
float CObject3D::GetField()
{
	float fField = FIELD_SIZE;

	return fField;
}

//========================================
//ビルボードの頂点情報
//========================================
void CObject3D::SetVerTexBillboard(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_WIDTH, BILLBOARD_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BILLBOARD_WIDTH, BILLBOARD_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_WIDTH, -BILLBOARD_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BILLBOARD_WIDTH, -BILLBOARD_HEIGHT, 0.0f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

void CObject3D::SetVertex(void)
{
}

void CObject3D::SetSize(float fWidth, float fHeight)
{
}
