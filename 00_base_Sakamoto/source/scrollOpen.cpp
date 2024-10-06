//============================================
//
//	巻物処理 [makimono.h]
//	Author : Takeru Ogasawara
//
//============================================
#include "scrollOpen.h"
#include "object2D.h"
#include "objGauge2D.h"

#include "manager.h"
#include "sound.h"

// 定数宣言
namespace
{
	const char* SCROLL_TEX_PATH = "data\\TEXTURE\\makimono\\fire.png";	// 巻物テクスチャパス
	const char* PAPER_TEX_PATH = "data\\TEXTURE\\makimono\\paper.png";	// 紙テクスチャパス

	const D3DXVECTOR3 SCROLL_POS = { 1100.0f, SCREEN_HEIGHTCENTER, 0.0f };	// 巻物位置
	const D3DXVECTOR2 SCROLL_SIZE = { 600.0f, 150.0f };	// 巻物大きさ
	const float SCROLL_OPEN_SPEED = 30.0f;	// 巻物開く速さ
	const float SCROLL_OPEN_END_POS = 200.0f;	// 巻物終了位置

	const float SCROLL_CLOSE_SPEED = 50.0f;	// 巻物閉じる速さ
	const float SCROLL_CLOSE_END_POS = 1190.0f;	// 巻物終了位置

	const D3DXVECTOR3 PAPER_POS = { 1200.0f, 115.0f, 0.0f };	// 紙の初期位置
	const D3DXVECTOR2 PAPER_SIZE = { 488.0f, 50.0f };	// 紙の大きさ
}

//====================================================================
// コンストラクタ
//====================================================================
CScrollOpen::CScrollOpen()
{
	m_pScroll = nullptr;
	m_pPaper = nullptr;
	m_bSound = false;
	m_mode = MODE_NONE;
}

//====================================================================
// デストラクタ
//====================================================================
CScrollOpen::~CScrollOpen()
{

}

//====================================================================
// 生成
//====================================================================
CScrollOpen* CScrollOpen::Create()
{
	CScrollOpen* pScroll = nullptr;

	if (pScroll == nullptr)
	{
		pScroll = new CScrollOpen;

		pScroll->Init();
	}

	return pScroll;
}

//====================================================================
// 初期化
//====================================================================
HRESULT CScrollOpen::Init()
{
	InitPaper();
	InitScroll();

	return S_OK;
}

//====================================================================
// 終了
//====================================================================
void CScrollOpen::Uninit()
{
	if (m_pScroll)
	{
		m_pScroll->Uninit();
		m_pScroll = nullptr;
	}

	if (m_pPaper)
	{
		m_pPaper->Uninit();
		m_pPaper = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
// 更新
//====================================================================
void CScrollOpen::Update()
{
	switch (m_mode)
	{
	case CScrollOpen::MODE_NONE:
		break;

	case CScrollOpen::MODE_OPEN:	// 開く
		Open();
		break;

	case CScrollOpen::MODE_STOP:	// 止める
		break;

	case CScrollOpen::MODE_CLOSE:	// 閉じる
		Close();
		break;

	case CScrollOpen::MODE_FADE:	// フェード
		Fade();
		break;

	case CScrollOpen::MODE_END:		// 終了
		//Uninit();
		break;
	default:
		break;
	}
}

//====================================================================
// 描画
//====================================================================
void CScrollOpen::Draw()
{

}

//====================================================================
// 巻物の初期化
//====================================================================
void CScrollOpen::InitScroll()
{
	m_pScroll = CObject2D::Create();

	if (m_pScroll)
	{
		m_pScroll->SetPos(SCROLL_POS);
		m_pScroll->SetHeight(SCROLL_SIZE.x);
		m_pScroll->SetWidth(SCROLL_SIZE.y);
		m_pScroll->SetTexture(SCROLL_TEX_PATH);
	}

	if (m_mode == MODE_NONE)
	{
		m_mode = MODE_OPEN;
	}
}

//====================================================================
// 紙の初期化
//====================================================================
void CScrollOpen::InitPaper()
{
	m_pPaper = CObjGauge2D::Create();

	if (m_pPaper)
	{
		m_pPaper->SetPos(PAPER_POS);
		m_pPaper->SetHeight(PAPER_SIZE.x);
		m_pPaper->SetWidth(PAPER_SIZE.y);
		m_pPaper->SetTexture(PAPER_TEX_PATH);
		m_pPaper->SetTypeVertex(m_pPaper->TYPE_VERTEX::TYPE_LEFT);
	}
}

//====================================================================
// 巻物を開く
//====================================================================
void CScrollOpen::Open()
{
	if (!m_pScroll)
	{
		return;
	}

	if (!m_bSound)
	{
		// 開く音
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SCROLL_OPEN);
		m_bSound = true;
	}

	// 位置取得
	D3DXVECTOR3 pos = m_pScroll->GetPos();

	// 開く速度を加算
	pos.x -= CManager::GetInstance()->GetGameSpeed() * SCROLL_OPEN_SPEED;

	if (pos.x < SCROLL_OPEN_END_POS)
	{// 終了位置まで来たら止める
		pos.x = SCROLL_OPEN_END_POS;
		m_mode = MODE_STOP;
	}

	// 設定
	m_pScroll->SetPos(pos);

	// 紙の移動
	movePaper(pos);
}

//====================================================================
// 巻物を閉じる
//====================================================================
void CScrollOpen::Close()
{
	if (!m_pScroll)
	{
		return;
	}

	if (m_bSound)
	{
		// 開く音
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SCROLL_OPEN);
		m_bSound = false;
	}

	// 位置の取得
	D3DXVECTOR3 pos = m_pScroll->GetPos();

	// // 閉じる速度を加算
	pos.x += CManager::GetInstance()->GetGameSpeed() * SCROLL_CLOSE_SPEED;

	if (pos.x > SCROLL_CLOSE_END_POS)
	{// 終了位置まで来たら終わる
		pos.x = SCROLL_CLOSE_SPEED;
		m_mode = MODE_END;

		return;
	}

	// 設定
	m_pScroll->SetPos(pos);

	// 紙の移動
	movePaper(pos);
}

//====================================================================
// 巻物をフェード
//====================================================================
void CScrollOpen::Fade()
{
	if (!m_pScroll)
	{
		return;
	}

	// 色の取得
	D3DXCOLOR col = m_pScroll->GetColor();

	// 色を薄くしてく
	col.a -= CManager::GetInstance()->GetGameSpeed() * SCROLL_CLOSE_SPEED;

	if (col.a <= 0.0f)
	{// 終了位置まで来たら終わる
		col.a = 0.0f;
		m_mode = MODE_END;

		return;
	}

	// 色の設定
	m_pScroll->SetColor(col);
}

//====================================================================
// 紙を開く
//====================================================================
void CScrollOpen::movePaper(D3DXVECTOR3 pos)
{
	if (!m_pPaper)
	{
		return;
	}

	// 値を変換
	float fWidth = 1200.0f - pos.x;

	// 横幅の設定
	m_pPaper->SetWidth(fWidth);

	// テクスチャの設定
	m_pPaper->SetAnim1({ 1.0f, 1.0f });
}
