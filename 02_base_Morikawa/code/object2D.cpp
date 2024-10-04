//========================================
//
//オブジェクト2D[object.cp]
//Author：森川駿弥
//
//========================================
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "input.h"
#include "texture.h"

//========================================
// 定数定義
//========================================
namespace
{
const int NUM_PLACE = 8;	// スコアの桁数
const float SCORE_WIDTH = 20.0f;	// スコアの幅
const float SCORE_HEIGHT = 20.0f;	// スコアの高さ
const float SCORE_SPACE = 40.0f;	// スコアの桁数毎の間隔
}

//========================================
//コンストラクタ
//========================================
CObject2D::CObject2D(int nPriority) :
	CObject(nPriority),
	m_col(1.0f, 1.0f, 1.0f, 1.0f),	//色
	m_fLength(0.0f),		//対角線の長さ
	m_fAngle(0.0f),			//対角線の角度
	m_nCounterAnim(0),		//アニメーションカウンター
	m_nPatternAnim(0),		//アニメーションパターンNo
	m_pVtxBuff(nullptr),	//頂点情報を格納
	m_nIdxTexture(0)		//テクスチャの番号
{//値クリア

	m_size = D3DXVECTOR2(0.0f, 0.0f);		// サイズ
	m_sizeOrigin = D3DXVECTOR2(0.0f, 0.0f);	// 元のサイズ
}

//========================================
//デストラクタ
//========================================
CObject2D::~CObject2D(void)
{
}

//========================================
//ポリゴンの生成
//========================================
CObject2D *CObject2D::Create()
{
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//2Dオブジェクトの生成
		pObject2D = new CObject2D;

		//初期化
		pObject2D->Init();
	}

	//ポインタを返す
	return pObject2D;
}

//========================================
//ポリゴンの初期化
//========================================
HRESULT CObject2D::Init(void)
{
	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (pDevice != nullptr)
	{
		//頂点バッファの生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr)))
		{// 失敗を返す
			return E_FAIL;
		}

		//変数の初期化
		m_fLength = sqrtf(50.0f * 50.0f + 50.0f * 50.0f) * 1.0f;	//対角線の長さを算出する
		m_fAngle = atan2f(50.0f, 50.0f);	//対角線の角度を算出する

		//頂点情報
		SetVertex();
	}

	return S_OK;
}

//========================================
//ポリゴンの終了
//========================================
void CObject2D::Uninit(void)
{
	//頂点情報の終了
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//オブジェクト(自分自身)の破棄
	Release();
}

//========================================
//ポリゴンの更新
//========================================
void CObject2D::Update(void)
{
	//頂点座標
	SetVertex();
}

//========================================
//ポリゴンの描画
//========================================
void CObject2D::Draw(void)
{
	//CRenderer型のポインタ
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//CTexture型のポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
//頂点情報の設定
//========================================
void CObject2D::SetVertex(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXCOLOR col = GetCol();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = pos.x + sinf(rot.z - D3DX_PI + m_fAngle) *  m_fLength;
	pVtx[0].pos.y = pos.y + cosf(rot.z - D3DX_PI + m_fAngle) *  m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + D3DX_PI - m_fAngle) *  m_fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + D3DX_PI - m_fAngle) *  m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - m_fAngle) *  m_fLength;
	pVtx[2].pos.y = pos.y + cosf(rot.z - m_fAngle) *  m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z + m_fAngle) *  m_fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z + m_fAngle) *  m_fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	SetPos(pos);
	SetRot(rot);
}

//========================================
//ポリゴンのサイズ調整
//========================================
void  CObject2D::SetSize(float fWidht, float fHeight)
{
	//対角線の長さ(横、縦)
	m_fLength = sqrtf(fWidht * fWidht + fHeight * fHeight) * 0.5f;

	//対角線の向き(横、縦)
	m_fAngle = atan2f(fWidht, fHeight);

	// サイズ
	m_size = D3DXVECTOR2(fWidht, fHeight);

	//頂点情報
	SetVertex();
}

//========================================
//テクスチャの割り当て
//========================================
void CObject2D::BindTexture(int pTexture)
{
	m_nIdxTexture = pTexture;
}

//========================================
//アニメーションテクスチャ座標の更新
//========================================
void CObject2D::SetVertexAnim(int Num, int Cnt)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の再設定
	pVtx[0].tex = D3DXVECTOR2(1.0f / Num * Cnt, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / Num * (Cnt + 1), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f / Num * Cnt, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f / Num * (Cnt + 1), 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//========================================
//頂点座標
//========================================
void CObject2D::SetPosVertex(void)
{
	// 位置・向き
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = pos.x + sinf(rot.z - D3DX_PI + m_fAngle) *  m_fLength;
	pVtx[0].pos.y = pos.y + cosf(rot.z - D3DX_PI + m_fAngle) *  m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + D3DX_PI - m_fAngle) *  m_fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + D3DX_PI - m_fAngle) *  m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - m_fAngle) *  m_fLength;
	pVtx[2].pos.y = pos.y + cosf(rot.z - m_fAngle) *  m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z + m_fAngle) *  m_fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z + m_fAngle) *  m_fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	SetPos(pos);
	SetRot(rot);
}

//========================================
//背景の頂点情報
//========================================
void CObject2D::SetVertexBg(void)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//========================================
//テクスチャ座標の開始位置(V値)
//========================================
void CObject2D::SetVertexMoveBg(void)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標(V値の更新)
	m_aTexV -= 0.001f;

	//テクスチャ座標の更新
	pVtx[0].tex = D3DXVECTOR2(0.0f, m_aTexV);
	pVtx[1].tex = D3DXVECTOR2(1.0f, m_aTexV);
	pVtx[2].tex = D3DXVECTOR2(0.0f, m_aTexV + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, m_aTexV + 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//========================================
//エフェクトの頂点座標
//========================================
void CObject2D::SetVertexEffect(float radius, D3DXCOLOR col)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標を更新
	pVtx[0].pos = D3DXVECTOR3(pos.x - radius, pos.y - radius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + radius, pos.y - radius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - radius, pos.y + radius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + radius, pos.y + radius, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// 位置・向き設定
	SetPos(pos);
	SetRot(rot);
}

//========================================
// 頂点情報の色設定
//========================================
void CObject2D::SetVertexColor(D3DXCOLOR col)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//========================================
//スコア頂点情報
//========================================
void CObject2D::SetScoreVertex(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x - SCORE_WIDTH, pos.y - SCORE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + SCORE_WIDTH, pos.y - SCORE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - SCORE_WIDTH, pos.y + SCORE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + SCORE_WIDTH, pos.y + SCORE_HEIGHT, 0.0f);

		//スコアの桁数をずらす
		pos.x += SCORE_SPACE;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //0.0～1.0fで設定
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + m_nPatternAnim * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + m_nPatternAnim * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + m_nPatternAnim * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + m_nPatternAnim * 0.1f, 1.0f);

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	SetPos(pos);
	SetRot(rot);
}

//========================================
//スコア設定の頂点情報
//========================================
void CObject2D::SetScore(int nNum[8])
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//頂点座標を更新
		pVtx[0].pos = D3DXVECTOR3(pos.x - SCORE_WIDTH, pos.y - SCORE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + SCORE_WIDTH, pos.y - SCORE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - SCORE_WIDTH, pos.y + SCORE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + SCORE_WIDTH, pos.y + SCORE_HEIGHT, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //0.0～1.0fで設定
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + nNum[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + nNum[nCntScore] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + nNum[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + nNum[nCntScore] * 0.1f, 1.0f);

		pVtx += 4;

		//スコアの桁数をずらす
		pos.x += SCORE_SPACE;
	}
	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	SetPos(pos);
	SetRot(rot);
}
