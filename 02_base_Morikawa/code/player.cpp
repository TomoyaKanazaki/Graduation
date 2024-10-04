//========================================
//
// プレイヤー[player.cpp]
// Author：森川駿弥
//
//========================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"
#include "particle.h"
#include "useful.h"
#include "gauge.h"
#include "main.h"
#include "model.h"
#include "motion.h"
#include "effect.h"
#include "object2D.h"
#include "object3D.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "calculation.h"

#include "stageobj.h"
#include "obstacle.h"
#include "reverse.h"
#include "timer.h"

//========================================
// 定数定義
//========================================
namespace
{
	const int LIFE = 3;			// 体力
	const float SPEED = 500.0f;		// 速度
	const float SPEED_SCALE = 75.0f; // 加速度
	const float JUMP_MOVE = 1500.0f;	// ジャンプ量
	const float JUMP_SAB = JUMP_MOVE * 0.04f;	// ジャンプ減衰
	const float ROLL_TIME = 0.3f;	// 転がり継続時間
	const float RADIUS = 90.0f;		// 半径
	const float HEIGHT = 200.0f;	// 身長
	const float HEIGHT_SCALE = 0.5f;	// 転がり中の身長倍率
	const int DAMAGE_COUNT = 80;		// ダメージカウント

	const char* RABBIT_PASS = "data\\FILE\\rabbit.txt"; // 兎パス
	const char* TURTLE_PASS = "data\\FILE\\turtle.txt"; // 亀パス

	const float WALK_COUNT = 0.3f; // 歩行音のインターバル
}

//========================================
// 静的メンバ変数
//========================================
CPlayer* CPlayer::m_pPlayer = nullptr;

//========================================
//コンストラクタ
//========================================
CPlayer::CPlayer(int nPriority) : CCharacter(nPriority),
m_apNumModel	(0),			// モデルの総数
m_nLife			(0),			// 体力
m_nOldMotion	(0),			// 前回のモーション
m_WalkCounter	(0),			// 歩行時エフェクト出す用のカウンター
m_State			(STATE_NONE),	// 状態
m_fRadius		(0.0f),			// 半径
m_fHeight		(0.0f),			// 身長
m_fRollTime		(0.0f),			// 転がり時間
m_fSpeedScale	(0.0f),			// 加速度
m_fWalkSound	(0.0f),			// 歩行音
m_bJump			(false),		// ジャンプフラグ
m_bRoll			(false),		// 転がりフラグ
m_pEffect		(nullptr),		// エフェクトのポインタ
m_pGauge		(nullptr),		// ゲージのポインタ
m_type			(TYPE_NONE),	// タイプ
m_typeDefault	(TYPE_NONE)		// デフォルトタイプ
{
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//モデル情報
}

//========================================
//デストラクタ
//========================================
CPlayer::~CPlayer()
{
}

//========================================
//プレイヤーの生成
//========================================
CPlayer *CPlayer::Create(std::string pfile)
{
	if (m_pPlayer == nullptr)
	{
		//プレイヤー生成
		m_pPlayer = new CPlayer;

		//初期化
		m_pPlayer->Init(pfile);
	}

	//ポインタを返す
	return m_pPlayer;
}

//========================================
// 初期化
//========================================
HRESULT CPlayer::Init(std::string pfile)
{
	// キャラの初期化
	CCharacter::Init(pfile);

	// プレイヤー状態の初期化
	m_State = STATE_NORMAL;
	m_typeDefault = m_type = TYPE_TURTLE;

	// 位置設定
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 向き設定
	SetRot(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));

	// 移動量設定
	SetMove(D3DXVECTOR3(SPEED, 0.0f, 0.0));

	// 歩行時のカウンター
	m_WalkCounter = 0;

	// 情報を初期化
	m_nLife = LIFE;
	m_fRadius = RADIUS;
	m_fHeight = HEIGHT;

	// ゲージ生成
	m_pGauge = CGauge::Create(m_nLife);

	// 位置設定
	m_pGauge->SetPos(D3DXVECTOR3(50.0f, 650.0f, 0.0f));

	// サイズ設定
	m_pGauge->SetSize(50.0f, 50.0f);

	return S_OK;
}

//========================================
//終了
//========================================
void CPlayer::Uninit(void)
{
	if (m_pGauge != nullptr)
	{
		m_pGauge->Uninit();
		m_pGauge = nullptr;
	}

	// 終了
	CCharacter::Uninit();
	m_pPlayer = nullptr;
}

//========================================
//更新
//========================================
void CPlayer::Update(void)
{
	// 状態を保存する
	PLAYERTYPE type = m_type;

	// 自身の情報を取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();

	// デルタタイムの取得
	const float fDeltaTime = CManager::GetInstance()->GetDeltaTime();

	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// カメラの追従設定
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
#ifndef _DEBUG
	pCamera->Following(pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
#endif // !_DEBUG
#if 1
	pCamera->Following(pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
#endif // 0

	// 移動処理
	Move(pos, move, fDeltaTime);

	// 着地判定
	Land(pos, move);

	// ダメージカウンター
	DamageCounter();

	// ステージオブジェとの範囲チェック
	CheckStageObjRange();

	// 反転オブジェの当たり判定
	CollisionReverseObj();

	// ゲージに体力設定
	m_pGauge->SetLife(m_nLife);

	// 状態が変わっていた場合モデルを変更する
	if (type != m_type)
	{

		// 変化
		Myparticle::Create(Myparticle::TYPE::TYPE_CHANGE, GetPos());

		// サウンド再生
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_CHANGE);

		// モーション情報の取得
		CMotion* pMotion = GetMotion();

		// モーションを削除
		if (pMotion != nullptr)
		{//モーション破棄
			pMotion->Uninit();
			delete pMotion;
			pMotion = nullptr;
		}

		// モーション生成
		pMotion = CMotion::Create();

		switch (m_type)
		{
		case TYPE_RABBIT:
			// モーション読み込み
			pMotion->Load(RABBIT_PASS);
			break;

		case TYPE_TURTLE:
			// モーション読み込み
			pMotion->Load(TURTLE_PASS);
			break;

		default:
			break;
		}
	}

	// モーション
	Motion();

	// 情報を設定
	SetPos(pos);
	SetMove(move);

	// 当たり判定処理
	Collision();

	D3DXVECTOR3 rot = GetRot();
	// デバッグ表示
	DebugProc::Print(DebugProc::POINT_LEFT, "プレイヤーの位置：%f、%f、%f\n", pos.x, pos.y, pos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "プレイヤーの移動量：%f、%f、%f\n", move.x, move.y, move.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "プレイヤーの向き：%f、%f、%f\n", rot.x, rot.y, rot.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "プレイヤーのサイズ：%f、%f、%f\n", m_fRadius, m_fHeight, m_fRadius);
	DebugProc::Print(DebugProc::POINT_LEFT, "プレイヤーの体力：%d\n", m_nLife);
}

//========================================
//描画
//========================================
void CPlayer::Draw(void)
{
	// 描画
	CCharacter::Draw();
}

//========================================
// ダメージカウンター
//========================================
void CPlayer::DamageCounter()
{
	// ダメージカウントを0にする
	m_nDamageCount--;

	if (m_nDamageCount <= 0)
	{ // カウントが一定値以下の場合

		// カウントを補正する
		m_nDamageCount = 0;
	}
}

//========================================
// モーション管理
//========================================
void CPlayer::Motion()
{
	// モーション情報取得
	CMotion* pMotion = GetMotion();

	// モーションがnullの場合関数を抜ける
	if (pMotion == nullptr)
	{
		assert(false);
		return;
	}

	// モーションを設定
	if (m_bJump) // ジャンプ状態
	{
		// ジャンプモーションを設定
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_JUMP);
		DebugProc::Print(DebugProc::POINT_CENTER, "ジャンプ状態\n");
	}
	else if (m_bRoll) // 転がり状態
	{
		// 転がりモーションを設定
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_ROLL);
		DebugProc::Print(DebugProc::POINT_CENTER, "転がり状態\n");
	}
	else // その他の状態
	{
		// 歩行モーションを設定
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_WALK);
	}

	// モーションを更新
	pMotion->Update();
}

//==========================================
//  移動処理
//==========================================
void CPlayer::Move(D3DXVECTOR3& pos, D3DXVECTOR3& move, const float fDeltaTime)
{
	// インプット情報の取得
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputPad* pPad = CManager::GetInstance()->GetInputPad();

#ifdef _DEBUG
	// 2キーで初期位置に戻る
	if (pKeyboard->GetTrigger(DIK_2))
	{
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		return;
	}

#endif

	// 状態の切り替え
	SwitchType(pPad, pKeyboard);

	// 状態ごとの処理
	switch (m_type)
	{
	case TYPE_RABBIT:
		// ジャンプ処理
		Jump(move, pPad, pKeyboard);
		DebugProc::Print(DebugProc::POINT_CENTER, "今、兎です！！！\n");
		break;

	case TYPE_TURTLE:
		// 転がり処理
		Roll(move, pPad, pKeyboard, fDeltaTime);
		DebugProc::Print(DebugProc::POINT_CENTER, "今、亀です！！！\n");
		break;

	default:
		assert(false);
		break;
	}

	// 加速
	SpeedUp(move);

	// 座標に移動量を加算
	pos += move * fDeltaTime;

	// サウンド加算
	m_fWalkSound += fDeltaTime;

	// サウンド再生
	if (m_fWalkSound >= WALK_COUNT)
	{
		CSound* pSound = CManager::GetInstance()->GetSound();
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
		m_fWalkSound = 0.0f;
	}
}

//==========================================
//  ジャンプ処理
//==========================================
void CPlayer::Jump(D3DXVECTOR3& move, CInputPad* pPad, CInputKeyboard* pKeyboard)
{
	// ジャンプ中の場合関数を抜ける
	if (m_bJump) { return; }

	// ボタンが押されたらジャンプする
	if (pPad->GetTrigger(CInputPad::BUTTON_A, 0) || pKeyboard->GetTrigger(DIK_SPACE))
	{
		// ジャンプフラグを設定
		m_bJump = true;

		// 移動量を加算
		move.y += JUMP_MOVE;

		// サウンド情報取得
		CSound* pSound = CManager::GetInstance()->GetSound();

		// サウンド停止
		pSound->Stop(CSound::SOUND_LABEL_SE_JUMP);

		// サウンド再生
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_JUMP);
	}
}

//==========================================
//  転がり処理
//==========================================
void CPlayer::Roll(D3DXVECTOR3& move, CInputPad* pPad, CInputKeyboard* pKeyboard, const float fDeltaTime)
{
	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// 転がり中の場合時間を加算
	if (m_bRoll)
	{
		// 加算
		m_fRollTime += fDeltaTime;

		// 回転
		MyLib::Vector3 rot = GetRot();
		rot.y = UtilFunc::Correction::EasingLinear(0.0f, (D3DX_PI * 2.0f) * 10.0f, 0.0f, ROLL_TIME, m_fRollTime);
		rot.y += -D3DX_PI * 0.5f;
		SetRot(rot);

		Myparticle::Create(Myparticle::TYPE::TYPE_ROLLINGTURTLE, GetPos());

		// 一定時間経過していない場合関数を抜ける
		if (m_fRollTime < ROLL_TIME) { return; }

		// 転がり状態を解除する
		m_fRollTime = 0.0f;
		m_bRoll = false;
		m_fHeight = HEIGHT;
		m_State = STATE_NORMAL;

		// サウンド停止
		pSound->Stop(CSound::SOUND_LABEL_SE_ROWLING);

		return;
	}

	// ボタンが押されたら転がり状態になる
	if (pPad->GetTrigger(CInputPad::BUTTON_A, 0) || pKeyboard->GetTrigger(DIK_SPACE))
	{
		// フラグをtrue
		m_bRoll = true;

		// 身長を減らす
		m_fHeight *= HEIGHT_SCALE;

		// 回転状態にする
		m_State = STATE_ROLL;

		// サウンド再生
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ROWLING);
	}
}

//==========================================
//  状態の切り替え
//==========================================
void CPlayer::SwitchType(CInputPad* pPad, CInputKeyboard* pKeyboard)
{
	// 転がり状態もしくはジャンプ中の場合関数を抜ける
	if (m_bJump || m_bRoll) { return; }

	// タイプをデフォルトで上書きする
	m_type = m_typeDefault;

	// ボタンが押されていない場合関数を抜ける
	if(!pPad->GetPress(CInputPad::BUTTON_LB, 0) && !pKeyboard->GetPress(DIK_LSHIFT)) { return; }

	// デフォルトタイプを見てタイプを変更する
	m_type = m_typeDefault == TYPE_TURTLE ? TYPE_RABBIT : TYPE_TURTLE;
}

//==========================================
//  着地判定
//==========================================
void CPlayer::Land(D3DXVECTOR3& pos, D3DXVECTOR3& move)
{
	// y座標が0.0を下回った場合補正する
	if (pos.y < 0.0f)
	{
		// ジャンプフラグをfalse
		m_bJump = false;

		// 座標を補正
		pos.y = 0.0f;

		// 移動量を削除
		move.y = 0.0f;
	}

	// ジャンプ中なら移動量を減少させる
	if (m_bJump)
	{
		move.y -= JUMP_SAB;
	}
}

//==========================================
// ダメージ関数
//==========================================
void CPlayer::Damage()
{
	// ダメージカウントを設定する
	m_nDamageCount = DAMAGE_COUNT;

	// 寿命を減算する
	m_nLife--;

	// パーティクルを出す
	Myparticle::Create(Myparticle::TYPE_DEATH, GetPos());
}

//==========================================
// ステージオブジェの範囲チェック
//==========================================
void CPlayer::CheckStageObjRange()
{
	// ステージオブジェのリスト取得
	CListManager<CStageObj> list = CStageObj::GetList();

	// 終端を保存
	std::list<CStageObj*>::iterator itr = list.GetEnd();
	CStageObj* pObj = nullptr;

	// 位置取得
	MyLib::Vector3 pos = GetPos();

	// 終端までループ
	while (list.ListLoop(itr))
	{
		CStageObj* pObj = *itr;
		pObj->CollisionRange(pos);
	}
}

//==========================================
// 反転オブジェの当たり判定
//==========================================
void CPlayer::CollisionReverseObj()
{
//#ifndef _DEBUG

	// 反転オブジェのリスト取得
	CListManager<CReverse> list = CReverse::GetList();

	// 終端を保存
	std::list<CReverse*>::iterator itr = list.GetEnd();
	CReverse* pObj = nullptr;

	// 位置取得
	D3DXMATRIX mtx = GetmtxWorld();

	// 終端までループ
	while (list.ListLoop(itr))
	{
		CReverse* pObj = *itr;
		if (pObj->Collision(mtx, MyLib::Vector3(RADIUS, HEIGHT, RADIUS),(PLAYERSTATE)(m_State)))
		{
			// 反転
			m_typeDefault = (m_typeDefault == TYPE_RABBIT) ? PLAYERTYPE::TYPE_TURTLE : PLAYERTYPE::TYPE_RABBIT;
			return;
		}
	}

//#endif // !_DEBUG
}

//==========================================
// 当たり判定
//==========================================
void CPlayer::Collision()
{
//#ifndef _DEBUG

	// ダメージを受けない状態の場合、この関数を抜ける
	if (m_nDamageCount > 0) { return; }

	// 障害物のリスト取得
	CListManager<CObstacle> list = CObstacle::GetList();

	// 終端を保存
	std::list<CObstacle*>::iterator itr = list.GetEnd();
	CObstacle* pObj = nullptr;

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXMATRIX mtx;
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	SetmtxWorld(mtx);

	// 終端までループ
	while (list.ListLoop(itr))
	{
		CObstacle* pObj = *itr;

		if (pObj->Collision(mtx, D3DXVECTOR3(RADIUS, HEIGHT, RADIUS), (PLAYERSTATE)(m_State)))
		{ // 当たり判定に当たった場合

			// 加速を半分にする
			m_fSpeedScale *= 0.5f;

			// ダメージ処理
			Damage();
		}
	}

//#endif // !_DEBUG
}

//==========================================
//  加速処理
//==========================================
void CPlayer::SpeedUp(D3DXVECTOR3& move)
{
	// 加速度を加算
	m_fSpeedScale += CManager::GetInstance()->GetDeltaTime();

	// 移動量を加算
	move.x = SPEED + m_fSpeedScale * SPEED_SCALE;

	DebugProc::Print(DebugProc::POINT_RIGHT, "加速度 : %f", m_fSpeedScale);
}