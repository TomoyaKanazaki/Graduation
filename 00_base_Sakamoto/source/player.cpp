//============================================
//
//	プレイヤーの処理 [player.cpp]
//	Author:sakamoto kai
//
//============================================
#include "player.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "debugproc.h"
#include "camera.h"
#include "input.h"
#include "enemy.h"
#include "slowManager.h"
#include "Number.h"
#include "sound.h"
#include "LifeUi.h"
#include "cross.h"
#include "bowabowa.h"
#include "fire.h"
#include "DevilHole.h"
#include "devil.h"
#include "bible.h"
#include "RailBlock.h"
#include "objmeshField.h"
#include "RollRock.h"
#include "shadow.h"
#include "score.h"
#include "mask.h"
#include "wall.h"
#include "objectBillboard.h"
#include "move.h"
#include "MapMove.h"
#include "select.h"

#include "MyEffekseer.h"
#include "footprint.h"

//===========================================
// 定数定義
//===========================================
namespace
{
	const int LIFE_MAX = 2;	//初期ライフ数
	const int FIRE_STOPTIME = 30;	//攻撃時の移動停止時間
	const D3DXVECTOR3 RESPAWN_POS01 = D3DXVECTOR3(-200.0f, 1700.0f, 100.0f); // 復活位置(1P)
	const D3DXVECTOR3 RESPAWN_POS02 = D3DXVECTOR3(+200.0f, 1700.0f, 100.0f); // 復活位置(2P)
	const float RESPAWN_GRAVITY = 0.35f;			//卵の重力
	const int INVINCIBLE_TIME = 120;			//無敵時間(後で消す)

	const float GRIT_OK = 45.0f;			//移動可能なグリットの範囲内
	const float OBJDISTANCE = 10000.0f;		// オブジェクトの距離

	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(35.0f, 40.0f, 35.0f);		//横の当たり判定

	const float CROSS_TIME = 10.0f; // 十字架を所持していられる時間

	const float EGG_GRAVITY = 0.98f;	 //移動量の減衰速度
	const D3DXVECTOR3 EGG_MOVE = D3DXVECTOR3(10.0f, 10.0f, 10.0f);	 //移動量の減衰速度(後で消す)
	const float EGG_ROT = D3DX_PI * 0.006f;		//回転速度
	const float EGG_MOVE_DEL = 0.9f;			//移動量の減衰速度
	const float EGG_COLOR_DEL_A = 0.01f;		//不透明度の減衰速度

	const float SHADOW_SIZE = 50.0f;			// 丸影の大きさ
	const float EFFECT_HEIGHT = 50.0f;			// エフェクトの高さ

	const D3DXVECTOR2 NUMBER_SIZE = { 280.0f , 170.0f };
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CPlayer>* CPlayer::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CPlayer::CPlayer(int nPriority) : CObjectCharacter(nPriority),
m_AutoMoveRot(INITVECTOR3),
m_bJump(false),
m_nActionCount(0),
m_Action(ACTION_WAIT),
m_AtkAction(ACTION_WAIT),
m_nStateCount(0),
m_AtkPos(INITVECTOR3),
m_CollisionRot(0.0f),
m_bInput(false),
m_nLife(0),
m_eItemType(TYPE_NONE),
m_OldGrid(0, 0),
m_bGritCenter(true),
m_bPressObj(false),
m_fCrossTimer(0.0f),
m_pUpEgg(nullptr),
m_pDownEgg(nullptr),
//m_bInvincible(true),
//m_nInvincibleCount(0),
m_nTime(0),
m_pEffectEgg(nullptr),
m_pEffectSpeed(nullptr),
m_pP_NumUI(nullptr),
m_pEffectGuide(nullptr),
m_pEffectItem(nullptr),
m_pMoveState(nullptr)
{
	// 移動の進行状況
	m_Progress.bOKD = true;
	m_Progress.bOKL = true;
	m_Progress.bOKR = true;
	m_Progress.bOKU = true;
}

//====================================================================
//デストラクタ
//====================================================================
CPlayer::~CPlayer()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CPlayer::Init(int PlayNumber)
{
	// 値を取得
	D3DXVECTOR3 posThis = GetPos();			// 位置
	D3DXVECTOR3 posOldThis = GetPosOld();	// 前回の位置
	D3DXVECTOR3 rotThis = GetRot();			// 向き
	D3DXVECTOR3 sizeThis = GetSize();		// 大きさ

	// プレイヤー番号を設定
	m_nPlayNumber = PlayNumber;
	m_bPressObj = false;

	// キャラクタークラスの初期化（継承）
	if (FAILED(CObjectCharacter::Init())) { assert(false); }

	// キャラクターテキスト読み込み処理
	switch (m_nPlayNumber)
	{
	case 0:

		CObjectCharacter::SetTxtCharacter("data\\TXT\\motion_tamagon1P.txt");

		break;

	case 1:

		CObjectCharacter::SetTxtCharacter("data\\TXT\\motion_tamagon2P.txt");

		break;
	}

	// キャラクターのマトリックス設定
	CObjectCharacter::SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
	CObjectCharacter::SetUseShadowMtx(true);

	CMapSystem* pMapSystem = CMapSystem::GetInstance();		// マップシステムの情報
	int nSelectMap = CMapSystem::GetInstance()->GetSelectMap();

	// プレイヤーの位置取得
	posThis = pMapSystem->GetPlayerPos(PlayNumber, nSelectMap);
	SetGrid(pMapSystem->GetPlayerGrid(PlayNumber, nSelectMap));
	SetGridOld(pMapSystem->GetPlayerGrid(PlayNumber, nSelectMap));

	// サイズの設定
	sizeThis = COLLISION_SIZE;

	// 向きの設定
	rotThis = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

	// アクションの設定
	m_Action = ACTION_WAIT;
	m_AtkAction = ACTION_WAIT;

	//種類設定
	SetType(CObject::TYPE_PLAYER3D);

	// 体力の設定
	m_nLife = LIFE_MAX;

	//所持するUIの生成
	UI_Create();

	// アイテム状態を設定
	SetItemType(CPlayer::TYPE_NONE);

	// スローの生成
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_PLAYER, CSlowManager::TAG_PLAYER);

	// 値更新
	SetPos(posThis);		// 位置
	SetPosOld(posOldThis);	// 前回の位置
	SetRot(rotThis);		// 向き
	SetSize(sizeThis);		// 大きさ

	// 状態の設定
	SetState(STATE_EGG);

	// 移動状態の設定
	if (m_pMoveState == nullptr)
	{
		m_pMoveState = new CStateStop();	// 停止状態
		m_pMoveState->ControlStop(this);	// 操作状態にする
	}
	//// 向き状態の設定
	//m_pMoveState->SetRotState(CMoveState::ROTSTATE_WAIT);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CPlayer>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// スクロールをオンにする
	SetMapScroll(true);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CPlayer::Uninit(void)
{
	if (m_pList != nullptr)
	{
		// リストから自身のオブジェクトを削除
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // オブジェクトが一つもない場合

			// リストマネージャーの破棄
			m_pList->Release(m_pList);
		}
	}

	// キャラクタークラスの終了（継承）
	CObjectCharacter::Uninit();

	// 移動状態の破棄
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	// エフェクトの削除
	if (m_pEffectEgg != nullptr)
	{
		m_pEffectEgg->SetDeath();
		m_pEffectEgg = nullptr;
	}
	if (m_pEffectSpeed != nullptr)
	{
		m_pEffectSpeed->SetDeath();
		m_pEffectSpeed = nullptr;
	}
	if (m_pEffectItem != nullptr)
	{
		m_pEffectItem->SetDeath();
		m_pEffectItem = nullptr;
	}
	if (m_pEffectGuide != nullptr)
	{
		m_pEffectGuide->SetDeath();
		m_pEffectGuide = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CPlayer::Update(void)
{
	// 値を取得
	D3DXVECTOR3 posThis = GetPos();			// 位置
	D3DXVECTOR3 posOldThis = GetPosOld();	// 前回の位置
	D3DXVECTOR3 rotThis = GetRot();			// 向き
	D3DXVECTOR3 sizeThis = GetSize();		// 大きさ
	STATE state = GetState();				// 状態
	STATE oldstate = GetOldState();			// 状態

	// 過去の位置に代入
	posOldThis = posThis;
	m_OldGrid = m_Grid;

	if (state != STATE_DEATH)
	{
		//壁があるか判断
		SearchWall(posThis);

		if (
			(state != STATE_EGG/* &&
				CMapSystem::GetInstance()->GetGritBool(m_Grid.x, m_Grid.z) == false*/) ||
			(state == STATE_EGG && CollisionStageIn(posThis) == true/* &&
				CMapSystem::GetInstance()->GetGritBool(m_Grid.x, m_Grid.z) == false*/ &&
				m_bGritCenter == true && posThis.y <= 0.0f)
			)
		{// ステージ内にいる かつ ブロックの無いグリッド上の時

			if (CScene::GetMode() == CScene::MODE_GAME)
			{
				if (CGame::GetInstance()->GetEvent() == true)
				{
					CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_SE_WALK);
				}
				else
				{
					// 移動処理
					m_pMoveState->Move(this, posThis, rotThis);
					state = GetState();				// 状態
				}
			}
			else
			{
				// 移動処理
				m_pMoveState->Move(this, posThis, rotThis);
				state = GetState();				// 状態
			}

			// モデルを描画する
			if (GetState() != STATE_EGG && oldstate == STATE_EGG)
			{
				SetItemType(TYPE_NONE);
				CMapSystem::GetInstance()->SetGritBool(m_Grid.x, m_Grid.z, false);

			}
			else if (GetState() == STATE_EGG)
			{
				CMapSystem::GetInstance()->SetGritBool(m_Grid.x, m_Grid.z, true);
			}
		}

		// 向き移動処理
		Rot(rotThis);

		if (m_eItemType != TYPE_NONE &&
			CGame::GetInstance()->GetEvent() == false)
		{
			// 攻撃
			Attack(posThis,rotThis);
		}

		// 十字架を持っている場合
		if (m_eItemType == TYPE_CROSS)
		{
			// タイマーを加算
			m_fCrossTimer += DeltaTime::Get();
			DebugProc::Print(DebugProc::POINT_CENTER, "残り時間 : %f", m_fCrossTimer);

			// 十字架の所持可能時間を超過した場合
			if (m_fCrossTimer >= CROSS_TIME)
			{
				// 所持時間タイマーをリセット
				m_fCrossTimer = 0.0f;

				// エフェクトを生成
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_CRASH_CROSS, false, ef, rotThis);

				// アイテムを所持していない状態にする
				SetItemType(TYPE_NONE);
			}
		}

		// カメラ更新処理
		CameraPosUpdate(posThis);

		if (state == STATE_WALK || state == STATE_WAIT)
		{
			// 位置更新処理
			PosUpdate(posThis, posOldThis, sizeThis);
		}

		if (state != STATE_EGG && state != STATE_DEATH)
		{
			//画面外判定
			CollisionStageOut(posThis);

			if (m_pP_NumUI != nullptr)
			{
				m_pP_NumUI->SetPos(D3DXVECTOR3(
					posThis.x,
					posThis.y + 50.0f,
					posThis.z + 50.0f));
			}
		}

		// 敵の判定
		CollisionEnemy(posThis);
	}

	// プレイヤーがマップのどのマスに存在しているか設定する
	m_Grid.x = CMapSystem::GetInstance()->CMapSystem::CalcGridX(posThis.x);
	m_Grid.z = CMapSystem::GetInstance()->CMapSystem::CalcGridZ(posThis.z);

	//状態の管理
	StateManager(posThis, rotThis);

	int nInvincibleCount = GetInvincibleCount();
	bool bInvincible = GetInvincible();

	if (nInvincibleCount > 0)
	{
		nInvincibleCount--;
		SetInvincibleCount(nInvincibleCount);
	}
	else
	{
		bInvincible = false;
		SetInvincible(bInvincible);
	}

	if (bInvincible)
	{
		SetModelColor(CModel::COLORTYPE_TRUE_ALL, D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
	}
	else
	{
		SetModelColor(CModel::COLORTYPE_FALSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//卵の動き
	EggMove(posThis, rotThis);

	//モーションの管理
	ActionState();

	// エフェクトの操作
	SpeedEffect(posThis, rotThis);
	ControlEffect(m_pEffectEgg);	// 卵のエフェクト
	D3DXVECTOR3 temp = posThis;
	temp.y += EFFECT_HEIGHT;
	MoveEffect(m_pEffectSpeed, &temp);	// 加減速のエフェクト
	ControlEffect(m_pEffectItem);	// アイテム所持エフェクト
	if (m_pShadow != nullptr)
	{
		ControlEffect(m_pEffectGuide, &m_pShadow->GetPos()); // 復活位置のガイドエフェクト
	}
	PrintFoot(rotThis);

	// 値更新
	SetPos(posThis);		// 位置
	SetPosOld(posOldThis);	// 前回の位置
	SetRot(rotThis);		// 向き
	SetSize(sizeThis);		// 大きさ

	// キャラクタークラスの更新（継承）
	CObjectCharacter::Update();

	//デバッグ表示
	DebugProc::Print(DebugProc::POINT_LEFT, "[自分]位置 %f : %f : %f\n", posThis.x, posThis.y, posThis.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[自分]向き %f : %f : %f\n", rotThis.x, rotThis.y, rotThis.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[自分]横 %d : 縦 %d\n", m_Grid.x, m_Grid.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[自分]状態 : ");
	auto str = magic_enum::enum_name(state);
	DebugProc::Print(DebugProc::POINT_LEFT, str.data());
	DebugProc::Print(DebugProc::POINT_LEFT, "\n");
}

//====================================================================
//描画処理
//====================================================================
void CPlayer::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 => (参照値 => ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// キャラクタークラスの描画（継承）
	CObjectCharacter::Draw();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 11);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 => (参照値 => ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// キャラクタークラスの描画（継承）
	CObjectCharacter::Draw();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 13);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 => (参照値 => ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// キャラクタークラスの描画（継承）
	CObjectCharacter::Draw();

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//====================================================================
//所持するUIの生成
//====================================================================
void CPlayer::UI_Create(void)
{
	//プレイヤー番号UIの生成
	if (m_pP_NumUI == nullptr && CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
	{
		m_pP_NumUI = CObjectBillboard::Create();
		m_pP_NumUI->SetWidth(150.0f);
		m_pP_NumUI->SetHeight(150.0f);
		m_pP_NumUI->SetAppear(false);

		switch (m_nPlayNumber)
		{
		case 0:
			m_pP_NumUI->SetTexture("data\\TEXTURE\\UI\\1p.png");
			break;

		case 1:
			m_pP_NumUI->SetTexture("data\\TEXTURE\\UI\\2p.png");
			break;
		}
	}
}

//====================================================================
//移動方向処理
//====================================================================
void CPlayer::Rot(D3DXVECTOR3& rotThis)
{
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera(0)->GetRot();
	STATE state = GetState();		// 状態

	//移動方向に向きを合わせる処理
	float fRotMove, fRotDest;
	fRotMove = rotThis.y;
	fRotDest = CManager::GetInstance()->GetCamera(0)->GetRot().y;

	if (state == STATE_WALK)
	{
		rotThis.y = atan2f(-m_move.x, -m_move.z);
	}

	useful::NormalizeAngle(&rotThis);
}

//====================================================================
//攻撃処理
//====================================================================
void CPlayer::Attack(D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis)
{
	STATE state = GetState();		// 状態

	if (state == STATE_WALK)
	{
		//キーボードの取得
		CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
		CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();


		if (pInputKeyboard->GetTrigger(DIK_SPACE)
			|| pInputJoypad->GetTrigger(CInputJoypad::BUTTON_A, m_nPlayNumber))
		{
			// 火炎放射
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_FIRE);
			D3DXMATRIX mat = *GetUseMultiMatrix();
			D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);

			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_SMOKE, false, ef, rotThis);

			CFire::Create("data\\model\\fireball.x", posThis, rotThis);
			SetState(STATE_ATTACK);
			m_nStateCount = FIRE_STOPTIME;
		}
	}
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CPlayer::ActionState(void)
{
	// モーションの取得
	CMotion* pMotion = GetMotion();
	STATE state = GetState();

	if (pMotion == nullptr)
	{
		return;
	}

	//移動モーション
	if (state == STATE_DEATH)
	{
		if (m_Action != ACTION_ENEMYDEATH)
		{
			m_Action = ACTION_ENEMYDEATH;
			pMotion->Set(ACTION_ENEMYDEATH, 5);
		}
	}
	//卵モーション
	else if (state == STATE_EGG)
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			pMotion->Set(ACTION_WAIT, 5);
		}
	}
	//移動モーション
	else if (state == STATE_ATTACK)
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			pMotion->Set(ACTION_WAIT, 5);
		}
	}
	//移動モーション
	else if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			pMotion->Set(ACTION_MOVE, 5);
		}
	}
	//ニュートラルモーション
	else
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			pMotion->Set(ACTION_WAIT, 5);
		}
	}
}

//====================================================================
//状態管理
//====================================================================
void CPlayer::StateManager(D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis)
{
	STATE state = GetState();

	switch (state)
	{
	case STATE_WAIT:
		//	スローをdefaultへ
		CSlowManager::SetValueDefault();
		break;

	case STATE_WALK:

		m_nTime++;

		if (m_nTime >= 20)
		{
			// サウンドの再生
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
			D3DXMATRIX mat = *GetUseMultiMatrix();
			D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotThis);

			m_nTime = 0;
		}
		break;

	case STATE_ATTACK:

		if (m_nStateCount == 0)
		{
			SetState(STATE_WALK);
		}

		break;

	case STATE_DEATH:
		if (m_nStateCount == 0)
		{
			//復活処理
			Reivel(posThis);
		}

		break;

	case STATE_EGG:

		//卵の画面外ワープ処理
		if (m_Grid.x == -1)
		{//X軸ワープ
			if (posThis.x <= 0.0f)
			{
				m_Grid.x = CMapSystem::GetInstance()->GetMove()->GetMaxGrid().x;
				posThis = CMapSystem::GetInstance()->GetGritPos(m_Grid);
			}
			else
			{
				m_Grid.x = CMapSystem::GetInstance()->GetMove()->GetMinGrid().x;
				posThis = CMapSystem::GetInstance()->GetGritPos(m_Grid);
			}
		}
		else if(m_Grid.z == -1)
		{//Z軸ワープ
			if (posThis.z > 0.0f)
			{
				m_Grid.z = CMapSystem::GetInstance()->GetMove()->GetMaxGrid().z;
				posThis = CMapSystem::GetInstance()->GetGritPos(m_Grid);
			}
			else
			{
				m_Grid.z = CMapSystem::GetInstance()->GetMove()->GetMinGrid().z;
				posThis = CMapSystem::GetInstance()->GetGritPos(m_Grid);
			}
		}

		// モデル数の取得
		int nNumModel = GetNumModel();

		for (int nCnt = 0; nCnt < nNumModel; nCnt++)
		{
			// モデルの取得
			CModel* pModel = GetModel(nCnt);

			if (pModel != nullptr)
			{
				pModel->SetDisp(false);
			}
		}

		if (m_pUpEgg == nullptr)
		{
			// タマゴ読み込み処理
			switch (m_nPlayNumber)
			{
			case 0:
				m_pUpEgg = CObjectX::Create("data\\MODEL\\00_Player\\1P\\upper_egg.x");
				m_pUpEgg->SetMatColor(D3DXCOLOR(0.263529f, 0.570980f, 0.238431f, 1.0f));

				m_pUpEgg->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
				break;

			case 1:
				m_pUpEgg = CObjectX::Create("data\\MODEL\\00_Player\\2P\\upperegg.x");
				m_pUpEgg->SetMatColor(D3DXCOLOR(0.235294f, 0.715294f, 0.800000f, 1.0f));

				m_pUpEgg->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
				break;
			}
		}

		if (m_pDownEgg == nullptr)
		{
			// タマゴ読み込み処理
			switch (m_nPlayNumber)
			{
			case 0:
				m_pDownEgg = CObjectX::Create("data\\MODEL\\00_Player\\1P\\downer_egg.x");
				m_pDownEgg->SetMatColor(D3DXCOLOR(0.263529f, 0.570980f, 0.238431f, 1.0f));

				m_pDownEgg->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
				break;

			case 1:
				m_pDownEgg = CObjectX::Create("data\\MODEL\\00_Player\\2P\\downeregg.x");
				m_pDownEgg->SetMatColor(D3DXCOLOR(0.235294f, 0.715294f, 0.800000f, 1.0f));

				m_pDownEgg->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
				break;
			}
		}
		if (m_pP_NumUI != nullptr)
		{
			m_pP_NumUI->SetAppear(false);
		}
		break;
	}

	if (state != STATE_WALK)
	{
		//加減速状態の設定
		SetSpeedState(CMapMove::SPEED_NONE);
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// 壁との当たり判定
//====================================================================
void CPlayer::CollisionWall(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis, useful::COLLISION XYZ)
{
	// 壁のリスト構造が無ければ抜ける
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // リストを取得

	// 壁リストの中身を確認する
	for (CWall* pWall : list)
	{
		D3DXVECTOR3 pos = pWall->GetPos();
		D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f);
		D3DXVECTOR3 Size = pWall->GetSize();

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, pos, Move, Size, &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, &m_bJump, XYZ) == true)
		{
			if (!m_bPressObj)
			{ // 何かに押されてる

				// 待機状態
				SetState(STATE_WAIT);

				// 向き状態の設定
				m_pMoveState->SetRotState(CMoveState::ROTSTATE_WAIT);
			}
			else if (m_bPressObj)
			{ // 何にも押されてない
				Death();
			}

			posThis = m_Grid.ToWorld();	
		}
	}
}

//====================================================================
// 止まっているレールブロックとの当たり判定
//====================================================================
void CPlayer::CollisionWaitRailBlock(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis, useful::COLLISION XYZ)
{
	// レールブロックのリスト構造が無ければ抜ける
	if (CRailBlock::GetList() == nullptr) { return; }
	std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // リストを取得

	// レールブロックリストの中身を確認する
	for (CRailBlock* pRailBlock : list)
	{
		D3DXVECTOR3 pos = pRailBlock->GetPos();
		D3DXVECTOR3 posOld = pRailBlock->GetPosOld();
		D3DXVECTOR3 Move = pos - posOld;
		D3DXVECTOR3 Size = pRailBlock->GetSize();

		// 移動量が過剰な場合に当たり判定を無効化する
		if (Move.x * Move.x + Move.z * Move.z >= CMapSystem::GetInstance()->GetGritSize() * CMapSystem::GetInstance()->GetGritSize())
		{
			continue;
		}

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, posOld, Move, Size, &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, &m_bJump, XYZ) == true)
		{
			// 押されてる状態にする
			m_bPressObj = true;

			//待機状態にする
			SetState(STATE_WAIT);

			// 向き状態の設定
			m_pMoveState->SetRotState(CMoveState::ROTSTATE_WAIT);
		}
	}
}

//====================================================================
// 動いているレールブロックとの当たり判定
//====================================================================
void CPlayer::CollisionMoveRailBlock(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis, useful::COLLISION XYZ)
{
	// レールブロックのリスト構造が無ければ抜ける
	if (CRailBlock::GetList() == nullptr) { return; }
	std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // リストを取得

	// レールブロックリストの中身を確認する
	for (CRailBlock* pRailBlock : list)
	{
		D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float G_Size = CMapSystem::GetInstance()->GetGritSize();

		D3DXVECTOR3 Mypos = pRailBlock->GetPos();

		if (Mypos.x < D_pos.x + MapSize.x - G_Size &&
			Mypos.x > D_pos.x - MapSize.x + G_Size &&
			Mypos.z < D_pos.z + MapSize.z - G_Size &&
			Mypos.z > D_pos.z - MapSize.z + G_Size)
		{
			D3DXVECTOR3 MyposOld = pRailBlock->GetPosOld();
			D3DXVECTOR3 MyMove = (Mypos - MyposOld);

			// 移動量が過剰な場合に当たり判定を無効化する
			if (MyMove.x * MyMove.x + MyMove.z * MyMove.z >= CMapSystem::GetInstance()->GetGritSize() * CMapSystem::GetInstance()->GetGritSize())
			{
				continue;
			}

			float MySize = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

			D3DXVECTOR3 Move = m_move;
			bool a = false;
			
			// 矩形の当たり判定
			if (useful::CollisionBlock(Mypos, MyposOld, MyMove, D3DXVECTOR3(MySize, MySize, MySize), &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, &a, XYZ) == true)
			{
				m_bPressObj = true;
				return;
			}
		}
	}
}

//====================================================================
// 止まっている岩との当たり判定
//====================================================================
void CPlayer::CollisionWaitRock(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis, useful::COLLISION XYZ)
{
	// 岩のリスト構造が無ければ抜ける
	if (CRollRock::GetList() == nullptr) { return; }
	std::list<CRollRock*> list = CRollRock::GetList()->GetList();    // リストを取得

	// 岩リストの中身を確認する
	for (CRollRock* pRollRock : list)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(pRollRock->GetPos().x, 0.0f, pRollRock->GetPos().z);
		D3DXVECTOR3 posOld = pRollRock->GetPosOld();
		D3DXVECTOR3 Move = pos - posOld;
		D3DXVECTOR3 Size = pRollRock->GetSize();

		// 移動量が過剰な場合に当たり判定を無効化する
		if (Move.x * Move.x + Move.z * Move.z >= CMapSystem::GetInstance()->GetGritSize() * CMapSystem::GetInstance()->GetGritSize())
		{
			continue;
		}

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, posOld, Move, Size, &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, &m_bJump, XYZ) == true)
		{
			//待機状態にする
			SetState(STATE_WAIT);
			// 向き状態の設定
			m_pMoveState->SetRotState(CMoveState::ROTSTATE_WAIT);
		}
	}
}

//====================================================================
// 動いている岩との当たり判定
//====================================================================
void CPlayer::CollisionMoveRock(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis, useful::COLLISION XYZ)
{
	// 岩のリスト構造が無ければ抜ける
	if (CRollRock::GetList() == nullptr) { return; }
	std::list<CRollRock*> list = CRollRock::GetList()->GetList();    // リストを取得

	// 岩リストの中身を確認する
	for (CRollRock* pRock : list)
	{
		D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float G_Size = CMapSystem::GetInstance()->GetGritSize();

		D3DXVECTOR3 Mypos = pRock->GetPos();

		if (Mypos.x < D_pos.x + MapSize.x - G_Size &&
			Mypos.x > D_pos.x - MapSize.x + G_Size &&
			Mypos.z < D_pos.z + MapSize.z - G_Size &&
			Mypos.z > D_pos.z - MapSize.z + G_Size)
		{
			D3DXVECTOR3 MyposOld = pRock->GetPosOld();
			D3DXVECTOR3 MyMove = pRock->GetMove();

			// 移動量が過剰な場合に当たり判定を無効化する
			if (MyMove.x * MyMove.x + MyMove.z * MyMove.z >= CMapSystem::GetInstance()->GetGritSize() * CMapSystem::GetInstance()->GetGritSize())
			{
				continue;
			}

			float MySize = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

			D3DXVECTOR3 Move = m_move;
			bool a = false;

			// 矩形の当たり判定
			if (useful::CollisionBlock(Mypos, MyposOld, MyMove, D3DXVECTOR3(MySize, MySize, MySize), &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, &a, XYZ) == true)
			{
				//m_Objmove.x = MyMove.x;
				//m_move.x = 0.0f;
				//m_bPressObj = true;
				return;
			}
		}
	}
}

//====================================================================
// 上下左右に壁が存在するかの判定
//====================================================================
void CPlayer::SearchWall(D3DXVECTOR3& posThis)
{
	STATE state = GetState();

	bool OKR = true;	//右
	bool OKL = true;	//左
	bool OKU = true;	//上
	bool OKD = true;	//下

	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	int nMapWightMax = pMapSystem->GetWightMax();
	int nMapHeightMax = pMapSystem->GetHeightMax();
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

	int nRNumber = m_Grid.x + 1;
	int nLNumber = m_Grid.x - 1;
	int nUNumber = m_Grid.z - 1;
	int nDNumber = m_Grid.z + 1;

	nRNumber = useful::RangeNumber(nMapWightMax, 0, nRNumber);
	nLNumber = useful::RangeNumber(nMapWightMax, 0, nLNumber);
	nUNumber = useful::RangeNumber(nMapHeightMax, 0, nUNumber);
	nDNumber = useful::RangeNumber(nMapHeightMax, 0, nDNumber);

	OKR = !pMapSystem->GetGritBool(nRNumber, m_Grid.z);
	OKL = !pMapSystem->GetGritBool(nLNumber, m_Grid.z);
	OKU = !pMapSystem->GetGritBool(m_Grid.x, nUNumber);
	OKD = !pMapSystem->GetGritBool(m_Grid.x, nDNumber);

	//自分の立っているグリットの中心位置を求める
	D3DXVECTOR3 MyGritPos = m_Grid.ToWorld();
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "自分がいるグリットの中心位置 %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if ((posThis.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) &&
		posThis.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&
		posThis.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		posThis.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK)) ||
		state == STATE_WAIT)
	{// グリットの中心位置に立っているなら操作を受け付ける
		m_Progress.bOKR = OKR;	//右
		m_Progress.bOKL = OKL;	//左
		m_Progress.bOKU = OKU;	//上
		m_Progress.bOKD = OKD;	//下

		if (m_bGritCenter == false)
		{
			float MyPosY = posThis.y;
			posThis = CMapSystem::GetInstance()->GetGritPos(m_Grid);
			posThis.y = MyPosY;

			m_bGritCenter = true;
		}
	}
	else
	{
		m_Progress.bOKR = false;	//右
		m_Progress.bOKL = false;	//左
		m_Progress.bOKU = false;	//上
		m_Progress.bOKD = false;	//下

		m_bGritCenter = false;
	}
}

//====================================================================
// デビルホールとの当たり判定
//====================================================================
void CPlayer::CollisionDevilHole(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis, useful::COLLISION XYZ)
{
	// デビルホールのリスト構造が無ければ抜ける
	if (CDevilHole::GetList() == nullptr) { return; }
	std::list<CDevilHole*> list = CDevilHole::GetList()->GetList();    // リストを取得

	// デビルホールリストの中身を確認する
	for (CDevilHole* pDevilHole : list)
	{
		D3DXVECTOR3 pos = pDevilHole->GetPos();
		D3DXVECTOR3 posOld = pDevilHole->GetPosOld();
		D3DXVECTOR3 Size = pDevilHole->GetSize();

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, pos, INITVECTOR3, Size, &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, &m_bJump, XYZ) == true)
		{
			//待機状態にする
			SetState(STATE_WAIT);
			// 向き状態の設定
			m_pMoveState->SetRotState(CMoveState::ROTSTATE_WAIT);
			posThis = m_Grid.ToWorld();
		}
	}
}

//====================================================================
// 敵の当たり判定
//====================================================================
void CPlayer::CollisionEnemy(D3DXVECTOR3& posThis)
{
	// 敵のリスト構造が無ければ抜ける
	if (CEnemy::GetList() == nullptr) { return; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // リストを取得

	// 敵のリストの中身を確認する
	for (CEnemy* pEnemy : list)
	{
		if (pEnemy->GetHitState() != CEnemy::HIT_STATE_NORMAL)
		{
			continue;
		}

		D3DXVECTOR3 pos = pEnemy->GetPos();
		D3DXVECTOR3 posOld = pEnemy->GetPosOld();
		D3DXVECTOR3 Size = pEnemy->GetSize();

		// 円の当たり判定
		if (useful::CollisionCircle(posThis, pos, 30.0f) == true)
		{
			if (!GetInvincible())
			{
				Death();
			}
		}
	}
}

//====================================================================
// ステージ外の当たり判定
//====================================================================
void CPlayer::CollisionStageOut(D3DXVECTOR3& posThis)
{
	D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	float G_Size = CMapSystem::GetInstance()->GetGritSize();
	D3DXVECTOR3 PressPos = posThis;

	if (posThis.x + G_Size > D_pos.x + MapSize.x)	// 右
	{
		SetState(STATE_WAIT);

		//壁の圧死判定
		CollisionPressWall(posThis, 2);

		posThis.x = D_pos.x + MapSize.x - G_Size;
		m_move.x = 0.0f;
	}
	if (posThis.x - G_Size < D_pos.x - MapSize.x)	// 左
	{
		SetState(STATE_WAIT);

		//壁の圧死判定
		CollisionPressWall(posThis, 3);

		posThis.x = D_pos.x - MapSize.x + G_Size;
		m_move.x = 0.0f;
	}
	if (posThis.z + G_Size > D_pos.z + MapSize.z)	// 上
	{
		SetState(STATE_WAIT);

		//壁の圧死判定
		CollisionPressWall(posThis, 0);

		posThis.z = D_pos.z + MapSize.z - G_Size;
		m_move.z = 0.0f;
	}
	if (posThis.z - G_Size < D_pos.z - MapSize.z)	// 下
	{
		SetState(STATE_WAIT);

		//壁の圧死判定
		CollisionPressWall(posThis, 1);

		posThis.z = D_pos.z - MapSize.z + G_Size;
		m_move.z = 0.0f;
	}
}

//====================================================================
// 壁との圧死判定
//====================================================================
void CPlayer::CollisionPressWall(D3DXVECTOR3& posThis, int Rot)
{//Rot [0 : 上][1 : 下][2 : 右][3 : 左]

	D3DXVECTOR3 ScorllPos = posThis;
	CMapSystem::GetInstance()->GetMove()->FollowScroll(ScorllPos);

	//スクロールしていないときはこの関数は作用しない
	if (posThis == ScorllPos)
	{
		return;
	}

	CMapSystem::GRID MyGrid = GetGrid();

	switch (Rot)
	{
	case 0:
		MyGrid.z += 1;

		if (CMapSystem::GetInstance()->GetGritBool(MyGrid) == true &&
			CMapSystem::GetInstance()->GetMove()->GetMove().z > 0.0f)
		{
			Death();
		}
		break;

	case 1:
		MyGrid.z -= 1;

		if (CMapSystem::GetInstance()->GetGritBool(MyGrid) == true &&
			CMapSystem::GetInstance()->GetMove()->GetMove().z < 0.0f)
		{
			Death();
		}
		break;

	case 2:
		MyGrid.x -= 1;

		if (CMapSystem::GetInstance()->GetGritBool(MyGrid) == true &&
			CMapSystem::GetInstance()->GetMove()->GetMove().x > 0.0f)
		{
			Death();
		}
		break;

	case 3:
		MyGrid.x += 1;

		if (CMapSystem::GetInstance()->GetGritBool(MyGrid) == true &&
			CMapSystem::GetInstance()->GetMove()->GetMove().x < 0.0f)
		{
			Death();
		}
		break;
	}
}

//====================================================================
// ステージ内にいるかどうか
//====================================================================
bool CPlayer::CollisionStageIn(D3DXVECTOR3& posThis)
{
	D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	float G_Size = CMapSystem::GetInstance()->GetGritSize();

	if (posThis.x + G_Size <= D_pos.x + MapSize.x &&
		posThis.x - G_Size >= D_pos.x - MapSize.x &&
		posThis.z + G_Size <= D_pos.z + MapSize.z &&
		posThis.z - G_Size >= D_pos.z - MapSize.z)
	{
		return true;
	}

	return false;
}

//====================================================================
// 画面外との圧死判定
//====================================================================
void CPlayer::CollisionPressStageOut(D3DXVECTOR3& posThis)
{
	//if (m_bPressObj == true)
	//{
	//	D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
	//	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	//	float G_Size = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

	//	if (posThis.x + G_Size > D_pos.x + MapSize.x)
	//	{
	//		Death();
	//	}
	//	if (posThis.x - G_Size < D_pos.x - MapSize.x)
	//	{
	//		Death();
	//	}
	//	if (posThis.z + G_Size > D_pos.z + MapSize.z)
	//	{
	//		Death();
	//	}
	//	if (posThis.z - G_Size < D_pos.z - MapSize.z)
	//	{
	//		Death();
	//	}
	//}
}

//====================================================================
//カメラ位置更新処理
//====================================================================
void CPlayer::CameraPosUpdate(D3DXVECTOR3& posThis)
{
	//カメラ位置の更新
	m_CameraPos.x = posThis.x;
	m_CameraPos.z = posThis.z;

	if (m_bJump == false)
	{
		m_CameraPos.y = posThis.y;
	}
}

//====================================================================
//位置更新処理
//====================================================================
void CPlayer::PosUpdate(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis)
{
	//減衰係数
	//m_move.x = m_move.x * 0.5f;
	//m_move.z = m_move.z * 0.5f;

	if (m_move.x <= 0.0001f && m_move.x >= -0.0001f)
	{
		m_move.x = 0.0f;
	}

	if (m_move.z <= 0.0001f && m_move.z >= -0.0001f)
	{
		m_move.z = 0.0f;
	}

	if (m_bJump == true)
	{
		m_Objmove.x = m_Objmove.x * 0.25f;
		if (m_Objmove.x <= 0.0001f && m_Objmove.x >= -0.0001f)
		{
			m_Objmove.x = 0.0f;
		}

		m_Objmove.z = m_Objmove.z * 0.25f;
		if (m_Objmove.z <= 0.0001f && m_Objmove.z >= -0.0001f)
		{
			m_Objmove.z = 0.0f;
		}
	}

	// 変数宣言
	float fSpeed = 1.0f;	// スロー用 default1.0fで初期化
	if (m_pSlow)
	{
		fSpeed = m_pSlow->GetValue();
	}

	CMapMove* pMapMove = CMapSystem::GetInstance()->GetMove();

	//Y軸の位置更新
	posThis.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	CollisionWall(posThis,posOldThis,sizeThis,useful::COLLISION_Y);
	CollisionDevilHole(posThis, posOldThis, sizeThis, useful::COLLISION_Y);

	// 加減速状態の取得
	CMapMove::SPEED Speed = GetSpeedState();

	//X軸の位置更新
	if (m_move.x > 0.0f || m_move.x < 0.0f)
	{
		posThis.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeX(m_move.x, Speed);
	}

	// 何も押されてない状態にする
	m_bPressObj = false;

	// 壁との当たり判定
	CollisionDevilHole(posThis, posOldThis, sizeThis, useful::COLLISION_X);
	CollisionWaitRailBlock(posThis, posOldThis, sizeThis, useful::COLLISION_X);
	CollisionWaitRock(posThis, posOldThis, sizeThis, useful::COLLISION_X);
	CollisionWall(posThis, posOldThis, sizeThis, useful::COLLISION_X);

	//Z軸の位置更新
	if (m_move.z > 0.0f || m_move.z < 0.0f)
	{
		posThis.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeZ(m_move.z, Speed);
	}

	//加減速状態の設定
	SetSpeedState(Speed);

	// 壁との当たり判定
	CollisionDevilHole(posThis, posOldThis, sizeThis, useful::COLLISION_Z);
	CollisionWaitRailBlock(posThis, posOldThis, sizeThis, useful::COLLISION_Z);
	CollisionWaitRock(posThis, posOldThis, sizeThis, useful::COLLISION_Z);
	CollisionWall(posThis, posOldThis, sizeThis, useful::COLLISION_Z);
}

//====================================================================
//オブジェクトによる位置更新処理
//====================================================================
void CPlayer::ObjPosUpdate(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis)
{
	if (m_bJump == true)
	{
		m_Objmove.x = m_Objmove.x * 0.25f;
		if (m_Objmove.x <= 0.0001f && m_Objmove.x >= -0.0001f)
		{
			m_Objmove.x = 0.0f;
		}

		m_Objmove.z = m_Objmove.z * 0.25f;
		if (m_Objmove.z <= 0.0001f && m_Objmove.z >= -0.0001f)
		{
			m_Objmove.z = 0.0f;
		}
	}

	// 変数宣言
	float fSpeed = 1.0f;	// スロー用 default1.0fで初期化
	if (m_pSlow)
	{
		fSpeed = m_pSlow->GetValue();
	}

	//Y軸の位置更新
	posThis.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	//X軸の位置更新
	posThis.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 当たり判定
	//CollisionMoveRailBlock(posThis, posOldThis, sizeThis, useful::COLLISION_X);		// レールブロック
	//CollisionMoveRock(posThis, posOldThis, sizeThis, useful::COLLISION_X);			// 岩

	//Z軸の位置更新
	posThis.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	//CollisionMoveRailBlock(posThis, posOldThis, sizeThis, useful::COLLISION_Z);		// レールブロック
	//CollisionMoveRock(posThis, posOldThis, sizeThis, useful::COLLISION_Z);			// 岩
}

//====================================================================
//向き更新処理
//====================================================================
void CPlayer::RotUpdate(D3DXVECTOR3& rotThis)
{
	// 角度の差分を算出
	D3DXVECTOR3 rotDiff = m_rotDest - rotThis;

	// 正規化
	useful::NormalizeAngle(&rotDiff);

	// 向きの更新処理
	rotThis += (rotDiff * 0.5);
}

//====================================================================
//卵の動き
//====================================================================
void CPlayer::EggMove(D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis)
{
	STATE state = GetState();

	if (state == STATE_EGG)
	{
		if (posThis.y > 0.0f)
		{
			//重力
			m_move.y -= RESPAWN_GRAVITY;

			// エフェクトの生成
			if (m_pEffectEgg == nullptr)
			{
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
				m_pEffectEgg = MyEffekseer::EffectCreate((CMyEffekseer::TYPE)((int)CMyEffekseer::TYPE_BORN_PLAYER_00 + m_nPlayNumber), true, ef, rotThis);
			}

			// エフェクトの生成
			if (m_pEffectGuide == nullptr && m_pShadow != nullptr)
			{
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(m_pShadow->GetPos(), rotThis, mat);
				m_pEffectGuide = MyEffekseer::EffectCreate((CMyEffekseer::TYPE)((int)CMyEffekseer::TYPE_BORNGUID1 + m_nPlayNumber), true, ef, rotThis);
			}
		}

		//Y軸の位置更新
		posThis.y += m_move.y * CManager::GetInstance()->GetGameSpeed();

		//卵の落下判定
		if (posThis.y < 0.0f)
		{
			//落下時に移動量と位置を０にする
			posThis.y = 0.0f;
			m_move.y = 0.0f;

			// エフェクトの生成
			D3DXMATRIX mat = *GetUseMultiMatrix();
			D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_FALLSMOKE, false, ef, rotThis);

			// エフェクトの削除
			if (m_pEffectEgg != nullptr)
			{
				m_pEffectEgg->SetDeath();
				m_pEffectEgg = nullptr;
			}

			// エフェクトの削除
			if (m_pEffectGuide != nullptr)
			{
				m_pEffectGuide->SetDeath();
				m_pEffectGuide = nullptr;
			}
		}

		if (m_pUpEgg != nullptr)
		{
			m_pUpEgg->SetPos(D3DXVECTOR3(posThis.x, posThis.y + 65.0f, posThis.z));
		}
		if (m_pDownEgg != nullptr)
		{
			m_pDownEgg->SetPos(D3DXVECTOR3(posThis.x, posThis.y + 65.0f, posThis.z));
		}
		SetEggMove(INITVECTOR3);
	}
	else
	{
		if (m_pUpEgg != nullptr)
		{
			D3DXVECTOR3 pos = m_pUpEgg->GetPos();
			D3DXVECTOR3 rot = m_pUpEgg->GetRot();
			float ColorA = m_pUpEgg->GetMatColor().a;

			ColorA -= EGG_COLOR_DEL_A;

			D3DXVECTOR3 EggMove = GetEggMove();

			EggMove.y -= EGG_GRAVITY;

			pos += EggMove;

			rot.z -= EggMove.x * EGG_ROT;
			rot.x += EggMove.z * EGG_ROT;

			EggMove.x = EggMove.x * EGG_MOVE_DEL;
			EggMove.z = EggMove.z * EGG_MOVE_DEL;

			SetEggMove(EggMove);

			if (pos.y < CObjmeshField::GetListTop()->GetPos().y + 30.0f)
			{
				pos.y = CObjmeshField::GetListTop()->GetPos().y + 30.0f;
			}
			else
			{
				m_pUpEgg->SetRot(rot);
			}

			m_pUpEgg->SetPos(pos);
			m_pUpEgg->SetMatColorA(ColorA);

			if (ColorA <= 0.0f)
			{
				m_pUpEgg->Uninit();
				m_pUpEgg = nullptr;
			}
		}

		if (m_pDownEgg != nullptr)
		{
			float ColorA = m_pDownEgg->GetMatColor().a;

			ColorA -= EGG_COLOR_DEL_A;

			m_pDownEgg->SetMatColorA(ColorA);

			if (ColorA <= 0.0f)
			{
				m_pDownEgg->Uninit();
				m_pDownEgg = nullptr;
			}
		}
	}
}

//==========================================
//  足跡の設置
//==========================================
void CPlayer::PrintFoot(const D3DXVECTOR3& rotThis)
{
	// 前回グリッドと現在のグリッドが一致していた場合関数を抜ける
	if (m_Grid == m_OldGrid) { return; }

	// 足跡を設定
	D3DXVECTOR3 rot = rotThis;
	rot.y += D3DX_PI;
	CFootPrint::Create(m_OldGrid, rot);
}

//==========================================
//  エフェクトの移動
//==========================================
void CPlayer::ControlEffect(CEffekseer* pTarget, const D3DXVECTOR3* pPos)
{
	// 向きの変更
	RotationEffect(pTarget);

	// 移動
	MoveEffect(pTarget, pPos);
}

//==========================================
//  エフェクトの回転
//==========================================
void CPlayer::RotationEffect(CEffekseer* pTarget)
{
	// 対象のエフェクトがnullの場合関数を抜ける
	if (pTarget == nullptr) { return; }

	// 計算に使用する値の取得
	D3DXMATRIX mat = *GetUseMultiMatrix();

	// 向きにマトリックスを反映
	D3DXVECTOR3 rot = useful::CalcMatrixToRot(mat);
	rot += GetRot();

	// エフェクトに情報を適用
	pTarget->SetRotation(rot);
}

//==========================================
//  エフェクトの移動
//==========================================
void CPlayer::MoveEffect(CEffekseer* pTarget, const D3DXVECTOR3* pPos)
{
	// 対象のエフェクトがnullの場合関数を抜ける
	if (pTarget == nullptr) { return; }

	// 計算に使用する値の取得
	D3DXMATRIX mat = *GetUseMultiMatrix();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 pos = (pPos != nullptr) ? *pPos : GetPos();

	// 座標にマトリックスを反映
	pos = useful::CalcMatrix(pos, rot, mat);
	rot = useful::CalcMatrixToRot(mat);

	// エフェクトに情報を適用
	pTarget->SetPosition(pos);
}

//==========================================
//  加減速のエフェクトを生成
//==========================================
void CPlayer::SpeedEffect(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	DebugProc::Print(DebugProc::POINT_CENTER, "プレイヤーの向き : %f\n", rot.y);

	// 前回の状態と一致していたら関数を抜ける
	CMapMove::SPEED speed = GetSpeedState();
	if (speed == GetOldSpeedState()) { return; }

	// エフェクトを削除
	if (m_pEffectSpeed != nullptr)
	{
		m_pEffectSpeed->SetDeath();
		m_pEffectSpeed = nullptr;
	}

	// 通常状態の場合関数を抜ける
	if (speed == CMapMove::SPEED_NONE) { return; }

	// 生成位置を算出
	D3DXMATRIX mtx = *GetUseMultiMatrix();
	D3DXVECTOR3 temp = pos;
	temp.y += EFFECT_HEIGHT;
	D3DXVECTOR3 ef = useful::CalcMatrix(temp, rot, mtx);
	D3DXVECTOR3 efRot = useful::CalcMatrixToRot(mtx);
	efRot.y = rot.y;

	// 新しいエフェクトを設定する
	switch (speed)
	{
	case CMapMove::SPEED_DOWN:
		m_pEffectSpeed = MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DROP, true, ef, efRot);
		break;
	case CMapMove::SPEED_UP:
		m_pEffectSpeed = MyEffekseer::EffectCreate(CMyEffekseer::TYPE_ACCELE, true, ef, efRot);
		break;
	default:
		break;
	}
}

//====================================================================
//ダメージ処理
//====================================================================
void CPlayer::Death(void)
{
	if (m_pP_NumUI != nullptr)
	{
		m_pP_NumUI->SetAppear(false);
	}
}

//====================================================================
//復活処理
//====================================================================
void CPlayer::Reivel(D3DXVECTOR3& posThis)
{
	//指定位置からブロックが存在しないグリッドを検索してその場所に復活する処理
	int WMax = CMapSystem::GetInstance()->GetWightMax();
	int HMax = CMapSystem::GetInstance()->GetHeightMax();
	CMapSystem::GRID ReivelPos = CMapSystem::GRID(0, 0);

	if (m_nPlayNumber == 0)
	{
		ReivelPos.x = CMapSystem::GetInstance()->CalcGridX(RESPAWN_POS01.x);
		ReivelPos.z = CMapSystem::GetInstance()->CalcGridZ(RESPAWN_POS01.z);
	}
	else
	{
		ReivelPos.x = CMapSystem::GetInstance()->CalcGridX(RESPAWN_POS02.x);
		ReivelPos.z = CMapSystem::GetInstance()->CalcGridZ(RESPAWN_POS02.z);
	}

	for (int nSetW = ReivelPos.x, nCntW = 0; nCntW < WMax; nSetW++, nCntW++)
	{
		if (nSetW >= WMax)
		{
			nSetW = nSetW - WMax;
		}

		for (int nSetH = ReivelPos.z, nCntH = 0; nCntH < HMax; nCntH++, nCntH++)
		{
			if (nSetH >= HMax)
			{
				nSetH = nSetH - HMax;
			}

			if (CMapSystem::GetInstance()->GetGritBool(nSetW, nSetH) == false)
			{
				SetGrid(CMapSystem::GRID(nSetW, nSetH));
				posThis = CMapSystem::GRID(nSetW, nSetH).ToWorld();
				posThis.y = RESPAWN_POS01.y;
				SetState(STATE_EGG);
				return;
			}

		}
	}
}

//====================================================================
//プレイヤー番号の表示状態
//====================================================================
void CPlayer::PlayerNumberDisp(bool Set)
{
	if (m_pP_NumUI != nullptr)
	{
		m_pP_NumUI->SetAppear(Set);
	}
}

//====================================================================
// モデルのモーション設定
//====================================================================
void CPlayer::SetAction(ACTION_TYPE Action, float BlendTime)
{
	// モーションの取得
	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	if (m_Action != Action)
	{
		m_Action = Action;
		pMotion->Set(Action, BlendTime);
	}
}

//====================================================================
// モデル表示の設定
//====================================================================
void CPlayer::SetModelDisp(bool Sst)
{
	// モデル数の取得
	int nNumModel = GetNumModel();

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// モデルの取得
		CModel* pModel = GetModel(nCnt);

		if (pModel != nullptr)
		{
			pModel->SetDisp(Sst);
		}
	}
}

//==========================================
//  アイテムの設定
//==========================================
void CPlayer::SetItemType(ITEM_TYPE eType)
{
	// アイテムのタイプを設定
	m_eItemType = eType;

	// エフェクトの生成
	if (eType != TYPE_NONE)
	{
		if (m_pEffectItem == nullptr)
		{
			D3DXMATRIX mat = *GetUseMultiMatrix();
			D3DXVECTOR3 ef = useful::CalcMatrix(GetPos(), GetRot(), mat);
			m_pEffectItem = MyEffekseer::EffectCreate(CMyEffekseer::TYPE_GOT_THEITEM, true, ef, GetRot());
		}
	}

	// 所持しているアイテムによってモデルの表示を切り替える
	switch (eType)
	{
	case TYPE_CROSS:
		SetPartsDisp(10, false);	// 聖書のモデル非表示

		// 所持時間タイマーをリセット
		m_fCrossTimer = 0.0f;

		break;

	case TYPE_BIBLE:
		SetPartsDisp(10, true);		// 聖書のモデル表示

		break;

	default:
		SetPartsDisp(9, false);		// 十字架のモデル非表示
		SetPartsDisp(10, false);	// 聖書のモデル非表示

		// エフェクトの削除
		if (m_pEffectItem != nullptr)
		{
			m_pEffectItem->SetDeath();
			m_pEffectItem = nullptr;
		}

		break;
	}
}

//==========================================
// 移動状態変更処理
//==========================================
void CPlayer::ChangeMoveState(CMoveState* pMoveState)
{
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	m_pMoveState = pMoveState;
	m_pMoveState->Init();
}

//====================================================================
// プレイヤーの指定モデル消去
//====================================================================
void CPlayer::SetPartsDisp(int nParts, bool Set)
{
	// 特定番号のモデル取得
	CModel* pModel = GetModel(nParts);

	if (pModel != nullptr)
	{
		pModel->SetDisp(Set);
	}
}

//==========================================
//  リストの取得
//==========================================
CListManager<CPlayer>* CPlayer::GetList(void)
{
	return m_pList;
}

//====================================================================
// 十字架保持時間の取得
//====================================================================
float CPlayer::GetCrossStateParcent(void)
{
	float StateParcent = 1.0f - m_fCrossTimer / CROSS_TIME;
	return StateParcent;
}
