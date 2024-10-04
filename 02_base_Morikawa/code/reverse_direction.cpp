//=============================================================================
// 
//  反転演出処理 [reverse_direction.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "reverse_direction.h"
#include "manager.h"
#include "texture.h"
#include "calculation.h"
#include "sound.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\reverse.png";	// テクスチャのファイル
}

namespace StateTime
{
	const float APPEARANCE = 1.0f;	// 登場
	const float WAIT = 0.5f;		// 待機
	const float FADEOUT = 0.3f;		// フェードアウト
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CReverse_Direction::STATE_FUNC CReverse_Direction::m_StateFuncList[] =
{
	&CReverse_Direction::StateNone,			// なし
	&CReverse_Direction::StateAppearance,	// 登場
	&CReverse_Direction::StateWait,			// 待機
	&CReverse_Direction::StateFadeout,		// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CReverse_Direction::CReverse_Direction(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CReverse_Direction::~CReverse_Direction()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CReverse_Direction* CReverse_Direction::Create()
{
	// メモリの確保
	CReverse_Direction* pObj = new CReverse_Direction;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CReverse_Direction::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 180.0f);
#endif
	SetSize(size.x, size.y);
	SetSizeOrigin(size);


	// 位置
	SetPos(MyLib::Vector3(640.0f, 360.0f, 0.0f));
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// 状態遷移
	SetState(State::STATE_APPEARANCE);

	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// サウンド再生
	pSound->PlaySound(CSound::SOUND_LABEL_SE_REVERSE);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CReverse_Direction::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CReverse_Direction::Update()
{
	// 状態更新
	UpdateState();

	if (IsDeath()) return;

	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 状態更新
//==========================================================================
void CReverse_Direction::UpdateState()
{
	// 状態タイマー加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 実行
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// なし
//==========================================================================
void CReverse_Direction::StateNone()
{

}

//==========================================================================
// 登場
//==========================================================================
void CReverse_Direction::StateAppearance()
{
	// 透明度と割合同期
	float ratio = UtilFunc::Correction::EaseInExpo(0.3f, 1.0f, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, ratio));

	// 回転
	MyLib::Vector3 rot = GetRot();
	rot.z = UtilFunc::Correction::EaseInExpo(0.0f, -(D3DX_PI * 2.0f) * 1.0f, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetRot(rot);

	// サイズ
	D3DXVECTOR2 size;
	size.x = UtilFunc::Correction::EaseInBack(GetsizeOrigin().x, GetsizeOrigin().x * 3.0f, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	size.y = UtilFunc::Correction::EaseInBack(GetsizeOrigin().y, GetsizeOrigin().y * 3.0f, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetSize(size.x, size.y);

	if (m_fStateTime >= StateTime::APPEARANCE)
	{
		SetState(State::STATE_WAIT);
	}
}

//==========================================================================
// 待機
//==========================================================================
void CReverse_Direction::StateWait()
{
	if (m_fStateTime >= StateTime::WAIT)
	{
		SetState(State::STATE_FADEOUT);
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CReverse_Direction::StateFadeout()
{
	// 透明度と割合同期
	float ratio = UtilFunc::Correction::EaseInExpo(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, ratio));

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		Uninit();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CReverse_Direction::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

//==========================================================================
// 状態設定
//==========================================================================
void CReverse_Direction::SetState(const State& state)
{
	m_fStateTime = 0.0f;
	m_state = state;
}
