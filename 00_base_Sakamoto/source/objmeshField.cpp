//============================================
//
//	オブジェクトメッシュフィールド [objmeshField.cpp]
//	Author:sakamoto kai
//
//============================================
#include "objmeshField.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float WAIGHT_CENTER = 0.5f;	//横の原点(0.0f～1.0f)
	const float HEIGHT_CENTER = 0.5f;	//縦の原点(0.0f～1.0f)
	const float FIELD_SIZE = 100.0f;		//床一枚の大きさ
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CObjmeshField>* CObjmeshField::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CObjmeshField::CObjmeshField(int nPriority) :CObject(nPriority)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_vtxWidth = 0;
	m_vtxHeight = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CObjmeshField::~CObjmeshField()
{

}

//====================================================================
//生成処理
//====================================================================
CObjmeshField* CObjmeshField::Create(int vtxWidth, int vtxHeight)
{
	CObjmeshField* pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		//オブジェクト3Dの生成
		pObject3D = new CObjmeshField();
	}

	if (pObject3D != nullptr)
	{
		// 頂点数の設定
		pObject3D->SetVtxSize(vtxWidth, vtxHeight);
	}

	//オブジェクトの初期化処理
	if (FAILED(pObject3D->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pObject3D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CObjmeshField::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	SetTexture("data\\TEXTURE\\Field\\00_ground.jpg");

	SetType(CObject::TYPE_OBJMESHFIELD);

	//頂点バッファの生成
	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_vtxHeight * m_vtxWidth,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr)))
	{
		return E_FAIL;
	}

	VERTEX_3D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0, nCenterW = 0, nCenterH = 0; nCnt < m_vtxHeight * m_vtxWidth; nCnt++)
	{
		nCenterW = nCnt % m_vtxWidth;

		//頂点座標の設定 
		pVtx[nCnt].pos.x = FIELD_SIZE * (nCenterW - (((float)m_vtxWidth - 1) * WAIGHT_CENTER));
		pVtx[nCnt].pos.y = 0.0f;
		pVtx[nCnt].pos.z = FIELD_SIZE * (m_vtxHeight - 1) * HEIGHT_CENTER - FIELD_SIZE * nCenterH;

		//法線ベクトルの設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		if (nCenterW % 2 == 0)
		{
			pVtx[nCnt].tex.x = 0.0f;
		}
		else
		{
			pVtx[nCnt].tex.x = 1.0f;
		}
		if (nCenterH % 2 == 0)
		{
			pVtx[nCnt].tex.y = 0.0f;
		}
		else
		{
			pVtx[nCnt].tex.y = 1.0f;
		}

		if ((nCnt - nCenterH) % (m_vtxWidth - 1) == 0 && nCnt != 0 && nCnt != (m_vtxWidth - 1) * nCenterH + nCenterH)
		{
			nCenterH++;
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファの生成
	m_pDevice->CreateIndexBuffer(sizeof(WORD) * (m_vtxWidth * m_vtxHeight + m_vtxWidth * (m_vtxHeight - 2) + 2 * (m_vtxHeight - 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	WORD* pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを所得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nOri = 0; nCnt < (m_vtxWidth * m_vtxHeight + m_vtxWidth * (m_vtxHeight - 2) + 2 * (m_vtxHeight - 2)) / 2; nCnt++)
	{
		if ((nCnt - nOri) % m_vtxWidth == 0 && nCnt != 0 && nCnt != m_vtxWidth * nOri + nOri)
		{
			pIdx[nCnt * 2] = nCnt - 1 - nOri;			//偶数
			pIdx[nCnt * 2 + 1] = nCnt + m_vtxWidth - nOri;		//奇数

			nOri++;
		}
		else
		{
			pIdx[nCnt * 2] = nCnt + m_vtxWidth - nOri;	//偶数
			pIdx[nCnt * 2 + 1] = nCnt - nOri;		//奇数
		}
	}

	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CObjmeshField>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CObjmeshField::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//インデックスバッファの破棄
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//nullptrにする処理
//====================================================================
void CObjmeshField::SetNULL(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//インデックスバッファの破棄
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CObjmeshField::Update(void)
{

}

//====================================================================
//描画処理
//====================================================================
void CObjmeshField::Draw(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
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
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	m_pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	m_pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	m_pDevice->SetFVF(FVF_VERTEX_3D);

	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	assert(GetIdx() != -1);		//テクスチャの番号を入れ忘れた場合エラーを吐く

	//テクスチャの設定
	m_pDevice->SetTexture(0, pTexture->GetAddress(GetIdx()));

	//ポリゴンの描画
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		0,//用意した頂点の数
		0,
		(m_vtxWidth * m_vtxHeight + m_vtxWidth * (m_vtxHeight - 2) + 2 * (m_vtxHeight - 2)) - 2);//描画するプリミティブの数
}

//====================================================================
//テクスチャの割り当て
//====================================================================
void CObjmeshField::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//====================================================================
//頂点座標の設定
//====================================================================
void CObjmeshField::SetVerPos(D3DXVECTOR3 Pos0, D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3)
{
	VERTEX_3D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos = Pos0;
	pVtx[1].pos = Pos1;
	pVtx[2].pos = Pos2;
	pVtx[3].pos = Pos3;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//テクスチャ座標の設定
//====================================================================
void CObjmeshField::SetAnim(D3DXVECTOR2 Tex)
{
	VERTEX_3D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(Tex.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, Tex.y);
	pVtx[3].tex = D3DXVECTOR2(Tex.x, Tex.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点カラーの設定
//====================================================================
void CObjmeshField::SetColor(D3DXCOLOR col)
{
	m_Color = col;

	VERTEX_3D* pVtx;	//頂点ポインタを所得

					//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点カラーの設定
//====================================================================
void CObjmeshField::SetTexture(const char* name)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	m_nIdxTexture = pTexture->Regist(name);
}

//====================================================================
// 頂点数の設定
//====================================================================
void CObjmeshField::SetVtxSize(int vtxWidth, int vtxHeight)
{
	m_vtxWidth = vtxWidth;
	m_vtxHeight = vtxHeight;
}

//====================================================================
// リスト取得
//====================================================================
CListManager<CObjmeshField>* CObjmeshField::GetList(void)
{
	return m_pList;
}
