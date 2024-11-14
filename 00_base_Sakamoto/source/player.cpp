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
#include "2DEffect.h"
#include "camera.h"
#include "input.h"
#include "enemy.h"
#include "objGauge2D.h"
#include "CubeBlock.h"
#include "slowManager.h"
#include "Number.h"
#include "MapModel.h"
#include "effect.h"
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
#include "CubeBlock.h"
#include "RollRock.h"

#include "MyEffekseer.h"

//===========================================
// 定数定義
//===========================================
namespace
{
	const int LIFE_MAX = 2;	//初期ライフ数
	const int FIRE_STOPTIME = 30;	//攻撃時の移動停止時間

	const float GRIT_OK = 45.0f;			//移動可能なグリットの範囲内
	const float PLAYER_SPEED = 5.0f;		//プレイヤーの移動速度
	const float OBJDISTANCE = 10000.0f;		// オブジェクトの距離

	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(35.0f, 40.0f, 35.0f);		//横の当たり判定
	const D3DXVECTOR3 LIFE_POS = D3DXVECTOR3(50.0f, 650.0f, 0.0f);

	const float CROSS_TIME = 10.0f; // 十字架を所持していられる時間

	const float EGG_GRAVITY = 0.98f;	 //移動量の減衰速度
	const D3DXVECTOR3 EGG_MOVE = D3DXVECTOR3(10.0f, 10.0f, 10.0f);	 //移動量の減衰速度
	const float EGG_ROT = D3DX_PI * 0.006f;	 //回転速度
	const float EGG_MOVE_DEL = 0.9f;	 //移動量の減衰速度
	const float EGG_COLOR_DEL_A = 0.01f; //不透明度の減衰速度
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CPlayer>* CPlayer::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CPlayer::CPlayer(int nPriority) : CObject(nPriority),
m_size(INITVECTOR3),
m_pos(INITVECTOR3),
m_move(INITVECTOR3),
m_Objmove(INITVECTOR3),
m_rot(INITVECTOR3),
m_AutoMoveRot(INITVECTOR3),
m_bJump(false),
m_nActionCount(0),
m_Action(ACTION_NONE),
m_AtkAction(ACTION_NONE),
m_State(STATE_EGG),
m_nStateCount(0),
m_AtkPos(INITVECTOR3),
m_CollisionRot(0.0f),
m_pMotion(nullptr),
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
m_EggMove(INITVECTOR3)
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
CPlayer* CPlayer::Create()
{
	CPlayer* pPlayer = new CPlayer();

	// メモリの確保に失敗した場合nullを返す
	if (pPlayer == nullptr) { assert(false); return nullptr; }

	// 初期化処理に失敗した場合nullを返す
	if (FAILED(pPlayer->Init()))
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
HRESULT CPlayer::Init(void)
{
	// サイズの設定
	m_size = COLLISION_SIZE;

	// 向きの設定
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
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
	SetMultiMatrix(true);

	//モデルの生成
	LoadLevelData("data\\TXT\\motion_tamagon.txt");

	//モーションの生成
	if (m_pMotion == nullptr)
	{
		//モーションの生成
		m_pMotion = new CMotion;
	}

	//初期化処理
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData("data\\TXT\\motion_tamagon.txt");

	m_pLifeUi = CLifeUi::Create();

	// 数値
	m_pLifeUi->GetNumber()->SetPos(D3DXVECTOR3(LIFE_POS.x + 200.0f, LIFE_POS.y, LIFE_POS.z));

	// アイテム状態を設定
	SetItemType(CPlayer::TYPE_NONE);

	// 体力
	m_pLifeUi->SetPos(LIFE_POS);
	m_pLifeUi->GetNumber()->SetNumber(m_nLife);

	// スローの生成
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_PLAYER, CSlowManager::TAG_PLAYER);

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

	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Uninit();
		delete m_apModel[nCntModel];
		m_apModel[nCntModel] = nullptr;
	}

	//モーションの終了処理
	if (m_pMotion != nullptr)
	{
		//モーションの破棄
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CPlayer::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
		GameUpdate();
		break;

	case CScene::MODE_TUTORIAL:
		TutorialUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//タイトルでの更新処理
//====================================================================
void CPlayer::TitleUpdate(void)
{
	//モーションの更新
	m_pMotion->Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CPlayer::GameUpdate(void)
{
	// 過去の位置に代入
	m_posOld = m_pos;

	if (m_State != STATE_DEATH)
	{
		//壁があるか判断
		SearchWall();

		// 移動処理
		Move();

		// 向き移動処理
		Rot();

		if (m_eItemType != TYPE_NONE)
		{
			Attack();
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
		CameraPosUpdate();

		if (m_State == STATE_WALK)
		{
			// 位置更新処理
			PosUpdate();
		}

		ObjPosUpdate();

		if (m_State != STATE_EGG)
		{
			//画面外判定
			CollisionStageOut();
		}

		// 敵の判定
		CollisionEnemy();
	}

	// プレイヤーがマップのどのマスに存在しているか設定する
	m_Grid.x = CMapSystem::GetInstance()->CMapSystem::CalcGridX(m_pos.x);
	m_Grid.z = CMapSystem::GetInstance()->CMapSystem::CalcGridZ(m_pos.z);

	//状態の管理
	StateManager();

	//卵の動き
	EggMove();

	if (m_pMotion != nullptr)
	{
		//モーションの更新
		m_pMotion->Update();
	}

	//モーションの管理
	ActionState();

	//デバッグキーの処理と設定
	DebugKey();

	//デバッグ表示
	DebugProc::Print(DebugProc::POINT_LEFT, "[自分]位置 %f : %f : %f\n", m_pos.x, m_pos.y, m_pos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[自分]向き %f : %f : %f\n", m_rot.x, m_rot.y, m_rot.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[自分]横 %d : 縦 %d\n", m_Grid.x, m_Grid.z);
}

//====================================================================
//チュートリアルでの更新処理
//====================================================================
void CPlayer::TutorialUpdate(void)
{

}

//====================================================================
//描画処理
//====================================================================
void CPlayer::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_bMultiMatrix)
	{
		SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());

		//算出したマトリクスをかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&m_UseMultiMatrix);
	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

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

	//モデルの描画(全パーツ)
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//====================================================================
//移動処理
//====================================================================
void CPlayer::Move(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_bInput = false;

	//キーボードの移動処理
	if ((pInputKeyboard->GetPress(DIK_W) && m_OKU && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_W) && m_MoveState == MOVE_STATE_DOWN))
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

		NormarizeMove.z += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		NormarizeMove.x += 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_UP;
	}
	else if ((pInputKeyboard->GetPress(DIK_S) && m_OKD && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_S) && m_MoveState == MOVE_STATE_UP))
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

		NormarizeMove.z += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		NormarizeMove.x += -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_DOWN;
	}
	else if ((pInputKeyboard->GetPress(DIK_A) && m_OKL && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_A) && m_MoveState == MOVE_STATE_RIGHT))
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

		NormarizeMove.x += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		NormarizeMove.z -= -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_LEFT;
	}
	else if ((pInputKeyboard->GetPress(DIK_D) && m_OKR && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_D) && m_MoveState == MOVE_STATE_LEFT))
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

		NormarizeMove.x += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		NormarizeMove.z -= 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_RIGHT;
	}

	if (pInputKeyboard->GetPress(DIK_W) == false && pInputKeyboard->GetPress(DIK_A) == false && pInputKeyboard->GetPress(DIK_S) == false && pInputKeyboard->GetPress(DIK_D) == false)
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

		//左スティックによる前後移動	
		m_move.z += pInputJoypad->Get_Stick_Left(0).y * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		m_move.x += pInputJoypad->Get_Stick_Left(0).y * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		//左スティックによる左右移動
		m_move.x += pInputJoypad->Get_Stick_Left(0).x * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		m_move.z -= pInputJoypad->Get_Stick_Left(0).x * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;
	}

	if (m_bInput && m_State != STATE_ATTACK)
	{
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
			for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
			{
				if (m_apModel[nCnt] != nullptr)
				{
					m_apModel[nCnt]->SetDisp(true);
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
		}

		SetItemType(m_eItemType);

		//移動状態にする
		m_State = STATE_WALK;
	}
}

//====================================================================
//移動方向処理
//====================================================================
void CPlayer::Rot(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//移動方向に向きを合わせる処理
	float fRotMove, fRotDest;
	fRotMove = m_rot.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;

	if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{
		m_rot.y = atan2f(-m_move.x, -m_move.z);
	}

	useful::NormalizeAngle(&m_rot);
}

//====================================================================
//攻撃処理
//====================================================================
void CPlayer::Attack(void)
{
	if (m_State == STATE_WALK)
	{
		//キーボードの取得
		CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
		{
			// 火炎放射
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_FIRE);

			//計算用マトリックス
			D3DXMATRIX mtxRot, mtxTrans;

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&m_mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

			if (m_bMultiMatrix)
			{
				SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());

				//算出したマトリクスをかけ合わせる
				D3DXMatrixMultiply(&m_mtxWorld,
					&m_mtxWorld,
					&m_UseMultiMatrix);
			}

			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_HIT, false, D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43), m_rot);

			CFire::Create("data\\model\\fireball.x", m_pos, m_rot);
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
	//移動モーション
	if (m_State == STATE_DEATH)
	{
		if (m_Action != ACTION_DEATH)
		{
			m_Action = ACTION_DEATH;
			m_pMotion->Set(ACTION_DEATH, 5);
		}
	}
	//卵モーション
	else if (m_State == STATE_EGG)
	{
		if (m_Action != ACTION_EGG)
		{
			m_Action = ACTION_EGG;
			m_pMotion->Set(ACTION_EGG, 5);
		}
	}
	//移動モーション
	else if (m_State == STATE_ATTACK)
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			m_pMotion->Set(ACTION_WAIT, 5);
		}
	}
	//移動モーション
	else if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			m_pMotion->Set(ACTION_MOVE, 5);
		}
	}
	//ニュートラルモーション
	else
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			m_pMotion->Set(ACTION_WAIT, 5);
		}
	}
}

//====================================================================
//状態管理
//====================================================================
void CPlayer::StateManager(void)
{
	switch (m_State)
	{
	case STATE_WAIT:
		//	スローをdefaultへ
		CSlowManager::SetValueDefault();
		break;

	case STATE_WALK:
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
			int WMax = CMapSystem::GetInstance()->GetWightMax();
			int HMax = CMapSystem::GetInstance()->GetHeightMax();
			CMapSystem::GRID ReivelPos = CMapSystem::GRID(0, 0);
			ReivelPos.x = CMapSystem::GetInstance()->CalcGridX(-100.0f);
			ReivelPos.z = CMapSystem::GetInstance()->CalcGridZ(100.0f);

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
						m_pos = CMapSystem::GetInstance()->GetGritPos(CMapSystem::GRID(nSetW, nSetH));
						m_State = STATE_EGG;
						return;
					}

				}
			}
		}

		break;

	case STATE_EGG:

		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			if (m_apModel[nCnt] != nullptr)
			{
				m_apModel[nCnt]->SetDisp(false);
			}
		}

		if (m_pUpEgg == nullptr)
		{
			m_pUpEgg = CObjectX::Create("data\\MODEL\\00_tamagon\\upper_egg.x");
			m_pUpEgg->SetMatColor(D3DXCOLOR(0.263529f, 0.570980f, 0.238431f, 1.0f));
			m_pUpEgg->SetMultiMatrix(true);
		}

		if (m_pDownEgg == nullptr)
		{
			m_pDownEgg = CObjectX::Create("data\\MODEL\\00_tamagon\\downer_egg.x");
			m_pDownEgg->SetMatColor(D3DXCOLOR(0.263529f, 0.570980f, 0.238431f, 1.0f));
			m_pDownEgg->SetMultiMatrix(true);
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
void CPlayer::CollisionWall(useful::COLLISION XYZ)
{
	// キューブブロックのリスト構造が無ければ抜ける
	if (CCubeBlock::GetList() == nullptr) { return; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // リストを取得

	// キューブブロックリストの中身を確認する
	for (CCubeBlock* pCubeBlock : list)
	{
		D3DXVECTOR3 pos = pCubeBlock->GetPos();
		D3DXVECTOR3 posOld = pCubeBlock->GetPosOld();
		D3DXVECTOR3 Move = pCubeBlock->GetMove();
		D3DXVECTOR3 Size = pCubeBlock->GetSize();

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, pos, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
		{
			//待機状態にする
			m_State = STATE_WAIT;
			m_MoveState = MOVE_STATE_WAIT;
			m_pos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
		}
	}
}

//====================================================================
// 壁との圧死判定
//====================================================================
void CPlayer::CollisionPressWall(useful::COLLISION XYZ)
{
	// キューブブロックのリスト構造が無ければ抜ける
	if (CCubeBlock::GetList() == nullptr) { return; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // リストを取得

	// キューブブロックリストの中身を確認する
	for (CCubeBlock* pCubeBlock : list)
	{
		D3DXVECTOR3 pos = pCubeBlock->GetPos();
		D3DXVECTOR3 posOld = pCubeBlock->GetPosOld();
		D3DXVECTOR3 Move = pCubeBlock->GetMove();
		D3DXVECTOR3 Size = pCubeBlock->GetSize();

		// 矩形の当たり判定
		if (useful::CollisionRectangle2D(m_pos, pos, m_size, Size, XYZ) == true)
		{
			Death();
		}
	}
}

//====================================================================
// 止まっているレールブロックとの当たり判定
//====================================================================
void CPlayer::CollisionWaitRailBlock(useful::COLLISION XYZ)
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
		if (useful::CollisionBlock(pos, pos, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
		{
			//待機状態にする
			m_State = STATE_WAIT;
			m_MoveState = MOVE_STATE_WAIT;
			m_pos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
		}
	}
}

//====================================================================
// 動いているレールブロックとの当たり判定
//====================================================================
void CPlayer::CollisionMoveRailBlock(useful::COLLISION XYZ)
{
	// レールブロックのリスト構造が無ければ抜ける
	if (CRailBlock::GetList() == nullptr) { return; }
	std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // リストを取得

	// レールブロックリストの中身を確認する
	for (CRailBlock* pRailBlock : list)
	{
		D3DXVECTOR3 D_pos = CGame::GetDevil()->GetDevilPos();
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

			D3DXVECTOR3 pos = m_pos;
			D3DXVECTOR3 posOld = m_posOld;
			D3DXVECTOR3 Size = m_size;
			D3DXVECTOR3 Move = m_move;
			bool a = false;

			// 矩形の当たり判定
			if (useful::CollisionBlock(Mypos, MyposOld, MyMove, D3DXVECTOR3(MySize, MySize, MySize), &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &a, XYZ) == true)
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
void CPlayer::CollisionWaitRock(useful::COLLISION XYZ)
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
		if (useful::CollisionBlock(pos, pos, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
		{
			//待機状態にする
			m_State = STATE_WAIT;
			m_MoveState = MOVE_STATE_WAIT;
			m_pos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
		}
	}
}

//====================================================================
// 動いている岩との当たり判定
//====================================================================
void CPlayer::CollisionMoveRock(useful::COLLISION XYZ)
{
	// レールブロックのリスト構造が無ければ抜ける
	if (CRollRock::GetList() == nullptr) { return; }
	std::list<CRollRock*> list = CRollRock::GetList()->GetList();    // リストを取得

	// レールブロックリストの中身を確認する
	for (CRollRock* pRock : list)
	{
		D3DXVECTOR3 D_pos = CGame::GetDevil()->GetDevilPos();
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

			D3DXVECTOR3 pos = m_pos;
			D3DXVECTOR3 posOld = m_posOld;
			D3DXVECTOR3 Size = m_size;
			D3DXVECTOR3 Move = m_move;
			bool a = false;

			// 矩形の当たり判定
			if (useful::CollisionBlock(Mypos, MyposOld, MyMove, D3DXVECTOR3(MySize, MySize, MySize), &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &a, XYZ) == true)
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
void CPlayer::SearchWall(void)
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
	D3DXVECTOR3 MyGritPos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "自分がいるグリットの中心位置 %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if ((m_pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) &&
		m_pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&
		m_pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		m_pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK)) ||
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
void CPlayer::CollisionDevilHole(useful::COLLISION XYZ)
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
		if (useful::CollisionBlock(pos, pos, INITVECTOR3, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
		{
			//待機状態にする
			m_State = STATE_WAIT;
			m_MoveState = MOVE_STATE_WAIT;
			m_pos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
		}
	}
}

//====================================================================
// 敵の当たり判定
//====================================================================
void CPlayer::CollisionEnemy(void)
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
		if (useful::CollisionCircle(m_pos, pos, 30.0f) == true)
		{
			Death();
		}
	}
}

//====================================================================
// ステージ外の当たり判定
//====================================================================
void CPlayer::CollisionStageOut(void)
{
	D3DXVECTOR3 D_pos = CGame::GetDevil()->GetDevilPos();
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	float G_Size = CMapSystem::GetInstance()->GetGritSize();

	if (m_pos.x + G_Size > D_pos.x + MapSize.x)	// 右
	{
		m_pos.x = D_pos.x + MapSize.x - G_Size;
		m_State = STATE_WAIT;
		m_move.x = 0.0f;
	}
	if (m_pos.x - G_Size < D_pos.x - MapSize.x)	// 左
	{
		m_pos.x = D_pos.x - MapSize.x + G_Size;
		m_State = STATE_WAIT;
		m_move.x = 0.0f;
	}
	if (m_pos.z + G_Size > D_pos.z + MapSize.z)	// 上
	{
		m_pos.z = D_pos.z + MapSize.z - G_Size;
		m_State = STATE_WAIT;
		m_move.z = 0.0f;
	}
	if (m_pos.z - G_Size < D_pos.z - MapSize.z)	// 下
	{
		m_pos.z = D_pos.z - MapSize.z + G_Size;
		m_State = STATE_WAIT;
		m_move.z = 0.0f;
	}
}

//====================================================================
// 画面外との圧死判定
//====================================================================
void CPlayer::CollisionPressStageOut(void)
{
	if (m_bPressObj == true)
	{
		D3DXVECTOR3 D_pos = CGame::GetDevil()->GetDevilPos();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float G_Size = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		if (m_pos.x + G_Size > D_pos.x + MapSize.x)
		{
			Death();
		}
		if (m_pos.x - G_Size < D_pos.x - MapSize.x)
		{
			Death();
		}
		if (m_pos.z + G_Size > D_pos.z + MapSize.z)
		{
			Death();
		}
		if (m_pos.z - G_Size < D_pos.z - MapSize.z)
		{
			Death();
		}
	}
}

//====================================================================
//カメラ位置更新処理
//====================================================================
void CPlayer::CameraPosUpdate(void)
{
	//カメラ位置の更新
	m_CameraPos.x = m_pos.x;
	m_CameraPos.z = m_pos.z;

	if (m_bJump == false)
	{
		m_CameraPos.y = m_pos.y;
	}
}

//====================================================================
//位置更新処理
//====================================================================
void CPlayer::PosUpdate(void)
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

	CDevil* pDevil = CGame::GetDevil();

	//Y軸の位置更新
	m_pos.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_Y);
	CollisionDevilHole(useful::COLLISION_Y);

	//X軸の位置更新
	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(m_move.x);

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_X);
	CollisionDevilHole(useful::COLLISION_X);
	CollisionWaitRailBlock(useful::COLLISION_X);
	CollisionWaitRock(useful::COLLISION_X);

	//Z軸の位置更新
	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(m_move.z);

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_Z);
	CollisionDevilHole(useful::COLLISION_Z);
	CollisionWaitRailBlock(useful::COLLISION_Z);
	CollisionWaitRock(useful::COLLISION_Z);
}

//====================================================================
//オブジェクトによる位置更新処理
//====================================================================
void CPlayer::ObjPosUpdate(void)
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

	CDevil* pDevil = CGame::GetDevil();

	//Y軸の位置更新
	m_pos.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	//X軸の位置更新
	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// レールブロックとの当たり判定
	CollisionMoveRailBlock(useful::COLLISION_X);
	CollisionMoveRock(useful::COLLISION_X);

	//Z軸の位置更新
	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	CollisionMoveRailBlock(useful::COLLISION_Z);
	CollisionMoveRock(useful::COLLISION_Z);

	//// 壁との当たり判定
	CollisionPressWall(useful::COLLISION_ZX);
}

//====================================================================
//向き更新処理
//====================================================================
void CPlayer::RotUpdate(void)
{
	// 角度の差分を算出
	D3DXVECTOR3 rotDiff = m_rotDest - m_rot;

	// 正規化
	useful::NormalizeAngle(&rotDiff);

	// 向きの更新処理
	m_rot += (rotDiff * 0.5);
}

//====================================================================
//卵の動き
//====================================================================
void CPlayer::EggMove(void)
{
	if (m_State == STATE_EGG)
	{
		if (m_pUpEgg != nullptr)
		{
			m_pUpEgg->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + 65.0f, m_pos.z));
		}
		if (m_pDownEgg != nullptr)
		{
			m_pDownEgg->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + 65.0f, m_pos.z));
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

			if (pos.y < CGame::GetMapField()->GetPos().y + 30.0f)
			{
				pos.y = CGame::GetMapField()->GetPos().y + 30.0f;
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
	if (m_State != STATE_EGG && m_State != STATE_DEATH)
	{
		m_nLife--;

		// アイテムを所持していない状態にする
		SetItemType(TYPE_NONE);

		// 聖書を所持しているときにその場に聖書を落とす
		if (m_eItemType == TYPE_BIBLE)
		{
			// 聖書生成
			CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(m_Grid.x, m_Grid.z));
		}

		if (m_nLife < 0)
		{
			// 死亡音
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DEATH);

			CGame::SetGameEnd(true);
			CGame::SetGameClear(false);
			CManager::GetInstance()->SetStage(0);
		}
		else
		{
			if (m_pLifeUi != nullptr)
			{
				m_pLifeUi->GetNumber()->SetNumber(m_nLife);
			}

			m_State = STATE_DEATH;
			m_move = INITVECTOR3;
			m_Objmove = INITVECTOR3;
			m_nStateCount = 150;

			// ダメージ音(仮)
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DEATH);
		}
	}
}

//====================================================================
// デバッグボタン
//====================================================================
void CPlayer::DebugKey(void)
{
#ifdef _DEBUG

	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse->GetTrigger(pMouse->PUSH_WHEEL))
	{
		HitDamage(10.0f);
	}
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
	if (m_Action != Action)
	{
		m_Action = Action;
		m_pMotion->Set(Action, BlendTime);
	}
}

//====================================================================
// モデル表示の設定
//====================================================================
void CPlayer::SetModelDisp(bool Sst)
{
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			m_apModel[nCnt]->SetDisp(Sst);
		}
	}
}

//====================================================================
//マップオブジェクトと壁とのソート処理
//====================================================================
bool CPlayer::SortObject(D3DXVECTOR3 pos)
{
	float fDistance = sqrtf((m_pos.x - pos.x) * (m_pos.x - pos.x) + (m_pos.z - pos.z) * (m_pos.z - pos.z));

	// 敵のリスト構造が無ければ抜ける
	if (CCubeBlock::GetList() == nullptr) { return false; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // リストを取得

	// 敵のリストの中身を確認する
	for (CCubeBlock* pCubeBlock : list)
	{
		if (pCubeBlock->GetPos().y <= 200.0f)
		{
			if (useful::CollisionCircle(m_pos, pCubeBlock->GetPos(), 250.0f) == true)
			{
				D3DXVECTOR3 CrossPos = INITVECTOR3;
				float ObjDistance = OBJDISTANCE;

				//交点の位置を求める
				for (int nCnt = 0; nCnt < 4; nCnt++)
				{
					switch (nCnt)
					{
					case 0:
						CrossPos = useful::CrossIntersection(m_pos, pos,
							D3DXVECTOR3(pCubeBlock->GetPos().x + pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z + pCubeBlock->GetSize().z),
							D3DXVECTOR3(pCubeBlock->GetPos().x - pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z + pCubeBlock->GetSize().z),
							250.0f);
						break;

					case 1:
						CrossPos = useful::CrossIntersection(m_pos, pos,
							D3DXVECTOR3(pCubeBlock->GetPos().x - pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z + pCubeBlock->GetSize().z),
							D3DXVECTOR3(pCubeBlock->GetPos().x - pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z - pCubeBlock->GetSize().z),
							250.0f);
						break;

					case 2:
						CrossPos = useful::CrossIntersection(m_pos, pos,
							D3DXVECTOR3(pCubeBlock->GetPos().x - pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z - pCubeBlock->GetSize().z),
							D3DXVECTOR3(pCubeBlock->GetPos().x + pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z - pCubeBlock->GetSize().z),
							250.0f);
						break;

					case 3:
						CrossPos = useful::CrossIntersection(m_pos, pos,
							D3DXVECTOR3(pCubeBlock->GetPos().x + pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z - pCubeBlock->GetSize().z),
							D3DXVECTOR3(pCubeBlock->GetPos().x + pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z + pCubeBlock->GetSize().z),
							250.0f);
						break;
					}

					ObjDistance = sqrtf((m_pos.x - CrossPos.x) * (m_pos.x - CrossPos.x) + (m_pos.z - CrossPos.z) * (m_pos.z - CrossPos.z));

					if (ObjDistance < fDistance)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
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
//ロード処理
//====================================================================
void CPlayer::LoadLevelData(const char* pFilename)
{
	FILE* pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(pFilename, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		int ModelParent = 0;
		D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 ModelRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		char ModelName[128] = {};
		int nCntModel = 0;
		int nCntParts = 0;
		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//ゴミ箱
		char aMessage[128] = {};			//スタートとエンドのメッセージ
		char aBool[128] = {};				//bool変換用メッセージ

		// 読み込み開始-----------------------------------------------------
		while (1)
		{//「SCRIPT」を探す
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// モデル数読み込み-----------------------------------------------------
				while (1)
				{//「NUM_MODEL」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%d", &m_nNumModel);		//モデル数の設定
						break;
					}
				}

				//モデルファイルの読み込み
				while (1)
				{//「MODEL_FILENAME」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &ModelName[0]);		//読み込むモデルのパスを取得

						m_apModel[nCntModel] = CModel::Create(&ModelName[0]);
						nCntModel++;
					}
					if (nCntModel >= m_nNumModel)
					{
						nCntModel = 0;
						break;
					}
				}

				// キャラクター情報読み込み-----------------------------------------------------
				while (1)
				{//「PARTSSET」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "PARTSSET") == 0)
					{
						while (1)
						{//各種変数を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "INDEX") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &nCntModel);	//インデックスを設定
							}
							if (strcmp(&aMessage[0], "PARENT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParent);	//親モデルのインデックスを設定

								if (ModelParent == -1)
								{
									m_apModel[nCntModel]->SetParent(nullptr);
								}
								else
								{
									m_apModel[nCntModel]->SetParent(m_apModel[ModelParent]);
								}
							}
							if (strcmp(&aMessage[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelPos.x);				//位置(オフセット)の初期設定
								fscanf(pFile, "%f", &ModelPos.y);				//位置(オフセット)の初期設定
								fscanf(pFile, "%f", &ModelPos.z);				//位置(オフセット)の初期設定

								m_apModel[nCntModel]->SetPos(ModelPos);
								m_apModel[nCntModel]->SetStartPos(ModelPos);
							}
							if (strcmp(&aMessage[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelRot.x);				////向きの初期設定
								fscanf(pFile, "%f", &ModelRot.y);				////向きの初期設定
								fscanf(pFile, "%f", &ModelRot.z);				////向きの初期設定

								m_apModel[nCntModel]->SetRot(ModelRot);
								m_apModel[nCntModel]->SetStartRot(ModelRot);
							}
							if (strcmp(&aMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}
						nCntModel++;
						if (nCntModel >= m_nNumModel)
						{
							break;
						}
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}

//====================================================================
// プレイヤーの指定モデル消去
//====================================================================
void CPlayer::SetPartsDisp(int nParts, bool Set)
{
	if (m_apModel[nParts] != nullptr)
	{
		m_apModel[nParts]->SetDisp(Set);
	}
}

//==========================================
//  リストの取得
//==========================================
CListManager<CPlayer>* CPlayer::GetList(void)
{
	return m_pList;
}
