//============================================
//
//	プレイヤー2Dのサンプル [Player2D.cpp]
//	Author:sakamoto kai
//
//============================================
#include "Player2D.h"
#include "renderer.h"
#include "manager.h"
#include "blockmanager.h"
#include "input.h"
#include "game.h"
#include "objectAnim2d.h"
#include "sound.h"
#include "texture.h"
//マクロ定義

//====================================================================
//コンストラクタ
//====================================================================
CPlayer2D::CPlayer2D(int nPriority) : CObject2D(nPriority)
{

	SetWidth(Size.x);
	SetHeight(Size.y);
	m_Move = INITVECTOR3;
	m_nSample = 0;
	m_bLanding = false;
}

//====================================================================
//デストラクタ
//====================================================================
CPlayer2D::~CPlayer2D()
{

}

//====================================================================
//生成処理
//====================================================================
CPlayer2D* CPlayer2D::Create(int nPriority)
{
	CPlayer2D* pSample2D = NULL;

	if (pSample2D == NULL)
	{
		//オブジェクト2Dの生成
		pSample2D = new CPlayer2D(nPriority);
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
HRESULT CPlayer2D::Init(void)
{
	CObject2D::Init();

	////テクスチャ設定
	//SetTexture("data\\TEXTURE\\Number01.png");

	//新しくcppを作成した時は新しいTYPEを列挙に追加して指定すること
	SetType(CObject::TYPE_PLAYER2D);

	 CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\HGS\\die.png");
	m_Move.x = move_player;
	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CPlayer2D::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CPlayer2D::Update(void)
{
	//頂点情報の更新
	CObject2D::Update();

	D3DXVECTOR3 pos = GetPos();
	m_PosOld = pos;
	Jump();
	Move(&pos);

	//移動範囲のチェック
	if (pos_max.x < (pos.x + (Size.x / 2)))
	{
		pos.x = pos_max.x - (Size.x / 2);
		m_Move.x *= -1.0f;
	}
	else if (pos_min.x > (pos.x - (Size.x / 2)))
	{
		pos.x = pos_min.x + (Size.x / 2);
		m_Move.x *= -1.0f;
	}
	//重力と落下限界
	m_Move.y += gravity;
	if (pos.y > pos_max.y)
	{
		Death();
	}
	else if (pos.y < pos_min.y)
	{
		pos.y = pos_min.y;
		if (m_bLanding)
		{
			Death();
		}
	}

	SetPos(pos);

	CManager::GetInstance()->GetBlockManager()->Collision(this);
}

//====================================================================
//描画処理
//====================================================================
void CPlayer2D::Draw(void)
{
	CObject2D::Draw();
}
//====================================================================
//描画処理
//====================================================================
void CPlayer2D::Death(void)
{
	CManager::GetInstance()->SetPause(false);
	SetDeathFlag(true);
	CGame::SetGameEnd(true);
	CObjectAnim2D* pDead;
	pDead = CObjectAnim2D::Create(SCREEN_CENTER, 5, 9, 5 * 9, true, 60, 5);
	pDead->SetPos(GetPos());
	pDead->SetHeight(500.0f);
	pDead->SetWidth(500.0f);
	pDead->SetTexture("data\\TEXTURE\\HGS\\die.png");
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOSS_BULLET);
}
//====================================================================
//移動処理
//====================================================================
void CPlayer2D::Move(D3DXVECTOR3* pos)
{
	float fMove = 5.0f;
	*pos += m_Move;

	if(m_Move.y > 3.0f)
		m_bLanding = false;
}

//====================================================================
//ジャンプ処理
//====================================================================
void CPlayer2D::Jump()
{
	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputPad = CManager::GetInstance()->GetInputJoyPad();
	if (m_bLanding &&(pInputKey->GetTrigger(DIK_SPACE) || pInputPad->GetTrigger(CInputJoypad::BUTTON_A,0)))
	{
		m_bLanding = false;
		m_Move.y = -jump_player;
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE_PLAYER);
	}
}