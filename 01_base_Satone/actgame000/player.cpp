//==============================================================
//
//プレイヤー処理[playerModel.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "modelHierarchy.h"
#include "input.h"
#include "motion.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"
#include "sound.h"
#include "fade.h"
#include "UI_death.h"
#include "UI_item.h"
#include "particle.h"
#include "object3D.h"

//マクロ定義
#define PRIORITY			(3)			//優先順位
#define POS					(10.0f)		//pos初期値
#define CURVE_RL			(0.5f)		//左右の角度
#define CURVE_UP			(0.0f)		//上の角度
#define CURVE_DOWN			(1.0f)		//下の角度
#define MOVE_Y				(0.7f)		//移動量Y
#define ADD_MOVE_Y			(1.5f)		//移動量Y加算する数
#define JUMP_HEIGHT			(10.0f)		//ジャンプの高さ
#define MAX_STR				(128)		//文字の最大数
#define FRONT_MOVE			(0.6f)		//手前の時の移動量
#define FRONT_DASH_MOVE		(18.0f)		//手前のダッシュ時の移動量
#define MAX_DASH			(2)			//ダッシュの最大数
#define STOP_MOVE			(0.8f)		//止まる判定の移動量
#define FILE_HUMAN			"data\\TEXT\\motion_player.txt"		//プレイヤーモデルのテキスト

#define SAVE_POS_DEFAULT		(D3DXVECTOR3(-5400.0f, 1000.0f, 0.0f))		//チュートリアルの初期値のセーブポイント
#define SAVE_POS_ARTBRIDGE		(D3DXVECTOR3(-2750.0f, 1300.0f, 0.0f))		//チュートリアルの1番目のセーブポイント
#define SAVE_POS_ENEMYBRIDGE	(D3DXVECTOR3(-350.0f, 1000.0f, 0.0f))		//チュートリアルの2番目のセーブポイント

#define SAVE_POS_START		(D3DXVECTOR3(1000.0f, -150.0f, 0.0f))		//ゲームの初期値のセーブポイント
#define SAVE_POS_DOWN_FLOOR	(D3DXVECTOR3(1200.0f, -950.0f, 0.0f))		//ゲームの1番目のセーブポイント
#define SAVE_POS_PIER		(D3DXVECTOR3(50.0f, -1800.0f, 0.0f))		//ゲームの2番目のセーブポイント
#define SAVE_POS_POINT_MAZE	(D3DXVECTOR3(1300.0f, -2800.0f, 0.0f))		//ゲームの3番目のセーブポイント
#define SAVE_POS_BIGINEND	(D3DXVECTOR3(100.0f, -3600.0f, 0.0f))		//ゲームの4番目のセーブポイント
#define SAVE_POS_FALLING	(D3DXVECTOR3(650.0f, -4300.0f, 0.0f))		//ゲームの5番目のセーブポイント

#define HIT_CNT				(60 * 2)	//攻撃当たるまでのカウント数
#define DAMAGE_CNT			(9)			//ダメージカウント数
#define APP_CNT				(100)		//点滅時間
#define STEP_CNT			(10)		//歩く音のカウンター

//静的メンバ変数宣言
char *CPlayer::m_apFileName[PARTS_MAX] =
{
	"data\\MODEL\\sphere.x",

};

//==============================================================
//コンストラクタ
//==============================================================
CPlayer::CPlayer()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
	m_moveSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量保存用

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_apObject3D[nCnt] = nullptr;
	}

	m_posSavePoint[POINT_DEFAULT] = SAVE_POS_DEFAULT;			//チュートリアルの初期値のセーブポイント
	m_posSavePoint[POINT_ARTBRIDGE] = SAVE_POS_ARTBRIDGE;		//チュートリアルの初期値のセーブポイント
	m_posSavePoint[POINT_ENEMYBRIDGE] = SAVE_POS_ENEMYBRIDGE;	//チュートリアルの初期値のセーブポイント

	m_posSavePoint[POINT_START] = SAVE_POS_START;			//ゲームの初期値のセーブポイント
	m_posSavePoint[POINT_DOWN_FLOOR] = SAVE_POS_DOWN_FLOOR;	//ゲームの1番目のセーブポイント
	m_posSavePoint[POINT_PIER] = SAVE_POS_PIER;				//ゲームの2番目のセーブポイント
	m_posSavePoint[POINT_MAZE] = SAVE_POS_POINT_MAZE;		//ゲームの3番目のセーブポイント
	m_posSavePoint[POINT_BIGINEND] = SAVE_POS_BIGINEND;		//ゲームの4番目のセーブポイント
	m_posSavePoint[POINT_FALLING] = SAVE_POS_FALLING;		//ゲームの5番目のセーブポイント

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向き
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最大値
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最小値

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		m_apModel[nCntPlayer] = NULL;		//プレイヤー(パーツ)へのポインタ
	}

	m_nNumModel = 0;		//プレイヤー(パーツ)の総数
	//m_pMotion = NULL;

	m_nPressCounter = 0;	//キーを押したフレーム数

	m_bJump = false;		//ジャンプしたか
	m_bMove = false;		//歩いてるか
	m_bLand = true;			//着地した
	m_bDash = false;		//ダッシュしたか
	m_bDashAuto = false;	//自動ダッシュするか
	m_bCollisionAlpha = false;		//透明ブロックに当たったか

	m_fRotDest = 0.0f;		//目標
	m_fRotDiff = 0.0f;		//差分

	m_nDashCounter = 0;		//ダッシュした回数

	m_particleType = PARTICLETYPE_NONE;	//パーティクルの種類
	m_nType = 1;		//何番目のパーティクルか
	m_nParticleLife = 50;			//パーティクルの寿命
	m_state = STATE_NONE;			//状態
	m_nCntDamage = 0;				//ダメージカウンター
	m_nCntHit = HIT_CNT;			//攻撃あたるまでのカウンター
	m_bPad = false;					//パッドのスティックを倒してるか
	m_nCntSand = STEP_CNT;			//砂のパーティクルカウンター
	m_nNumPosSave = 0;		//何番目のセーブポイントか
}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CPlayer::CPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = SAVE_POS_DEFAULT;						//位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
	m_moveSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量保存用

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_apObject3D[nCnt] = nullptr;
	}

	m_posSavePoint[POINT_DEFAULT] = SAVE_POS_DEFAULT;			//チュートリアルの初期値のセーブポイント
	m_posSavePoint[POINT_ARTBRIDGE] = SAVE_POS_ARTBRIDGE;		//チュートリアルの初期値のセーブポイント
	m_posSavePoint[POINT_ENEMYBRIDGE] = SAVE_POS_ENEMYBRIDGE;	//チュートリアルの初期値のセーブポイント

	m_posSavePoint[POINT_START] = SAVE_POS_START;			//ゲームの初期値のセーブポイント
	m_posSavePoint[POINT_DOWN_FLOOR] = SAVE_POS_DOWN_FLOOR;	//ゲームの1番目のセーブポイント
	m_posSavePoint[POINT_PIER] = SAVE_POS_PIER;				//ゲームの2番目のセーブポイント
	m_posSavePoint[POINT_MAZE] = SAVE_POS_POINT_MAZE;		//ゲームの3番目のセーブポイント
	m_posSavePoint[POINT_BIGINEND] = SAVE_POS_BIGINEND;		//ゲームの4番目のセーブポイント
	m_posSavePoint[POINT_FALLING] = SAVE_POS_FALLING;		//ゲームの5番目のセーブポイント

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最大値
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最小値
	m_rot = rot;		//向き

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		m_apModel[nCntPlayer] = NULL;		//プレイヤー(パーツ)へのポインタ
	}

	//m_pMotion = NULL;		//モーションの情報
	m_nNumModel = 0;		//プレイヤー(パーツ)の総数

	m_nPressCounter = 0;	//キーを押したフレーム数

	m_bJump = false;		//ジャンプしたか
	m_bMove = false;		//歩いてるか
	m_bLand = true;			//着地した
	m_bDash = false;		//ダッシュしたか
	m_bDashAuto = false;	//自動ダッシュするか
	m_bCollisionAlpha = false;		//透明ブロックに当たったか

	m_fRotDest = 0.0f;	//目標
	m_fRotDiff = 0.0f;	//差分

	m_nDashCounter = 0;		//ダッシュした回数

	m_particleType = PARTICLETYPE_NONE;	//パーティクルの種類
	m_nType = 1;				//何番目のパーティクルか
	m_nParticleLife = 50;		//パーティクルの寿命
	m_state = STATE_NONE;		//状態
	m_nCntDamage = 0;			//ダメージカウンター
	m_nCntHit = HIT_CNT;		//攻撃あたるまでのカウンター
	m_nCntMove = STEP_CNT;		//足音のカウンター
	m_bPad = false;				//パッドのスティックを倒してるか
	m_nCntSand = STEP_CNT;		//砂のパーティクルカウンター
	m_nNumPosSave = 0;		//何番目のセーブポイントか

}

//==============================================================
//デストラクタ
//==============================================================
CPlayer::~CPlayer()
{

}

//==============================================================
//プレイヤーの生成処理
//==============================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer *pPlayerModel = NULL;

	if (pPlayerModel == NULL)
	{//メモリが使用されてなかったら

		//プレイヤーの生成
		pPlayerModel = new CPlayer(pos, rot);
	}

	//初期化処理
	pPlayerModel->Init();

	return pPlayerModel;
}

//==============================================================
//プレイヤーの初期化処理
//==============================================================
HRESULT CPlayer::Init(void)
{
	m_fRotDest = m_rot.y;

	//プレイヤーの生成（全パーツ分）
	for (int nCntModel = 0; nCntModel < PARTS_MAX; nCntModel++)
	{
		m_apModel[nCntModel] = m_apModel[nCntModel]->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_apFileName[nCntModel]);
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_apObject3D[nCnt] = CObject3D::Create();
	}

	CObject::SetType(CObject::TYPE_PLAYER);

	return S_OK;
}

//==============================================================
//プレイヤーの終了処理
//==============================================================
void CPlayer::Uninit(void)
{
	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		if (m_apModel[nCntPlayer] != NULL)
		{//使用されてるとき

			//終了処理
			m_apModel[nCntPlayer]->Uninit();
			m_apModel[nCntPlayer] = NULL;
		}
	}

	//if (m_pMotion != NULL)
	//{//使用されてるとき

	//	//モーションの破棄
	//	delete m_pMotion;
	//	m_pMotion = NULL;
	//}

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//プレイヤーの更新処理
//==============================================================
void CPlayer::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得

	//前回の位置更新
	m_posOld = m_pos;

	//手前側の更新処理
	CPlayer::UpdateFront();

	//モーションの更新処理
	//m_pMotion->Update();

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		//プレイヤーの更新処理
		m_apModel[nCntPlayer]->Update();
	}

	//デバッグ表示
	pDebugProc->Print("\nプレイヤーの位置    (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("プレイヤーの移動量    (%f, %f, %f)\n", m_move.x, m_move.y, m_move.z);
	pDebugProc->Print("プレイヤーの向き      (%f, %f, %f)\n", m_rot.x, m_rot.y, m_rot.z);
	pDebugProc->Print("現在のチェックポイント  (%f, %f, %f)\n", m_posSavePoint[m_nNumPosSave].x, m_posSavePoint[m_nNumPosSave].y, m_posSavePoint[m_nNumPosSave].z);

}

//==============================================================
//手前の更新処理
//==============================================================
void CPlayer::UpdateFront(void)
{
	CFade *pFade = CManager::GetInstance()->GetFade();

	if (m_bDashAuto == false && pFade->GetFade() != CFade::FADE_OUT)
	{//自動ダッシュしてないとき

		//プレイヤーの操作
		CPlayer::ControlFrontKeyboard();
		CPlayer::ControlFrontJoyPad();

		//if (m_move.y >= JUMP_HEIGHT ||
		//	(m_bJump == true && (m_move.x <= 7.0f && m_move.x >= -7.0f)) ||
		//	(m_bDash == true && (m_move.x <= 7.0f && m_move.x >= -7.0f)))
		//{
		//	//移動量加算
		//	m_move.y -= MOVE_Y;

		//	if (m_move.y <= 0.0f)
		//	{//着地したら

		//		m_bDash = false;
		//	}
		//}
	}

	//足音鳴らす処理
	CPlayer::SEStep();

	//位置更新
	if (m_bDashAuto == true)
	{//自動ダッシュしてる間

		m_pos += m_moveSave;		//自動ダッシュの移動量加算
	}
	else if (m_bDashAuto == false)
	{//自動ダッシュしてない間

		m_pos += m_move;			//普通の移動量加算
	}

	//当たった時の行動処理
	CPlayer::CollisionAction();

	//状態更新
	CPlayer::UpdateState();

	if (m_bDashAuto == false)
	{
		//移動量を更新
		m_move.x += (0.0f - m_move.x) * 0.1f;
		m_move.y += (0.0f - m_move.y) * 0.1f;
	}

	//向きの補正
	CPlayer::RotCorrection();

	//チェックポイント更新
	CPlayer::SavePoint();

	//画面外処理
	CPlayer::Screen();

	//モーション管理
	//CPlayer::MotionManager();
}

//==============================================================
//状態更新処理
//==============================================================
void CPlayer::UpdateState(void)
{
	CFade *pFade = CManager::GetInstance()->GetFade();

	switch (m_state)
	{
	case STATE_NONE:		//通常状態

		if (m_nCntHit > 0 && pFade->GetFade() == CFade::FADE_NONE)
		{
			m_nCntHit--;		//攻撃あたるまでのカウンター減算
		}

		break;

	case STATE_ATTACK:		//攻撃状態

		break;

	case STATE_DAMAGE:		//ダメージ状態

		m_nCntDamage--;		//ダメージ時間減算
		m_nCntHit--;		//攻撃あたるまでのカウンター減算

		if (m_nCntDamage <= 0)
		{//ダメージ時間が終わったら

			m_state = CObject::STATE_NONE;

			//状態設定
			for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
			{
				m_apModel[nCntPlayer]->SetState(m_state);		//通常状態にする

			}
		}

		break;

	case STATE_APPEAR:		//点滅状態

		if (pFade->GetFade() == CFade::FADE_IN)
		{
			m_state = CObject::STATE_NONE;		//通常状態にする

			m_pos = m_posSavePoint[m_nNumPosSave];		//セーブした場所に戻る
			m_nCntDamage = 0;

			//状態設定
			for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
			{
				m_apModel[nCntPlayer]->SetState(m_state);		//状態設定

			}
		}

		m_nCntDamage--;		//ダメージ時間減算

		if ((m_nCntDamage % 3) == 0)
		{//一定時間経ったら

			//表示切替
			for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
			{
				m_apModel[nCntPlayer]->SetDisp();

			}
		}

		break;

	case STATE_DEATH:		//死亡状態

		break;
	}
}

//==============================================================
//当たった時の行動処理
//==============================================================
void CPlayer::CollisionAction(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();				//パッドの情報取得

	bool bLand = CObjectX::Collision(&m_pos, &m_posOld, &m_move, m_min, m_max);

	//当たり判定
	if (bLand == true)
	{//着地したら

		m_nDashCounter = 0;		//ダッシュ数リセット

		if (m_bLand == false)
		{
			//パーティクルの生成
			//CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), D3DXCOLOR(0.1f, 0.2f, 0.4f, 0.8f), PARTICLETYPE_MOVE, 20, 10.0f);
		}

		m_bJump = false;	//ジャンプしてない状態にする
		m_bLand = true;		//着地した状態にする

	}
	else if (bLand == false &&
		pInputKeyboard->GetPress(DIK_SPACE) == false && pInputJoyPad->GetPress(pInputJoyPad->BUTTON_A, 0) == false)
	{//地面についてない && ジャンプボタン押してない

		m_bJump = true;		//ジャンプしてる状態にする
		m_bLand = false;	//着地してない状態にする
	}
}

//==============================================================
//セーブポイント処理
//==============================================================
void CPlayer::SavePoint(void)
{
	if (m_nNumPosSave < POINT_FALLING)
	{//最大チェックポイント数より少なかったら

		int nNumSave = m_nNumPosSave + 1;		//次のチェックポイント

		if ((m_nNumPosSave + 1) == POINT_ARTBRIDGE || (m_nNumPosSave + 1) == POINT_ENEMYBRIDGE)
		{//次のチェックポイントがチュートリアルの位置だったら

			if (m_posSavePoint[nNumSave].x <= m_pos.x)
			{//次のチェックポイントを通り過ぎたら

				m_nNumPosSave += 1;		//セーブポイントを変更
			}
		}
		else if ((m_nNumPosSave + 1) == POINT_START)
		{//次のチェックポイントがゲーム開始位置だったら

			if (m_posSavePoint[nNumSave].y >= m_pos.y)
			{//次のチェックポイントを通り過ぎたら

				m_nNumPosSave += 1;		//セーブポイントを変更
			}
		}
		else
		{
			if (m_posSavePoint[nNumSave].y >= m_pos.y &&
				m_posSavePoint[nNumSave].x - 100.0f <= m_pos.x &&
				m_posSavePoint[nNumSave].x + 100.0f >= m_pos.x)
			{//次のチェックポイントを通り過ぎたら

				m_nNumPosSave += 1;		//セーブポイントを変更
			}
		}
	}
}

//==============================================================
//モーション管理処理
//==============================================================
void CPlayer::MotionManager(void)
{
	//if (m_bMove == true && m_pMotion->GetType() == m_pMotion->MOTIONTYPE_NEUTRAL)
	//{//歩いてる && 待機状態

	//	//歩かせる
	//	m_pMotion->Set(m_pMotion->MOTIONTYPE_MOVE);
	//}
	//else if (m_pMotion->IsFinish() == true && m_bAction == true)
	//{//攻撃が終わったら

	//	m_bAction = false;		//攻撃してない状態にする
	//}
	//else if ((m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && m_pMotion->IsFinish() == true) ||
	//	(m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && m_bMove == false && m_bJump == false && m_bLand == true))
	//{//モーションが終了したら

	//	//待機状態に戻す
	//	m_pMotion->Set(m_pMotion->MOTIONTYPE_NEUTRAL);
	//}
}

//==============================================================
//向きの補正処理
//==============================================================
void CPlayer::RotCorrection(void)
{
	//向きの差分を求める
	m_fRotDiff = m_fRotDest - m_rot.y;

	//目標の方向までの差分を修正
	if (m_fRotDiff < -D3DX_PI)
	{
		m_fRotDiff += D3DX_PI * 2;
	}
	else if (m_fRotDiff > D3DX_PI)
	{
		m_fRotDiff -= D3DX_PI * 2;
	}

	//差分足す
	m_rot.y += m_fRotDiff * 0.1f;

	//現在の方向修正
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//==============================================================
//足音鳴らす処理
//==============================================================
void CPlayer::SEStep(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	//足音鳴らす
	if (m_bMove == true && m_bJump == false)
	{//歩いてるとき && ジャンプしてない

		if ((m_nCntMove >= STEP_CNT))
		{//一定時間たったら

			//BGM再生
			pSound->Play(pSound->SOUND_LABEL_SE_STEP);

			m_nCntMove = 0;
		}

		m_nCntMove++;		//足音鳴らすカウンター加算
	}
	else if(m_bMove == false)
	{//歩いてないとき

		m_nCntMove = STEP_CNT;
	}

}

//==============================================================
//プレイヤーのキーボード操作処理(手前側)
//==============================================================
void CPlayer::ControlFrontKeyboard(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得

	CSound *pSound = CManager::GetInstance()->GetSound();

	//移動処理
	CPlayer::ControlFrontKeyboardMove();
	
	//ジャンプ処理
	//CPlayer::ControlFrontKeyboardJump();

	////ダッシュ処理
	//if (m_nDashCounter < MAX_DASH)
	//{//最大ジャンプ数未満 && オートダッシュしてる間

	//	CPlayer::ControlFrontKeyboardDash();
	//}
}

//==============================================================
//プレイヤーのキーボードの移動操作処理(手前側)
//==============================================================
void CPlayer::ControlFrontKeyboardMove(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得

	//移動
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{//右
		m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_MOVE;
		m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;

		m_bMove = true;		//歩かせる
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{//左
		m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_MOVE;
		m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;

		m_bMove = true;		//歩かせる
	}

	if ((m_move.x <= STOP_MOVE && m_move.x >= -STOP_MOVE) && (m_move.z <= STOP_MOVE && m_move.z >= -STOP_MOVE))
	{//歩いてないとき

		m_bMove = false;		//歩いてない状態にする
	}
}

//==============================================================
//プレイヤーのキーボードのジャンプ操作処理(手前側)
//==============================================================
void CPlayer::ControlFrontKeyboardJump(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CSound *pSound = CManager::GetInstance()->GetSound();

	if (pInputKeyboard->GetPress(DIK_SPACE) == true && m_bJump == false && m_move.y <= JUMP_HEIGHT)
	{
		//ジャンプする
		m_move.y += ADD_MOVE_Y;

		m_nPressCounter++;		//フレーム数加算

		if (m_move.y >= JUMP_HEIGHT)
		{
			//ジャンプした状態にする
			m_bJump = true;
			m_bLand = false;
		}
	}

	if (pInputKeyboard->GetRelease(DIK_SPACE) == true && m_bJump == false)
	{
		//ジャンプした状態にする
		m_bJump = true;
		m_bLand = false;
	}
}

//==============================================================
//プレイヤーのキーボードのダッシュ操作処理(手前側)
//==============================================================
void CPlayer::ControlFrontKeyboardDash(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得
	CSound *pSound = CManager::GetInstance()->GetSound();

	if (pInputKeyboard->GetPress(DIK_D) == true)
	{//右

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//上
			if (pInputKeyboard->GetTrigger(DIK_J) == true)
			{
				m_move.x = 0.0f;		//移動量リセット
				m_move.y = 0.0f;		//移動量リセット
				m_bDash = true;			//ダッシュした状態にする
				m_bJump = true;			//ジャンプした状態にする

				if (m_bCollisionAlpha == true)
				{//自動ダッシュ

					m_bDashAuto = true;		//自動ダッシュした状態にする

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;

					m_nDashCounter++;		//ダッシュ回数加算

				}
			}
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//下
			if (pInputKeyboard->GetTrigger(DIK_J) == true)
			{
				m_move.x = 0.0f;		//移動量リセット
				m_move.y = 0.0f;		//移動量リセット
				m_bDash = true;			//ダッシュした状態にする
				m_bJump = true;			//ジャンプした状態にする

				if (m_bCollisionAlpha == true)
				{//自動ダッシュ

					m_bDashAuto = true;		//自動ダッシュした状態にする

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if(m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//ダッシュ回数加算

				}
			}
		}
		else if (pInputKeyboard->GetTrigger(DIK_J) == true)
		{
			m_move.x = 0.0f;		//移動量リセット
			m_move.z = 0.0f;		//移動量リセット
			m_bDash = true;			//ダッシュした状態にする
			m_bJump = true;			//ジャンプした状態にする

			if (m_bCollisionAlpha == true)
			{//自動ダッシュ

				m_bDashAuto = true;		//自動ダッシュした状態にする

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//ダッシュ回数加算

			}
		}
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{//左
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//上
			if (pInputKeyboard->GetTrigger(DIK_J) == true)
			{
				m_move.x = 0.0f;		//移動量リセット
				m_move.y = 0.0f;		//移動量リセット
				m_bDash = true;			//ダッシュした状態にする
				m_bJump = true;			//ジャンプした状態にする

				if (m_bCollisionAlpha == true)
				{//自動ダッシュ

					m_bDashAuto = true;		//自動ダッシュした状態にする

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//ダッシュ回数加算
				}
			}
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//下
			if (pInputKeyboard->GetTrigger(DIK_J) == true)
			{
				m_move.x = 0.0f;		//移動量リセット
				m_move.y = 0.0f;		//移動量リセット
				m_bDash = true;			//ダッシュした状態にする
				m_bJump = true;			//ジャンプした状態にする

				if (m_bCollisionAlpha == true)
				{//自動ダッシュ

					m_bDashAuto = true;		//自動ダッシュした状態にする

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//ダッシュ回数加算
				}
			}
		}
		else if (pInputKeyboard->GetTrigger(DIK_J) == true)
		{
			m_move.x = 0.0f;		//移動量リセット
			m_move.z = 0.0f;		//移動量リセット
			m_bDash = true;			//ダッシュした状態にする
			m_bJump = true;			//ジャンプした状態にする

			if (m_bCollisionAlpha == true)
			{//自動ダッシュ

				m_bDashAuto = true;		//自動ダッシュした状態にする

				m_moveSave.x = sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//ダッシュ回数加算
			}
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{//上
		if (pInputKeyboard->GetTrigger(DIK_J) == true)
		{
			m_move.x = 0.0f;		//移動量リセット
			m_move.y = 0.0f;		//移動量リセット
			m_bDash = true;			//ダッシュした状態にする
			m_bJump = true;			//ジャンプした状態にする

			if (m_bCollisionAlpha == true)
			{//自動ダッシュ

				m_bDashAuto = true;		//自動ダッシュした状態にする

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -CURVE_UP) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -CURVE_UP) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * FRONT_DASH_MOVE;
				m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//ダッシュ回数加算
			}
		}
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{//下
		if (pInputKeyboard->GetTrigger(DIK_J) == true)
		{
			m_move.x = 0.0f;		//移動量リセット
			m_move.y = 0.0f;		//移動量リセット
			m_bDash = true;			//ダッシュした状態にする
			m_bJump = true;			//ジャンプした状態にする

			if (m_bCollisionAlpha == true)
			{//自動ダッシュ

				m_bDashAuto = true;		//自動ダッシュした状態にする

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -CURVE_DOWN) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -CURVE_DOWN) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * FRONT_DASH_MOVE;
				m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//ダッシュ回数加算
			}
		}
	}
	else if (pInputKeyboard->GetTrigger(DIK_J) == true)
	{//Jキーだけを押したとき

		if (m_rot.y > 0)
		{//プレイヤーの向きが左だったら

			m_move.x = 0.0f;		//移動量リセット
			m_move.z = 0.0f;		//移動量リセット

			if (m_bCollisionAlpha == true)
			{//自動ダッシュ

				m_bDashAuto = true;		//自動ダッシュした状態にする

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -CURVE_RL) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//ダッシュ回数加算
			}
		}
		else if (m_rot.y <= 0)
		{
			m_move.x = 0.0f;		//移動量リセット
			m_move.z = 0.0f;		//移動量リセット

			if (m_bCollisionAlpha == true)
			{//自動ダッシュ

				m_bDashAuto = true;		//自動ダッシュした状態にする

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//ダッシュ回数加算
			}
		}

		m_bDash = true;		//ダッシュした状態にする
		m_bJump = true;		//ジャンプした状態にする
	}

	if (m_bDashAuto == true)
	{
		//パーティクルの生成
		CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PARTICLETYPE_MOVE, 20, 30.0f);
	}

	if (m_bDash == true && m_bJump == true)
	{
		//パーティクルの生成
		CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PARTICLETYPE_MOVE, 20, 20.0f);

	}
}

//==============================================================
//プレイヤーのパッド操作処理(手前側)
//==============================================================
void CPlayer::ControlFrontJoyPad(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	//移動処理
	CPlayer::ControlFrontJoyPadMove();

	//ジャンプ処理
	//CPlayer::ControlFrontJoyPadJump();

	////ダッシュ処理
	//if (m_nDashCounter < MAX_DASH)
	//{//最大ジャンプ数未満 && オートダッシュしてる間

	//	CPlayer::ControlFrontJoyPadDash();
	//}
}

//==============================================================
//プレイヤーのパッドのジャンプ操作処理(手前側)
//==============================================================
void CPlayer::ControlFrontJoyPadJump(void)
{
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();				//パッドの情報取得
	CSound *pSound = CManager::GetInstance()->GetSound();

	if (pInputJoyPad->GetPress(pInputJoyPad->BUTTON_A, 0) == true && m_bJump == false && m_move.y <= JUMP_HEIGHT)
	{
		//ジャンプする
		m_move.y += ADD_MOVE_Y;

		m_nPressCounter++;		//フレーム数加算

		if (m_move.y >= JUMP_HEIGHT)
		{
			//ジャンプした状態にする
			m_bJump = true;
			m_bLand = false;
		}
	}

	if (pInputJoyPad->GetRelease(pInputJoyPad->BUTTON_A, 0) == true && m_bJump == false)
	{
		//ジャンプした状態にする
		m_bJump = true;
		m_bLand = false;
	}
}

//==============================================================
//プレイヤーのパッドのダッシュ操作処理(手前側)
//==============================================================
void CPlayer::ControlFrontJoyPadDash(void)
{
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();				//パッドの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得
	CSound *pSound = CManager::GetInstance()->GetSound();

	if (pInputJoyPad->GetPressLX(0).x > 0.0f)
	{//右

		if (pInputJoyPad->GetPressLX(0).y > 0.0f)
		{//上
			if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
			{
				m_move.x = 0.0f;		//移動量リセット
				m_move.y = 0.0f;		//移動量リセット
				m_bDash = true;			//ダッシュした状態にする
				m_bJump = true;			//ジャンプした状態にする

				if (m_bCollisionAlpha == true)
				{//自動ダッシュ

					m_bDashAuto = true;		//自動ダッシュした状態にする

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//ダッシュ回数加算

				}
			}
		}
		else if (pInputJoyPad->GetPressLX(0).y < 0.0f)
		{//下
			if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
			{
				m_move.x = 0.0f;		//移動量リセット
				m_move.y = 0.0f;		//移動量リセット
				m_bDash = true;			//ダッシュした状態にする
				m_bJump = true;			//ジャンプした状態にする

				if (m_bCollisionAlpha == true)
				{//自動ダッシュ

					m_bDashAuto = true;		//自動ダッシュした状態にする

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//ダッシュ回数加算

				}
			}
		}
		else if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
		{
			m_move.x = 0.0f;		//移動量リセット
			m_move.z = 0.0f;		//移動量リセット
			m_bDash = true;			//ダッシュした状態にする
			m_bJump = true;			//ジャンプした状態にする

			if (m_bCollisionAlpha == true)
			{//自動ダッシュ

				m_bDashAuto = true;		//自動ダッシュした状態にする

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//ダッシュ回数加算

			}
		}
	}
	else if (pInputJoyPad->GetPressLX(0).x < 0.0f)
	{//左
		if (pInputJoyPad->GetPressLX(0).y > 0.0f)
		{//上
			if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
			{
				m_move.x = 0.0f;		//移動量リセット
				m_move.y = 0.0f;		//移動量リセット
				m_bDash = true;			//ダッシュした状態にする
				m_bJump = true;			//ジャンプした状態にする

				if (m_bCollisionAlpha == true)
				{//自動ダッシュ

					m_bDashAuto = true;		//自動ダッシュした状態にする

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//ダッシュ回数加算
				}
			}
		}
		else if (pInputJoyPad->GetPressLX(0).y < 0.0f)
		{//下
			if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
			{
				m_move.x = 0.0f;		//移動量リセット
				m_move.y = 0.0f;		//移動量リセット
				m_bDash = true;			//ダッシュした状態にする
				m_bJump = true;			//ジャンプした状態にする

				if (m_bCollisionAlpha == true)
				{//自動ダッシュ

					m_bDashAuto = true;		//自動ダッシュした状態にする

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;

					//BGM再生
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//ダッシュ回数加算
				}
			}
		}
		else if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
		{
			m_move.x = 0.0f;		//移動量リセット
			m_move.z = 0.0f;		//移動量リセット
			m_bDash = true;			//ダッシュした状態にする
			m_bJump = true;			//ジャンプした状態にする

			if (m_bCollisionAlpha == true)
			{//自動ダッシュ

				m_bDashAuto = true;		//自動ダッシュした状態にする

				m_moveSave.x = sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//ダッシュ回数加算
			}
		}
	}
	else if (pInputJoyPad->GetPressLX(0).y > 0.0f)
	{//上
		if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
		{
			m_move.x = 0.0f;		//移動量リセット
			m_move.y = 0.0f;		//移動量リセット
			m_bDash = true;			//ダッシュした状態にする
			m_bJump = true;			//ジャンプした状態にする

			if (m_bCollisionAlpha == true)
			{//自動ダッシュ

				m_bDashAuto = true;		//自動ダッシュした状態にする

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -CURVE_UP) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -CURVE_UP) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * FRONT_DASH_MOVE;
				m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//ダッシュ回数加算
			}
		}
	}
	else if (pInputJoyPad->GetPressLX(0).y < 0.0f)
	{//下
		if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
		{
			m_move.x = 0.0f;		//移動量リセット
			m_move.y = 0.0f;		//移動量リセット
			m_bDash = true;			//ダッシュした状態にする
			m_bJump = true;			//ジャンプした状態にする

			if (m_bCollisionAlpha == true)
			{//自動ダッシュ

				m_bDashAuto = true;		//自動ダッシュした状態にする

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -CURVE_DOWN) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -CURVE_DOWN) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * FRONT_DASH_MOVE;
				m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//ダッシュ回数加算
			}
		}
	}
	else if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
	{//Jキーだけを押したとき

		if (m_rot.y > 0)
		{//プレイヤーの向きが左だったら

			m_move.x = 0.0f;		//移動量リセット
			m_move.z = 0.0f;		//移動量リセット

			if (m_bCollisionAlpha == true)
			{//自動ダッシュ

				m_bDashAuto = true;		//自動ダッシュした状態にする

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -CURVE_RL) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//ダッシュ回数加算
			}
		}
		else if (m_rot.y <= 0)
		{
			m_move.x = 0.0f;		//移動量リセット
			m_move.z = 0.0f;		//移動量リセット

			if (m_bCollisionAlpha == true)
			{//自動ダッシュ

				m_bDashAuto = true;		//自動ダッシュした状態にする

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;

				//BGM再生
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//ダッシュ回数加算
			}
		}

		m_bDash = true;		//ダッシュした状態にする
		m_bJump = true;		//ジャンプした状態にする
	}
}

//==============================================================
//プレイヤーのパッドの移動操作処理(手前側)
//==============================================================
void CPlayer::ControlFrontJoyPadMove(void)
{
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();				//パッドの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得

	//移動
	if (pInputJoyPad->GetPressLX(0).x > 0.0f)
	{//右

		if (pInputJoyPad->GetPressLX(0).y > 0.0f)
		{//上

			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * -0.25f) * FRONT_MOVE;
			m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * -0.25f) * FRONT_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -0.75f;

			m_bMove = true;		//歩かせる
		}
		else if (pInputJoyPad->GetPressLX(0).y < 0.0f)
		{//下

			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * -0.75f) * FRONT_MOVE;
			m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * -0.75f) * FRONT_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -0.25f;

			m_bMove = true;		//歩かせる
		}
		else
		{
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;
		}

		m_bMove = true;		//歩かせる
	}
	else if (pInputJoyPad->GetPressLX(0).x < 0.0f)
	{//左

		if (pInputJoyPad->GetPressLX(0).y > 0.0f)
		{//上

			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * 0.25f) * FRONT_MOVE;
			m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * 0.25f) * FRONT_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.75f;

			m_bMove = true;		//歩かせる
		}
		else if (pInputJoyPad->GetPressLX(0).y < 0.0f)
		{//下

			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * 0.75f) * FRONT_MOVE;
			m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * 0.75f) * FRONT_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.25f;

			m_bMove = true;		//歩かせる
		}
		else
		{
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;
		}

		m_bMove = true;		//歩かせる
	}
	else if (pInputJoyPad->GetPressLX(0).y > 0.0f)
	{//上

		m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * FRONT_MOVE;
		m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * FRONT_MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_UP;

		m_bMove = true;		//歩かせる
	}
	else if (pInputJoyPad->GetPressLX(0).y < 0.0f)
	{//下

		m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * FRONT_MOVE;
		m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * FRONT_MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_UP;

		m_bMove = true;		//歩かせる
	}

	if ((m_move.x <= STOP_MOVE && m_move.x >= -STOP_MOVE) && (m_move.z <= STOP_MOVE && m_move.z >= -STOP_MOVE))
	{//歩いてないとき

		m_bMove = false;		//歩いてない状態にする
	}
}

//==============================================================
//プレイヤーの自動ダッシュ処理
//==============================================================
void CPlayer::SetDashAuto(bool bDashAuto)
{ 
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得

	m_bCollisionAlpha = bDashAuto;

	if (m_bCollisionAlpha == true)
	{//透明ブロックに当たった時

		m_nDashCounter = 0;		//ダッシュカウンターをリセット
	}
	else if (m_bCollisionAlpha == false)
	{//透明ブロックに当たってないとき

		m_bDashAuto = false;		//自動ダッシュしない状態にする
	}
}

//==============================================================
//画面外判定処理
//==============================================================
void CPlayer::Screen(void)
{
	if (m_pos.y <= 0.0f && m_pos.x <= 35.0f)
	{//壁通り抜けたら

		m_pos.x = 35.0f;
		m_move.x = 0.0f;
	}

	if (m_pos.y <= 0.0f && m_pos.x >= 1330.0f)
	{//壁通り抜けたら

		m_pos.x = 1300.0f;
		m_move.x = 0.0f;
	}

	if (m_pos.y <= -6000.0f)
	{//一番下まで行ったら

		CFade *pFade = CManager::GetInstance()->GetFade();			//フェードの情報取得
		CDeathUI *pDeathUI = CGame::GetDeathUI();
		CItemUI *pItemUI = CGame::GetItemUI();

		//スコア保存
		CManager::GetInstance()->SetNumDeath(pDeathUI->Get());
		CManager::GetInstance()->SetNumItem(pItemUI->Get());

		//リザルトの判定
		CManager::GetInstance()->SetResult(true);

		//ゲーム画面
		pFade->SetFade(CScene::MODE_RESULT);
	}
}

//==============================================================
//プレイヤーの描画処理
//==============================================================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスを初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		//プレイヤーの描画
		m_apModel[nCntPlayer]->Draw();
	}
}

//==============================================================
//プレイヤーのヒット処理
//==============================================================
void CPlayer::Hit(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();
	CDeathUI *pDeathUI = CGame::GetDeathUI();
	CFade *pFade = CManager::GetInstance()->GetFade();

	if (m_state != CObject::STATE_DAMAGE && m_state != CObject::STATE_APPEAR && m_nCntDamage <= 0 && m_nCntHit <= 0 &&
		pFade->GetFade() != CFade::FADE_IN)
	{
		//BGM再生
		pSound->Play(pSound->SOUND_LABEL_SE_DAMAGE001);

		m_state = CObject::STATE_APPEAR;

		m_nCntDamage = HIT_CNT;				//ダメージ状態を保つ時間設定
		m_nCntHit = 60;				//攻撃あたるまでのカウンター

		pDeathUI->Add(1);		//死亡数加算

		//フェードさせる
		pFade->SetNormalFade();

		for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
		{
			//プレイヤーの色設定
			m_apModel[nCntPlayer]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		}

		//パーティクル生成
		//CParticle::Create(m_pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PARTICLETYPE_EXPLOSION, 30, 40);

		//状態設定
		for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
		{
			m_apModel[nCntPlayer]->SetState(m_state);		//ダメージ状態にする
		}
	}
}

//==============================================================
//モデルファイル読み込み処理
//==============================================================
void CPlayer::LoadFile(void)
{
	FILE *pFile;				//ファイルポインタ
	char aString[MAX_STR];		//文字読み込み
	int nIndex = 0, nParent = 0;	//パーツNo.,親番号
	D3DXVECTOR3 pos, rot;

	//ファイル開く
	pFile = fopen(FILE_HUMAN, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		while (strcmp(&aString[0], "CHARACTERSET") != 0)
		{//[CHARACTERSET]するまでの間

			fscanf(pFile, "%s", &aString[0]);		//文字読み込み
		}

		if (strcmp(&aString[0], "CHARACTERSET") == 0)
		{//[CHARACTERSET]が来たら

			while (strcmp(&aString[0], "END_CHARACTERSET") != 0)
			{//[END_CHARACTERSET]がくるまでの間

				fscanf(pFile, "%s", &aString[0]);		//文字読み込み

				if (strcmp(&aString[0], "PARTSSET") == 0)
				{//[PARTSSET]が来たら

					while (strcmp(&aString[0], "END_PARTSSET") != 0)
					{//[END_PARTSSET]がくるまでの間

						fscanf(pFile, "%s", &aString[0]);		//文字読み込み

						if (strcmp(&aString[0], "INDEX") == 0)
						{//パーツNo.

							fscanf(pFile, "%s", &aString[0]);		//文字読み込み
							fscanf(pFile, "%d", &nIndex);			//パーツNo.読み込み

						}
						else if (strcmp(&aString[0], "PARENT") == 0)
						{//親情報

							fscanf(pFile, "%s", &aString[0]);		//文字読み込み
							fscanf(pFile, "%d", &nParent);			//親番号読み込み

							if (nParent == -1)
							{//親がいなかったら

								m_apModel[nIndex]->SetParent(NULL);		//NULLを入れる
							}
							else
							{//親がいたら

								m_apModel[nIndex]->SetParent(m_apModel[nParent]);		//親番号入れる
							}
						}
						else if (strcmp(&aString[0], "POS") == 0)
						{//位置情報

							fscanf(pFile, "%s", &aString[0]);	//文字読み込み

							fscanf(pFile, "%f", &pos.x);		//位置読み込み
							fscanf(pFile, "%f", &pos.y);		//位置読み込み
							fscanf(pFile, "%f", &pos.z);		//位置読み込み

							m_apModel[nIndex]->SetPosition(pos);		//位置設定
							m_apModel[nIndex]->SetDefaultPosition(pos);	//初期位置設定

						}
						else if (strcmp(&aString[0], "ROT") == 0)
						{//向き情報

							fscanf(pFile, "%s", &aString[0]);	//文字読み込み

							fscanf(pFile, "%f", &rot.x);		//向き読み込み
							fscanf(pFile, "%f", &rot.y);		//向き読み込み
							fscanf(pFile, "%f", &rot.z);		//向き読み込み

							m_apModel[nIndex]->SetRotation(rot);		//向き設定
							m_apModel[nIndex]->SetDefaultRotation(rot);	//初期向き設定
						}
					}
				}
			}
		}

		//ファイル閉じる
		fclose(pFile);
	}
}