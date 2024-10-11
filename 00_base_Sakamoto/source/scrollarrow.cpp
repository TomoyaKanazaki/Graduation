//============================================
//
//	スクロールアロー [scrollarrow.cpp]
//	Author:sakamoto kai
//
//============================================
#include "scrollarrow.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float SAMPLE_WIGHT = 70.0f;		//横幅
	const float SAMPLE_HEIGHT = 70.0f;		//縦幅
}

//====================================================================
//コンストラクタ
//====================================================================
CScrollArrow::CScrollArrow(int nPriority) : CObject2D(nPriority)
{
	SetWidth(SAMPLE_WIGHT);
	SetHeight(SAMPLE_HEIGHT);
	m_nSample = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CScrollArrow::~CScrollArrow()
{

}

//====================================================================
//生成処理
//====================================================================
CScrollArrow* CScrollArrow::Create(int nPriority)
{
	CScrollArrow* pSample2D = NULL;

	if (pSample2D == NULL)
	{
		//オブジェクト2Dの生成
		pSample2D = new CScrollArrow(nPriority);
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample2D->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pSample2D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CScrollArrow::Init(void)
{
	CObject2D::Init();

	//テクスチャ設定
	SetTexture("data\\TEXTURE\\UI\\signal.png");

	//新しくcppを作成した時は新しいTYPEを列挙に追加して指定すること
	SetType(CObject::TYPE_SCROLLALLOW);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CScrollArrow::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CScrollArrow::Update(void)
{
	//頂点情報の更新
	CObject2D::Update();

	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		m_State = STATE_UP;
	}
	if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		m_State = STATE_DOWN;

	}
	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{
		m_State = STATE_RIGHT;

	}
	if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{
		m_State = STATE_LEFT;

	}

	D3DXVECTOR3 rot = GetRot();

	switch (m_State)
	{
	case STATE_UP:
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;

	case STATE_LEFT:
		rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f);
		break;

	case STATE_DOWN:
		rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI);
		break;

	case STATE_RIGHT:
		rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.5f);
		break;
	}

	SetRot(rot);
#endif

	//----頂点情報を変更したい場合は以下のコードを使用すること-----
	//VERTEX_2D* pVtx;	//頂点ポインタを所得

	////頂点バッファをロックし、両店情報へのポインタを所得
	//GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	////位置
	//pVtx[0].pos;
	//pVtx[1].pos;
	//pVtx[2].pos;
	//pVtx[3].pos;

	////色
	//pVtx[0].col;
	//pVtx[1].col;
	//pVtx[2].col;
	//pVtx[3].col;

	////テクスチャ
	//pVtx[0].tex;
	//pVtx[1].tex;
	//pVtx[2].tex;
	//pVtx[3].tex;

	////頂点バッファをアンロックする
	//GetVtxBuff()->Unlock();
}

//====================================================================
//描画処理
//====================================================================
void CScrollArrow::Draw(void)
{
	CObject2D::Draw();
}