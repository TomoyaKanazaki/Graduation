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
#include "CubeBlock.h"
#include "slowManager.h"
#include "Number.h"
#include "MapModel.h"
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

#include "MyEffekseer.h"

//===========================================
// 定数定義
//===========================================
namespace
{
	const int LIFE_MAX = 2;	//初期ライフ数
	const int FIRE_STOPTIME = 30;	//攻撃時の移動停止時間
	const D3DXVECTOR3 RESPAWN_POS = D3DXVECTOR3(-100.0f, 2000.0f, 100.0f); // 復活位置
	const float RESPAWN_GRAVITY = 0.3f;			//卵の重力
	const int INVINCIBLE_TIME = 120;			//無敵時間

	const float GRIT_OK = 45.0f;			//移動可能なグリットの範囲内
	const float PLAYER_SPEED = 5.0f;		//プレイヤーの移動速度
	const float OBJDISTANCE = 10000.0f;		// オブジェクトの距離

	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(35.0f, 40.0f, 35.0f);		//横の当たり判定

	const D3DXVECTOR3 LIFE_POS00 = D3DXVECTOR3(50.0f, 650.0f, 0.0f);
	const D3DXVECTOR3 LIFE_POS01 = D3DXVECTOR3(900.0f, 650.0f, 0.0f);

	const float CROSS_TIME = 10.0f; // 十字架を所持していられる時間

	const float EGG_GRAVITY = 0.98f;	 //移動量の減衰速度
	const D3DXVECTOR3 EGG_MOVE = D3DXVECTOR3(10.0f, 10.0f, 10.0f);	 //移動量の減衰速度
	const float EGG_ROT = D3DX_PI * 0.006f;		//回転速度
	const float EGG_MOVE_DEL = 0.9f;			//移動量の減衰速度
	const float EGG_COLOR_DEL_A = 0.01f;		//不透明度の減衰速度

	const float SHADOW_SIZE = 50.0f;			// 丸影の大きさ
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CPlayer>* CPlayer::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CPlayer::CPlayer(int nPriority) : CObjectCharacter(nPriority),
m_move(INITVECTOR3),
m_Objmove(INITVECTOR3),
m_AutoMoveRot(INITVECTOR3),
m_bJump(false),
m_nActionCount(0),
m_Action(ACTION_NONE),
m_AtkAction(ACTION_NONE),
m_State(STATE_EGG),
m_nStateCount(0),
m_AtkPos(INITVECTOR3),
m_CollisionRot(0.0f),
m_OKL(true),
m_OKR(true),
m_OKU(true),
m_OKD(true),
m_bInput(false),
m_pLifeUi(nullptr),
m_nLife(0),
m_eItemType(TYPE_NONE),
m_MoveState(MOVE_STATE_NONE),
m_Grid(0, 0),
m_bGritCenter(true),
m_bPressObj(false),
m_fCrossTimer(0.0f),
m_pUpEgg(nullptr),
m_pDownEgg(nullptr),
m_EggMove(INITVECTOR3),
m_bInvincible(true),
m_nInvincibleCount(0),
m_pShadow(nullptr),
m_pScore(nullptr),
m_nTime(0)
{

}

//====================================================================
//デストラクタ
//====================================================================
CPlayer::~CPlayer()
{

}

//====================================================================
//生成処理
//====================================================================
CPlayer* CPlayer::Create(int PlayNumber)
{
	CPlayer* pPlayer = new CPlayer();

	// メモリの確保に失敗した場合nullを返す
	if (pPlayer == nullptr) { assert(false); return nullptr; }

	// 初期化処理に失敗した場合nullを返す
	if (FAILED(pPlayer->Init(PlayNumber)))
	{
		assert(false);
		delete pPlayer;
		return nullptr;
	}

	return pPlayer;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CPlayer::Init(int PlayNumber)
{
	// 値を取得
	D3DXVECTOR3 posMy = GetPos();			// 位置
	D3DXVECTOR3 posOldMy = GetPosOld();		// 前回の位置
	D3DXVECTOR3 rotMy = GetRot();			// 向き
	D3DXVECTOR3 sizeMy = GetSize();			// 大きさ

	CMapSystem* pMapSystem = CMapSystem::GetInstance();		// マップシステムの情報

	m_nPlayNumber = PlayNumber;

	// プレイヤーの位置取得
	posMy = pMapSystem->GetPlayerPos(PlayNumber);

	// サイズの設定
	sizeMy = COLLISION_SIZE;

	// 向きの設定
	rotMy = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

	// アクションの設定
	m_Action = ACTION_EGG;
	m_AtkAction = ACTION_EGG;

	//種類設定
	SetType(CObject::TYPE_PLAYER3D);

	// 体力の設定
	m_nLife = LIFE_MAX;

	// 状態の設定
	m_MoveState = MOVE_STATE_WAIT;

	//マップとのマトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// キャラクターテキスト読み込み処理
	CObjectCharacter::Init("data\\TXT\\motion_tamagon1P.txt");

	// キャラクターのマトリックス設定
	CObjectCharacter::SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
	CObjectCharacter::SetUseStencil(true);
	CObjectCharacter::SetUseShadowMtx(true);

	if (m_pLifeUi == nullptr)
	{
		m_pLifeUi = CLifeUi::Create();
	}

	if (m_pScore == nullptr)
	{
		m_pScore = CScore::Create();
	}

	switch (m_nPlayNumber)
	{
	case 0:
		if (m_pLifeUi != nullptr)
		{
			// 数字の位置
			m_pLifeUi->GetNumber()->SetPos(D3DXVECTOR3(LIFE_POS00.x + 200.0f, LIFE_POS00.y, LIFE_POS00.z));

			// 体力
			m_pLifeUi->SetPos(LIFE_POS00);
			m_pLifeUi->GetNumber()->SetNumber(m_nLife);
		}
		
		if (m_pScore != nullptr)
		{
			m_pScore->SetPos(D3DXVECTOR3(50.0f, 40.0f, 0.0f));
		}

		break;

	case 1:

		if (m_pLifeUi != nullptr)
		{
			// 数字の位置
			m_pLifeUi->GetNumber()->SetPos(D3DXVECTOR3(LIFE_POS01.x + 200.0f, LIFE_POS01.y, LIFE_POS01.z));

			// 体力
			m_pLifeUi->SetPos(LIFE_POS01);
			m_pLifeUi->GetNumber()->SetNumber(m_nLife);
		}

		if (m_pScore != nullptr)
		{
			m_pScore->SetPos(D3DXVECTOR3(1050.0f, 40.0f, 0.0f));
		}

		break;
	}

	// アイテム状態を設定
	SetItemType(CPlayer::TYPE_NONE);

	// スローの生成
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_PLAYER, CSlowManager::TAG_PLAYER);

	// 値更新
	SetPos(posMy);			// 位置
	SetPosOld(posOldMy);	// 前回の位置
	SetRot(rotMy);			// 向き
	SetSize(sizeMy);		// 大きさ

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CPlayer>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CPlayer::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// キャラクタークラスの終了（継承）
	CObjectCharacter::Uninit();

	// スコアの削除
	if (m_pScore != nullptr)
	{
		m_pScore = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CPlayer::Update(void)
{
	// 値を取得
	D3DXVECTOR3 posMy = GetPos();			// 位置
	D3DXVECTOR3 posOldMy = GetPosOld();		// 前回の位置
	D3DXVECTOR3 rotMy = GetRot();			// 向き
	D3DXVECTOR3 sizeMy = GetSize();			// 大きさ

	// 過去の位置に代入
	posOldMy = posMy;

	if (m_State != STATE_DEATH)
	{
		//壁があるか判断
		SearchWall(posMy);

		if (
			(m_State != STATE_EGG && CollisionStageIn(posMy) == true &&
				CMapSystem::GetInstance()->GetGritBool(m_Grid.x, m_Grid.z) == false) ||
			(m_State == STATE_EGG && CollisionStageIn(posMy) == true &&
				CMapSystem::GetInstance()->GetGritBool(m_Grid.x, m_Grid.z) == false &&
				m_bGritCenter == true && posMy.y <= 0.0f)
			)
		{// ステージ内にいる かつ ブロックの無いグリッド上の時
			// 移動処理
			Move(posMy,rotMy);
		}

		// 向き移動処理
		Rot(rotMy);

		if (m_eItemType != TYPE_NONE)
		{
			Attack(posMy,rotMy);
		}

		// 十字架を持っている場合
		if (m_eItemType == TYPE_CROSS)
		{
			// タイマーを加算
			m_fCrossTimer += DeltaTime::Get();

			// 十字架の所持可能時間を超過した場合
			if (m_fCrossTimer >= CROSS_TIME)
			{
				// 所持時間タイマーをリセット
				m_fCrossTimer = 0.0f;

				// アイテムを所持していない状態にする
				SetItemType(TYPE_NONE);
			}
		}

		// カメラ更新処理
		CameraPosUpdate(posMy);

		if (m_State == STATE_WALK)
		{
			// 位置更新処理
			PosUpdate(posMy,posOldMy,sizeMy);
		}

		ObjPosUpdate(posMy,posOldMy,sizeMy);

		if (m_State != STATE_EGG && m_State != STATE_DEATH)
		{
			//画面外判定
			CollisionStageOut(posMy);
		}

		// 敵の判定
		CollisionEnemy(posMy);
	}

	// プレイヤーがマップのどのマスに存在しているか設定する
	m_Grid.x = CMapSystem::GetInstance()->CMapSystem::CalcGridX(posMy.x);
	m_Grid.z = CMapSystem::GetInstance()->CMapSystem::CalcGridZ(posMy.z);

	//状態の管理
	StateManager(posMy);

	if (m_nInvincibleCount > 0)
	{
		m_nInvincibleCount--;
	}
	else
	{
		m_bInvincible = false;
	}

	if (m_bInvincible)
	{
		SetModelColor(CModel::COLORTYPE_TRUE_ALL, D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
	}
	else
	{
		SetModelColor(CModel::COLORTYPE_FALSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//卵の動き
	EggMove(posMy,rotMy);

	// キャラクタークラスの更新（継承）
	CObjectCharacter::Update();

	//モーションの管理
	ActionState();

	//デバッグキーの処理と設定
	DebugKey();

	//デバッグ表示
	DebugProc::Print(DebugProc::POINT_LEFT, "[自分]位置 %f : %f : %f\n", posMy.x, posMy.y, posMy.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[自分]向き %f : %f : %f\n", rotMy.x, rotMy.y, rotMy.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[自分]横 %d : 縦 %d\n", m_Grid.x, m_Grid.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[自分]状態 : ");
	auto str = magic_enum::enum_name(m_State);
	DebugProc::Print(DebugProc::POINT_LEFT, str.data());
	DebugProc::Print(DebugProc::POINT_LEFT, "\n");

	// 値更新
	SetPos(posMy);			// 位置
	SetPosOld(posOldMy);	// 前回の位置
	SetRot(rotMy);			// 向き
	SetSize(sizeMy);		// 大きさ
}

//====================================================================
//描画処理
//====================================================================
void CPlayer::Draw(void)
{
	// キャラクタークラスの描画（継承）
	CObjectCharacter::Draw();
}

//====================================================================
//移動処理
//====================================================================
void CPlayer::Move(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy)
{
	m_bInput = false;

	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//入力処理
	NormarizeMove = MoveInputKey(posMy,rotMy,NormarizeMove);

	NormarizeMove = MoveInputPadStick(posMy, rotMy, NormarizeMove);

	NormarizeMove = MoveInputPadKey(posMy, rotMy, NormarizeMove);

	if (m_bInput && m_State != STATE_ATTACK)
	{
		//プレイヤーに移動量を反映させる
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= PLAYER_SPEED;

		//移動量を代入
		m_move = NormarizeMove;

		if (m_State == STATE_EGG)
		{
			// モデル数の取得
			int nNumModel = GetNumModel();

			for (int nCnt = 0; nCnt < nNumModel; nCnt++)
			{
				// モデルの取得
				CModel* pModel = GetModel(nCnt);
				
				if (pModel != nullptr)
				{
					pModel->SetDisp(true);
				}
			}

			m_EggMove.y = EGG_MOVE.y;

			switch (m_MoveState)
			{
			case CPlayer::MOVE_STATE_LEFT:
				m_EggMove.x = EGG_MOVE.x;
				m_EggMove.z = 0.0f;
				break;
			case CPlayer::MOVE_STATE_RIGHT:
				m_EggMove.x = -EGG_MOVE.x;
				m_EggMove.z = 0.0f;
				break;
			case CPlayer::MOVE_STATE_UP:
				m_EggMove.x = 0.0f;
				m_EggMove.z = -EGG_MOVE.z;
				break;
			case CPlayer::MOVE_STATE_DOWN:
				m_EggMove.x = 0.0f;
				m_EggMove.z = EGG_MOVE.z;
				break;
			}

			//無敵状態の設定
			m_bInvincible = true;
			m_nInvincibleCount = INVINCIBLE_TIME;
		}

		SetItemType(m_eItemType);

		//移動状態にする
		m_State = STATE_WALK;
	}
}

//====================================================================
//移動入力キーボード
//====================================================================
D3DXVECTOR3 CPlayer::MoveInputKey(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy, D3DXVECTOR3 Move)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//キーボードの移動処理
	if ((pInputKeyboard->GetPress(DIK_W) && m_OKU && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_W) && m_MoveState == MOVE_STATE_DOWN))
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
		MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotMy);

		Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_UP;
	}
	else if (((pInputKeyboard->GetPress(DIK_S) && m_OKD && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_S) && m_MoveState == MOVE_STATE_UP)) &&
		pInputKeyboard->GetPress(DIK_W) == false)
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
		MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotMy);

		Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_DOWN;
	}
	else if ((pInputKeyboard->GetPress(DIK_A) && m_OKL && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_A) && m_MoveState == MOVE_STATE_RIGHT))
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
		MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotMy);

		Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_LEFT;
	}
	else if (((pInputKeyboard->GetPress(DIK_D) && m_OKR && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_D) && m_MoveState == MOVE_STATE_LEFT)) &&
		pInputKeyboard->GetPress(DIK_A) == false)
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
		MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotMy);

		Move.x += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		Move.z -= 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_RIGHT;
	}

	return Move;
}

//====================================================================
//移動入力パッドスティック
//====================================================================
D3DXVECTOR3 CPlayer::MoveInputPadStick(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy, D3DXVECTOR3 Move)
{
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (nCnt == m_nPlayNumber)
		{
			//スティックの移動処理
			if ((pInputJoypad->Get_Stick_Left(nCnt).y > 0.0f && m_OKU && m_bGritCenter) ||
				(pInputJoypad->Get_Stick_Left(nCnt).y > 0.0f && m_MoveState == MOVE_STATE_DOWN))
			{
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotMy);

				Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_UP;
			}
			else if ((pInputJoypad->Get_Stick_Left(nCnt).y < 0.0f && m_OKD && m_bGritCenter) ||
				(pInputJoypad->Get_Stick_Left(nCnt).y < 0.0f && m_MoveState == MOVE_STATE_UP))
			{
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotMy);

				Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_DOWN;
			}
			else if ((pInputJoypad->Get_Stick_Left(nCnt).x < 0.0f && m_OKL && m_bGritCenter) ||
				(pInputJoypad->Get_Stick_Left(nCnt).x < 0.0f && m_MoveState == MOVE_STATE_RIGHT))
			{
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotMy);

				Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_LEFT;
			}
			else if ((pInputJoypad->Get_Stick_Left(nCnt).x > 0.0f && m_OKR && m_bGritCenter) ||
				(pInputJoypad->Get_Stick_Left(nCnt).x > 0.0f && m_MoveState == MOVE_STATE_LEFT))
			{
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotMy);

				Move.x += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.z -= 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_RIGHT;
			}
		}
	}

	return Move;
}

//====================================================================
//移動入力パッドキー
//====================================================================
D3DXVECTOR3 CPlayer::MoveInputPadKey(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy, D3DXVECTOR3 Move)
{
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (nCnt == m_nPlayNumber)
		{
			//キーボードの移動処理
			if ((pInputJoypad->GetPress(CInputJoypad::BUTTON_UP, nCnt) && m_OKU && m_bGritCenter) ||
				(pInputJoypad->GetPress(CInputJoypad::BUTTON_UP, nCnt) && m_MoveState == MOVE_STATE_DOWN))
			{
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotMy);

				Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_UP;
			}
			else if (((pInputJoypad->GetPress(CInputJoypad::BUTTON_DOWN, nCnt) && m_OKD && m_bGritCenter) ||
				(pInputJoypad->GetPress(CInputJoypad::BUTTON_DOWN, nCnt) && m_MoveState == MOVE_STATE_UP)) &&
				pInputJoypad->GetPress(CInputJoypad::BUTTON_UP, nCnt) == false)
			{
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotMy);

				Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_DOWN;
			}
			else if ((pInputJoypad->GetPress(CInputJoypad::BUTTON_LEFT, nCnt) && m_OKL && m_bGritCenter) ||
				(pInputJoypad->GetPress(CInputJoypad::BUTTON_LEFT, nCnt) && m_MoveState == MOVE_STATE_RIGHT))
			{
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotMy);

				Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_LEFT;
			}
			else if (((pInputJoypad->GetPress(CInputJoypad::BUTTON_RIGHT, nCnt) && m_OKR && m_bGritCenter) ||
				(pInputJoypad->GetPress(CInputJoypad::BUTTON_RIGHT, nCnt) && m_MoveState == MOVE_STATE_LEFT)) &&
				pInputJoypad->GetPress(CInputJoypad::BUTTON_LEFT, nCnt) == false)
			{
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotMy);

				Move.x += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.z -= 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_RIGHT;
			}
		}
	}

	return Move;
}

//====================================================================
//移動方向処理
//====================================================================
void CPlayer::Rot(D3DXVECTOR3& rotMy)
{
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//移動方向に向きを合わせる処理
	float fRotMove, fRotDest;
	fRotMove = rotMy.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;

	if (m_State == STATE_WALK)
	{
		rotMy.y = atan2f(-m_move.x, -m_move.z);
	}

	useful::NormalizeAngle(&rotMy);
}

//====================================================================
//攻撃処理
//====================================================================
void CPlayer::Attack(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy)
{
	if (m_State == STATE_WALK)
	{
		//キーボードの取得
		CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
		CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

		if (pInputKeyboard->GetTrigger(DIK_SPACE) || pInputJoypad->GetTrigger(CInputJoypad::BUTTON_B, 0))
		{
			// 火炎放射
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_FIRE);
			D3DXMATRIX mat = *GetUseMultiMatrix();
			D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);

			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_SMOKE, false, ef, rotMy);

			CFire::Create("data\\model\\fireball.x", posMy, rotMy);
			m_State = STATE_ATTACK;
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

	if (pMotion == nullptr)
	{
		return;
	}

	//移動モーション
	if (m_State == STATE_DEATH)
	{
		if (m_Action != ACTION_DEATH)
		{
			m_Action = ACTION_DEATH;
			pMotion->Set(ACTION_DEATH, 5);
		}
	}
	//卵モーション
	else if (m_State == STATE_EGG)
	{
		if (m_Action != ACTION_EGG)
		{
			m_Action = ACTION_EGG;
			pMotion->Set(ACTION_EGG, 5);
		}
	}
	//移動モーション
	else if (m_State == STATE_ATTACK)
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
void CPlayer::StateManager(D3DXVECTOR3& posMy)
{
	switch (m_State)
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

			m_nTime = 0;
		}
		break;

	case STATE_ATTACK:

		if (m_nStateCount == 0)
		{
			m_State = STATE_WALK;
		}

		break;

	case STATE_DEATH:
		if (m_nStateCount == 0)
		{
			//指定位置からブロックが存在しないグリッドを検索してその場所に復活する処理
			int WMax = CMapSystem::GetInstance()->GetWightMax();
			int HMax = CMapSystem::GetInstance()->GetHeightMax();
			CMapSystem::GRID ReivelPos = CMapSystem::GRID(0, 0);
			ReivelPos.x = CMapSystem::GetInstance()->CalcGridX(RESPAWN_POS.x);
			ReivelPos.z = CMapSystem::GetInstance()->CalcGridZ(RESPAWN_POS.z);

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
						posMy = CMapSystem::GRID(nSetW, nSetH).ToWorld();
						posMy.y = RESPAWN_POS.y;
						m_State = STATE_EGG;
						return;
					}

				}
			}
		}

		break;

	case STATE_EGG:

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
			m_pUpEgg = CObjectX::Create("data\\MODEL\\00_Player\\1P\\upper_egg.x");
			m_pUpEgg->SetMatColor(D3DXCOLOR(0.263529f, 0.570980f, 0.238431f, 1.0f));

			m_pUpEgg->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
		}

		if (m_pDownEgg == nullptr)
		{
			m_pDownEgg = CObjectX::Create("data\\MODEL\\00_Player\\1P\\downer_egg.x");
			m_pDownEgg->SetMatColor(D3DXCOLOR(0.263529f, 0.570980f, 0.238431f, 1.0f));

			m_pDownEgg->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
		}
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// 壁との当たり判定
//====================================================================
void CPlayer::CollisionWall(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ)
{
	// 壁のリスト構造が無ければ抜ける
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // リストを取得

	// 壁リストの中身を確認する
	for (CWall* pWall : list)
	{
		D3DXVECTOR3 pos = pWall->GetPos();
		D3DXVECTOR3 posOld = pWall->GetPosOld();
		D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f);
		D3DXVECTOR3 Size = pWall->GetSize();

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, pos, Move, Size, &posMy, posOldMy, &m_move, &m_Objmove, sizeMy, &m_bJump, XYZ) == true)
		{
			//待機状態にする
			m_State = STATE_WAIT;
			m_MoveState = MOVE_STATE_WAIT;
			posMy = m_Grid.ToWorld();
		}
	}
}

//====================================================================
// 壁との圧死判定
//====================================================================
void CPlayer::CollisionPressWall(D3DXVECTOR3& posMy, D3DXVECTOR3& sizeMy,useful::COLLISION XYZ)
{
	// 壁のリスト構造が無ければ抜ける
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // リストを取得

	// キューブブロックリストの中身を確認する
	for (CWall* pWall : list)
	{
		D3DXVECTOR3 pos = pWall->GetPos();
		D3DXVECTOR3 Size = pWall->GetSize();

		// 矩形の当たり判定
		if (useful::CollisionRectangle2D(posMy, pos, sizeMy, Size, XYZ) == true)
		{
			Death();
		}
	}
}

//====================================================================
// 止まっているレールブロックとの当たり判定
//====================================================================
void CPlayer::CollisionWaitRailBlock(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ)
{
	if (m_bPressObj == true)
	{
		return;
	}

	// レールブロックのリスト構造が無ければ抜ける
	if (CRailBlock::GetList() == nullptr) { return; }
	std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // リストを取得

	// レールブロックリストの中身を確認する
	for (CRailBlock* pRailBlock : list)
	{
		D3DXVECTOR3 pos = pRailBlock->GetPos();
		D3DXVECTOR3 posOld = pRailBlock->GetPosOld();
		D3DXVECTOR3 Move = (pos - posOld);
		D3DXVECTOR3 Size = pRailBlock->GetSize();

		if (abs(Move.x) > 0.01f)
		{
			return;
		}
		if (abs(Move.z) > 0.01f)
		{
			return;
		}

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, pos, Move, Size, &posMy, posOldMy, &m_move, &m_Objmove, sizeMy, &m_bJump, XYZ) == true)
		{
			//待機状態にする
			m_State = STATE_WAIT;
			m_MoveState = MOVE_STATE_WAIT;
			posMy = m_Grid.ToWorld();
		}
	}
}

//====================================================================
// 動いているレールブロックとの当たり判定
//====================================================================
void CPlayer::CollisionMoveRailBlock(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ)
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
			float MySize = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

			D3DXVECTOR3 Move = m_move;
			bool a = false;

			// 矩形の当たり判定
			if (useful::CollisionBlock(Mypos, MyposOld, MyMove, D3DXVECTOR3(MySize, MySize, MySize), &posMy, posOldMy, &m_move, &m_Objmove, sizeMy, &a, XYZ) == true)
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
void CPlayer::CollisionWaitRock(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ)
{
	if (m_bPressObj == true)
	{
		return;
	}

	// レールブロックのリスト構造が無ければ抜ける
	if (CRollRock::GetList() == nullptr) { return; }
	std::list<CRollRock*> list = CRollRock::GetList()->GetList();    // リストを取得

	// レールブロックリストの中身を確認する
	for (CRollRock* pRailBlock : list)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(pRailBlock->GetPos().x, 0.0f, pRailBlock->GetPos().z);
		D3DXVECTOR3 posOld = pRailBlock->GetPosOld();
		D3DXVECTOR3 Move = (pos - posOld);
		D3DXVECTOR3 Size = pRailBlock->GetSize();

		switch (XYZ)
		{
		case useful::COLLISION_X:
			if (abs(Move.x) > 0.0f)
			{
				return;
			}
			break;

		case useful::COLLISION_Z:
			if (abs(Move.z) > 0.0f)
			{
				return;
			}
			break;
		}

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, pos, Move, Size, &posMy, posOldMy, &m_move, &m_Objmove, sizeMy, &m_bJump, XYZ) == true)
		{
			//待機状態にする
			m_State = STATE_WAIT;
			m_MoveState = MOVE_STATE_WAIT;
			posMy = m_Grid.ToWorld();
		}
	}
}

//====================================================================
// 動いている岩との当たり判定
//====================================================================
void CPlayer::CollisionMoveRock(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ)
{
	// レールブロックのリスト構造が無ければ抜ける
	if (CRollRock::GetList() == nullptr) { return; }
	std::list<CRollRock*> list = CRollRock::GetList()->GetList();    // リストを取得

	// レールブロックリストの中身を確認する
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
			float MySize = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

			D3DXVECTOR3 Move = m_move;
			bool a = false;

			// 矩形の当たり判定
			if (useful::CollisionBlock(Mypos, MyposOld, MyMove, D3DXVECTOR3(MySize, MySize, MySize), &posMy, posOldMy, &m_move, &m_Objmove, sizeMy, &a, XYZ) == true)
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
void CPlayer::SearchWall(D3DXVECTOR3& posMy)
{
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

	if ((posMy.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) &&
		posMy.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&
		posMy.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		posMy.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK)) ||
		m_State == STATE_WAIT)
	{// グリットの中心位置に立っているなら操作を受け付ける
		m_OKR = OKR;	//右
		m_OKL = OKL;	//左
		m_OKU = OKU;	//上
		m_OKD = OKD;	//下

		m_bGritCenter = true;
	}
	else
	{
		m_OKR = false;	//右
		m_OKL = false;	//左
		m_OKU = false;	//上
		m_OKD = false;	//下

		m_bGritCenter = false;
	}
}

//====================================================================
// デビルホールとの当たり判定
//====================================================================
void CPlayer::CollisionDevilHole(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ)
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
		if (useful::CollisionBlock(pos, pos, INITVECTOR3, Size, &posMy, posOldMy, &m_move, &m_Objmove, sizeMy, &m_bJump, XYZ) == true)
		{
			//待機状態にする
			m_State = STATE_WAIT;
			m_MoveState = MOVE_STATE_WAIT;
			posMy = m_Grid.ToWorld();
		}
	}
}

//====================================================================
// 敵の当たり判定
//====================================================================
void CPlayer::CollisionEnemy(D3DXVECTOR3& posMy)
{
	// 敵のリスト構造が無ければ抜ける
	if (CEnemy::GetList() == nullptr) { return; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // リストを取得

	// 敵のリストの中身を確認する
	for (CEnemy* pEnemy : list)
	{
		D3DXVECTOR3 pos = pEnemy->GetPos();
		D3DXVECTOR3 posOld = pEnemy->GetPosOld();
		D3DXVECTOR3 Size = pEnemy->GetSize();

		// 円の当たり判定
		if (useful::CollisionCircle(posMy, pos, 30.0f) == true)
		{
			if (!m_bInvincible)
			{
				Death();
			}
		}
	}
}

//====================================================================
// ステージ外の当たり判定
//====================================================================
void CPlayer::CollisionStageOut(D3DXVECTOR3& posMy)
{
	D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	float G_Size = CMapSystem::GetInstance()->GetGritSize();

	if (posMy.x + G_Size > D_pos.x + MapSize.x)	// 右
	{
		posMy.x = D_pos.x + MapSize.x - G_Size;
		m_State = STATE_WAIT;
		m_move.x = 0.0f;
	}
	if (posMy.x - G_Size < D_pos.x - MapSize.x)	// 左
	{
		posMy.x = D_pos.x - MapSize.x + G_Size;
		m_State = STATE_WAIT;
		m_move.x = 0.0f;
	}
	if (posMy.z + G_Size > D_pos.z + MapSize.z)	// 上
	{
		posMy.z = D_pos.z + MapSize.z - G_Size;
		m_State = STATE_WAIT;
		m_move.z = 0.0f;
	}
	if (posMy.z - G_Size < D_pos.z - MapSize.z)	// 下
	{
		posMy.z = D_pos.z - MapSize.z + G_Size;
		m_State = STATE_WAIT;
		m_move.z = 0.0f;
	}
}

//====================================================================
// ステージ内にいるかどうか
//====================================================================
bool CPlayer::CollisionStageIn(D3DXVECTOR3& posMy)
{
	D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	float G_Size = CMapSystem::GetInstance()->GetGritSize();

	if (posMy.x + G_Size <= D_pos.x + MapSize.x &&
		posMy.x - G_Size >= D_pos.x - MapSize.x &&
		posMy.z + G_Size <= D_pos.z + MapSize.z &&
		posMy.z - G_Size >= D_pos.z - MapSize.z)
	{
		return true;
	}

	return false;
}

//====================================================================
// 画面外との圧死判定
//====================================================================
void CPlayer::CollisionPressStageOut(D3DXVECTOR3& posMy)
{
	if (m_bPressObj == true)
	{
		D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float G_Size = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		if (posMy.x + G_Size > D_pos.x + MapSize.x)
		{
			Death();
		}
		if (posMy.x - G_Size < D_pos.x - MapSize.x)
		{
			Death();
		}
		if (posMy.z + G_Size > D_pos.z + MapSize.z)
		{
			Death();
		}
		if (posMy.z - G_Size < D_pos.z - MapSize.z)
		{
			Death();
		}
	}
}

//====================================================================
//カメラ位置更新処理
//====================================================================
void CPlayer::CameraPosUpdate(D3DXVECTOR3& posMy)
{
	//カメラ位置の更新
	m_CameraPos.x = posMy.x;
	m_CameraPos.z = posMy.z;

	if (m_bJump == false)
	{
		m_CameraPos.y = posMy.y;
	}
}

//====================================================================
//位置更新処理
//====================================================================
void CPlayer::PosUpdate(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy)
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

	CDevil* pDevil = CDevil::GetListTop();

	//Y軸の位置更新
	posMy.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	CollisionWall(posMy,posOldMy,sizeMy,useful::COLLISION_Y);
	CollisionDevilHole(posMy, posOldMy, sizeMy, useful::COLLISION_Y);

	//X軸の位置更新
	posMy.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(m_move.x);

	// 壁との当たり判定
	CollisionWall(posMy, posOldMy, sizeMy, useful::COLLISION_X);
	CollisionDevilHole(posMy, posOldMy, sizeMy, useful::COLLISION_X);
	CollisionWaitRailBlock(posMy, posOldMy, sizeMy, useful::COLLISION_X);
	CollisionWaitRock(posMy, posOldMy, sizeMy, useful::COLLISION_X);

	//Z軸の位置更新
	posMy.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(m_move.z);

	// 壁との当たり判定
	CollisionWall(posMy, posOldMy, sizeMy, useful::COLLISION_Z);
	CollisionDevilHole(posMy, posOldMy, sizeMy, useful::COLLISION_Z);
	CollisionWaitRailBlock(posMy, posOldMy, sizeMy, useful::COLLISION_Z);
	CollisionWaitRock(posMy, posOldMy, sizeMy, useful::COLLISION_Z);
}

//====================================================================
//オブジェクトによる位置更新処理
//====================================================================
void CPlayer::ObjPosUpdate(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy)
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
	posMy.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	//X軸の位置更新
	posMy.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// レールブロックとの当たり判定
	CollisionMoveRailBlock(posMy, posOldMy, sizeMy, useful::COLLISION_X);
	CollisionMoveRock(posMy, posOldMy, sizeMy, useful::COLLISION_X);

	//Z軸の位置更新
	posMy.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	CollisionMoveRailBlock(posMy, posOldMy, sizeMy, useful::COLLISION_Z);
	CollisionMoveRock(posMy, posOldMy, sizeMy, useful::COLLISION_Z);

	//// 壁との当たり判定
	CollisionPressWall(posMy, sizeMy, useful::COLLISION_ZX);
}

//====================================================================
//向き更新処理
//====================================================================
void CPlayer::RotUpdate(D3DXVECTOR3& rotMy)
{
	// 角度の差分を算出
	D3DXVECTOR3 rotDiff = m_rotDest - rotMy;

	// 正規化
	useful::NormalizeAngle(&rotDiff);

	// 向きの更新処理
	rotMy += (rotDiff * 0.5);
}

//====================================================================
//卵の動き
//====================================================================
void CPlayer::EggMove(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy)
{
	if (m_State == STATE_EGG)
	{
		if (posMy.y > 0.0f)
		{
			//重力
			m_move.y -= RESPAWN_GRAVITY;
		}

		//Y軸の位置更新
		posMy.y += m_move.y * CManager::GetInstance()->GetGameSpeed();

		//卵の落下判定
		if (posMy.y < 0.0f)
		{
			//落下時に移動量と位置を０にする
			posMy.y = 0.0f;
			m_move.y = 0.0f;

			D3DXMATRIX mat = *GetUseMultiMatrix();
			D3DXVECTOR3 ef = useful::CalcMatrix(posMy, rotMy, mat);
			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_FALLSMOKE, false, ef, rotMy);
		}

		if (m_pUpEgg != nullptr)
		{
			m_pUpEgg->SetPos(D3DXVECTOR3(posMy.x, posMy.y + 65.0f, posMy.z));
		}
		if (m_pDownEgg != nullptr)
		{
			m_pDownEgg->SetPos(D3DXVECTOR3(posMy.x, posMy.y + 65.0f, posMy.z));
		}
		m_EggMove = INITVECTOR3;
	}
	else
	{
		if (m_pUpEgg != nullptr)
		{
			D3DXVECTOR3 pos = m_pUpEgg->GetPos();
			D3DXVECTOR3 rot = m_pUpEgg->GetRot();
			float ColorA = m_pUpEgg->GetMatColor().a;

			ColorA -= EGG_COLOR_DEL_A;

			m_EggMove.y -= EGG_GRAVITY;

			pos += m_EggMove;

			rot.z -= m_EggMove.x * EGG_ROT;
			rot.x += m_EggMove.z * EGG_ROT;

			m_EggMove.x = m_EggMove.x * EGG_MOVE_DEL;
			m_EggMove.z = m_EggMove.z * EGG_MOVE_DEL;

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

//====================================================================
//ダメージ処理
//====================================================================
void CPlayer::Death(void)
{

}

//====================================================================
// デバッグボタン
//====================================================================
void CPlayer::DebugKey(void)
{
#ifdef _DEBUG

	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//キーボードの移動処理
	if (pInputKeyboard->GetTrigger(DIK_3))
	{
		m_nLife++;
		m_pLifeUi->GetNumber()->SetNumber(m_nLife);;
	}
	if (pInputKeyboard->GetTrigger(DIK_4))
	{
		m_nLife--;
		m_pLifeUi->GetNumber()->SetNumber(m_nLife);;
	}

#endif // !_DEBUG
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

	// 所持しているアイテムによってモデルの表示を切り替える
	switch (eType)
	{
	case TYPE_CROSS:
		SetPartsDisp(9, true);		// 十字架のモデル表示
		SetPartsDisp(10, false);	// 聖書のモデル非表示

		// 所持時間タイマーをリセット
		m_fCrossTimer = 0.0f;

		break;

	case TYPE_BIBLE:
		SetPartsDisp(9, false);		// 十字架のモデル非表示
		SetPartsDisp(10, true);		// 聖書のモデル表示
		break;

	default:
		SetPartsDisp(9, false);		// 十字架のモデル非表示
		SetPartsDisp(10, false);	// 聖書のモデル非表示
		break;
	}
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

//====================================================================
// プレイヤーの指定モデル消去
//====================================================================
void CPlayer::SetModelColor(CModel::COLORTYPE Type, D3DXCOLOR Col)
{
	// モデル数の取得
	int nNumModel = GetNumModel();

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// モデルの取得
		CModel* pModel = GetModel(nCnt);

		if (pModel != nullptr)
		{
			pModel->SetColorType(Type);
			pModel->SetColor(Col);
		}
	}
}
//==========================================
//  リストの取得
//==========================================
CListManager<CPlayer>* CPlayer::GetList(void)
{
	return m_pList;
}
