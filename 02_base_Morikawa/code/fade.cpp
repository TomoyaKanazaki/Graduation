//=======================================
//
//フェード[fade.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================
#include "fade.h"

//=======================================
//コンストラクタ
//=======================================
CFade::CFade() :
	m_State(FADE_NONE),					// 何もしない状態
	m_modeNext(CScene::MODE_TITLE),		// タイトル
	m_pVtxBuff(nullptr), 
	m_pObject2D(nullptr)
{//値クリア
}

//=======================================
//デストラクタ
//=======================================
CFade::~CFade()
{
}

//=======================================
//初期化
//=======================================
HRESULT CFade::Init(CScene::MODE modenext)
{
	// オブジェクト2Dのポインタ
	m_pObject2D = CObject2D::Create();

	// 色取得
	D3DXCOLOR col = m_pObject2D->GetCol();
	col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// 位置設定
	m_pObject2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	// サイズ設定
	m_pObject2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// 色設定
	m_pObject2D->SetCol(col);

	// 頂点情報
	m_pObject2D->SetVertex();

	//フェード設定
	SetFade(modenext);

	m_pObject2D->SetDrawFrag(true);

	return S_OK;
}

//=======================================
//終了
//=======================================
void CFade::Uninit()
{
	if (m_pObject2D != nullptr)
	{// 終了
		m_pObject2D->Uninit();
		m_pObject2D = nullptr;
	}
}

//=======================================
//更新
//=======================================
void CFade::Update()
{
	if (m_pObject2D == nullptr)return;

	// 色取得
	D3DXCOLOR col = m_pObject2D->GetCol();

	// 更新
	m_pObject2D->Update();

	if (m_State != FADE_NONE)
	{
		switch (m_State)
		{
		case FADE_OUT:
			col.a += 0.03f;
			if (col.a >= 1.0f)
			{
				col.a = 1.0f;

				//フェードイン
				m_State = FADE_IN;

				//モード設定
				CManager::GetInstance()->SetMode(m_modeNext);
			}
			break;

		case FADE_IN:
			col.a -= 0.03f;
			if (col.a <= 0.0f)
			{
				col.a = 0.0f;

				//フェードイン
				m_State = FADE_NONE;
			}
			break;
		}
	}

	// 色設定
	m_pObject2D->SetCol(col);
}

//=======================================
//描画
//=======================================
void CFade::Draw()
{
	if (m_pObject2D == nullptr) return;

	// 描画
	m_pObject2D->Draw();
}

//=======================================
//設定
//=======================================
void CFade::SetFade(CScene::MODE modenext)
{
	// 色取得
	D3DXCOLOR col = m_pObject2D->GetCol();

	if (m_State != FADE_OUT)
	{
		// 状態設定
		m_State = FADE_OUT;

		// 次の画面
		m_modeNext = modenext;

		// 不透明ポリゴン
		col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

//=======================================
// 状態設定
//=======================================
void CFade::SetState(FADE state)
{
	if (m_pObject2D == nullptr)return;

	// 色取得
	D3DXCOLOR col = m_pObject2D->GetCol();
	col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	m_State = state;

	switch (state)
	{
	case FADE_NONE:
		col.a = 0.0f;
		break;

	case FADE_IN:
		col.a = 1.0f;
		break;

	case FADE_OUT:
		col.a = 0.0f;
		break;

	default:
		break;
	}

	// 色設定
	m_pObject2D->SetCol(col);
}

//=======================================
//生成
//=======================================
CFade *CFade::Create(CScene::MODE modenext)
{
	CFade* pFade = new CFade;

	pFade->Init(modenext);
	pFade->SetState(FADE_IN);

	return pFade;
}
