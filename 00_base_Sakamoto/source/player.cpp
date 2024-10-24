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
#include "CubeColl.h"
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
#include "MapSystem.h"

//===========================================
// 定数定義
//===========================================
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(45.0f, 40.0f, 45.0f);		//横の当たり判定
	const float PLAYER_SPEED = 5.0f;		//プレイヤーの移動速度
	const int LIFE_MAX = 2;	//初期ライフ数

	const D3DXVECTOR3 LIFE_POS = D3DXVECTOR3(50.0f, 650.0f, 0.0f);
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CPlayer>* CPlayer::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{
	SetSize(COLLISION_SIZE);
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_bJump = false;
	m_nActionCount = 0;
	m_Action = ACTION_BWAIT;
	m_AtkAction = ACTION_BWAIT;
	m_State = STATE_EGG;
	m_nStateCount = 0;
	m_AtkPos = INITVECTOR3;
	m_CollisionRot = 0.0f;
	m_pMotion = nullptr;
	m_OKL = true;
	m_OKR = true;
	m_OKU = true;
	m_OKD = true;
	m_bInput = false;
	m_UseItem = false;
	m_pLifeUi = nullptr;
	m_nLife = LIFE_MAX;
	m_eItemType = TYPE_NONE;
	m_MoveState = MOVE_STATE_WAIT;
	m_nMapWight = 0;
	m_nMapHeight = 0;
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
	CPlayer* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//プレイヤーの生成
		pPlayer = new CPlayer();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPlayer->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pPlayer;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CPlayer::Init(void)
{
	if (CScene::GetMode() == CScene::MODE_GAME ||
		CScene::GetMode() == CScene::MODE_TUTORIAL)
	{
		MyObjCreate();
	}

	//種類設定
	SetType(CObject::TYPE_PLAYER3D);

	//モデルの生成
	LoadLevelData("data\\TXT\\motion_tamagon.txt");

	// プレイヤーの指定パーツ削除
	SetPartsDisp(3, false);
	SetPartsDisp(0, false);

	//モーションの生成
	if (m_pMotion == nullptr)
	{
		//モーションの生成
		m_pMotion = new CMotion;
	}

	//初期化処理
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData("data\\TXT\\motion_tamagon.txt");

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		break;

	case CScene::MODE_RESULT:
		break;
	}

	m_pLifeUi = CLifeUi::Create();

	// 数値
	m_pLifeUi->GetNumber()->SetPos(D3DXVECTOR3(LIFE_POS.x + 200.0f, LIFE_POS.y, LIFE_POS.z));

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
//自分が保持するオブジェクトの生成
//====================================================================
void CPlayer::MyObjCreate(void)
{
	//オブジェクト生成
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

		if (m_UseItem)
		{
			Attack();
		}

		// カメラ更新処理
		CameraPosUpdate();

		// 位置更新処理
		PosUpdate();

		if (m_State != STATE_EGG)
		{
			//画面外判定
			CollisionStageOut();
		}

		// 敵の判定
		CollisionEnemy();
	}

	// プレイヤーがマップのどのマスに存在しているか設定する
	MapSystemNumber();

	//状態の管理
	StateManager();

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
	DebugProc::Print(DebugProc::POINT_LEFT,"[自分]位置 %f : %f : %f\n", m_pos.x, m_pos.y, m_pos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[自分]向き %f : %f : %f\n", m_rot.x, m_rot.y, m_rot.z);
	DebugProc::Print(DebugProc::POINT_LEFT,"[自分]横 %d : 縦 %d\n", m_nMapWight, m_nMapHeight);
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
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデルの描画(全パーツ)
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}
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
	if ((pInputKeyboard->GetPress(DIK_W) && m_OKU) ||
		(pInputKeyboard->GetPress(DIK_W) && m_MoveState == MOVE_STATE_DOWN))
	{
		NormarizeMove.z += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		NormarizeMove.x += 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_UP;
	}
	if ((pInputKeyboard->GetPress(DIK_S) && m_OKD) ||
		(pInputKeyboard->GetPress(DIK_S) && m_MoveState == MOVE_STATE_UP))
	{
		NormarizeMove.z += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		NormarizeMove.x += -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_DOWN;
	}
	if ((pInputKeyboard->GetPress(DIK_A) && m_OKL) ||
		(pInputKeyboard->GetPress(DIK_A) && m_MoveState == MOVE_STATE_RIGHT))
	{
		NormarizeMove.x += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		NormarizeMove.z -= -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_LEFT;
	}
	if ((pInputKeyboard->GetPress(DIK_D) && m_OKR) ||
		(pInputKeyboard->GetPress(DIK_D) && m_MoveState == MOVE_STATE_LEFT))
	{
		NormarizeMove.x += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		NormarizeMove.z -= 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_RIGHT;
	}

	if (pInputKeyboard->GetPress(DIK_W) == false && pInputKeyboard->GetPress(DIK_A) == false && pInputKeyboard->GetPress(DIK_S) == false && pInputKeyboard->GetPress(DIK_D) == false)
	{
		//左スティックによる前後移動	
		m_move.z += pInputJoypad->Get_Stick_Left(0).y * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		m_move.x += pInputJoypad->Get_Stick_Left(0).y * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		//左スティックによる左右移動
		m_move.x += pInputJoypad->Get_Stick_Left(0).x * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		m_move.z -= pInputJoypad->Get_Stick_Left(0).x * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;
	}

	if (m_bInput)
	{
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= PLAYER_SPEED;

		//移動量を代入
		m_move = NormarizeMove;

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
		m_rot.y = atan2f(m_move.z, -m_move.x);
	}

	useful::NormalizeAngle(&m_rot);
}

//====================================================================
//攻撃処理
//====================================================================
void CPlayer::Attack(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	{
		CFire::Create("data\\model\\BlockTest.x", m_pos, m_move);
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
		if (m_Action != ACTION_SDEATH)
		{
			m_Action = ACTION_SDEATH;
			m_pMotion->Set(ACTION_SDEATH, 5);
		}
	}

	//移動モーション
	if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
	{
		if (m_Action != ACTION_BMOVE)
		{
			m_Action = ACTION_BMOVE;
			m_pMotion->Set(ACTION_BMOVE, 5);
		}
	}
	//卵モーション
	else if (m_State == STATE_EGG)
	{
		if (m_Action != ACTION_SWAIT)
		{
			m_Action = ACTION_SWAIT;
			m_pMotion->Set(ACTION_SWAIT, 5);
		}
	}
	//ニュートラルモーション
	else
	{
		if (m_Action != ACTION_BWAIT)
		{
			m_Action = ACTION_BWAIT;
			m_pMotion->Set(ACTION_BWAIT, 5);
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

	case STATE_DEATH:

		if (m_nStateCount == 0)
		{
			m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_State = STATE_EGG;
		}

		break;

	case STATE_EGG:
		//if (m_nStateCount == 0)
		//{
		//	m_State = STATE_WAIT;
		//}
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
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_CUBEBLOCK)
			{//種類がブロックの時

				CCubeBlock* pBlock = (CCubeBlock*)pObj;	// ブロック情報の取得

				D3DXVECTOR3 pos = pBlock->GetPos();
				D3DXVECTOR3 posOld = pBlock->GetPosOld();
				D3DXVECTOR3 Move = pBlock->GetMove();
				D3DXVECTOR3 Size = pBlock->GetSize();

				// 矩形の当たり判定
				if (useful::CollisionBlock(pos, posOld, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
				{
					//待機状態にする
					m_State = STATE_WAIT;
					m_MoveState = MOVE_STATE_WAIT;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// 壁との当たり判定
//====================================================================
void CPlayer::SearchWall(void)
{
	bool OKR = true;	//右
	bool OKL = true;	//左
	bool OKU = true;	//上
	bool OKD = true;	//下

	CMapSystem *pMapSystem = CMapSystem::GetInstance();
	int nMapWightMax = pMapSystem->GetWightMax();
	int nMapHeightMax = pMapSystem->GetHeightMax();
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

	int nRNumber = m_nMapWight + 1;
	int nLNumber = m_nMapWight - 1;
	int nUNumber = m_nMapHeight - 1;
	int nDNumber = m_nMapHeight + 1;

	nRNumber = useful::RangeNumber(nMapWightMax, 0, nRNumber);
	nLNumber = useful::RangeNumber(nMapWightMax, 0, nLNumber);
	nUNumber = useful::RangeNumber(nMapWightMax, 0, nUNumber);
	nDNumber = useful::RangeNumber(nMapWightMax, 0, nDNumber);

	OKR = !pMapSystem->GetGritBool(nRNumber, m_nMapHeight);
	OKL = !pMapSystem->GetGritBool(nLNumber, m_nMapHeight);
	OKU = !pMapSystem->GetGritBool(m_nMapWight, nUNumber);
	OKD = !pMapSystem->GetGritBool(m_nMapWight, nDNumber);

	//自分の立っているグリットの中心位置を求める
	D3DXVECTOR3 MyGritPos = INITVECTOR3;
	float MapGritSize = pMapSystem->GetGritSize();

	D3DXVECTOR3 DevilPos = CGame::GetDevil()->GetDevilPos();
	D3DXVECTOR3 DevilSize = CGame::GetDevil()->GetDevilSize();

	//ｘ座標のグリットに位置を設定する
	MyGritPos.x = MapSystemPos.x + (m_nMapWight * MapGritSize);

	// 自分の位置がマップ外だった場合に反対側からはみ出た分の位置を設定する
	if (MyGritPos.x > DevilPos.x + (DevilSize.x))
	{
		MyGritPos.x = MyGritPos.x - (DevilPos.x + (DevilSize.x * 2.0f)) - MapGritSize;
	}

	//ｚ座標のグリットに位置を設定する
	MyGritPos.z = MapSystemPos.z - (m_nMapHeight * MapGritSize);

	// 自分の位置がマップ外だった場合に反対側からはみ出た分の位置を設定する
	if (MyGritPos.z < DevilPos.z - (DevilSize.z))
	{
		MyGritPos.z = MyGritPos.z + (DevilPos.z + (DevilSize.z * 2.0f)) + MapGritSize;
	}

	DebugProc::Print(DebugProc::POINT_LEFT, "ああああ %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if (m_pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - m_size.x) &&
		m_pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - m_size.x) &&
		m_pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - m_size.z) &&
		m_pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - m_size.z))
	{// グリットの中心位置に立っているなら操作を受け付ける
		m_OKR = OKR;	//右
		m_OKL = OKL;	//左
		m_OKU = OKU;	//上
		m_OKD = OKD;	//下
	}
	else
	{
		m_OKR = false;	//右
		m_OKL = false;	//左
		m_OKU = false;	//上
		m_OKD = false;	//下
	}
}

//====================================================================
// マップモデルの当たり判定
//====================================================================
void CPlayer::CollisionDevilHole(useful::COLLISION XYZ)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_DEVILHOLE)
			{//種類がデビルホールの時

				CDevilHole* pDevilHole = (CDevilHole*)pObj;	// ブロック情報の取得

				D3DXVECTOR3 pos = pDevilHole->GetPos();
				D3DXVECTOR3 posOld = pDevilHole->GetPosOld();
				D3DXVECTOR3 Size = pDevilHole->GetSize();

				// 矩形の当たり判定
				if (useful::CollisionBlock(pos, posOld, INITVECTOR3, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
				{
					//待機状態にする
					m_State = STATE_WAIT;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// 敵の当たり判定
//====================================================================
void CPlayer::CollisionEnemy(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_ENEMY3D)
			{//種類が敵の時

				CEnemy* pEnemy = (CEnemy*)pObj;	// アイテムの情報の取得

				D3DXVECTOR3 pos = pEnemy->GetPos();
				D3DXVECTOR3 posOld = pEnemy->GetPosOld();
				D3DXVECTOR3 Size = pEnemy->GetSize();

				// 円の当たり判定
				if (useful::CollisionCircle(m_pos, pos, 30.0f) == true)
				{
					Death();
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// ステージ外の当たり判定
//====================================================================
void CPlayer::CollisionStageOut(void)
{
	D3DXVECTOR3 D_pos = CGame::GetDevil()->GetDevilPos();
	D3DXVECTOR3 D_Size = CGame::GetDevil()->GetDevilSize();
	float G_Size = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

	if (m_pos.x + G_Size > D_pos.x + D_Size.x)
	{
		m_pos.x = D_pos.x + D_Size.x - G_Size;
		m_State = STATE_WAIT;
		m_move.x = 0.0f;
	}
	if (m_pos.x - G_Size < D_pos.x - D_Size.x)
	{
		m_pos.x = D_pos.x - D_Size.x + G_Size;
		m_State = STATE_WAIT;
		m_move.x = 0.0f;
	}
	if (m_pos.z + G_Size > D_pos.z + D_Size.z)
	{
		m_pos.z = D_pos.z + D_Size.z - G_Size;
		m_State = STATE_WAIT;
		m_move.z = 0.0f;
	}
	if (m_pos.z - G_Size < D_pos.z - D_Size.z)
	{
		m_pos.z = D_pos.z - D_Size.z + G_Size;
		m_State = STATE_WAIT;
		m_move.z = 0.0f;
	}
}

//====================================================================
// プレイヤーがマップのどのマスに存在しているか設定する
//====================================================================
void CPlayer::MapSystemNumber(void)
{
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();
	D3DXVECTOR3 DevilSize = pDevil->GetDevilSize();
	D3DXVECTOR3 MapSystemPos = CMapSystem::GetInstance()->GetMapPos();
	int MapWightMax = CMapSystem::GetInstance()->GetWightMax();
	int MapHeightMax = CMapSystem::GetInstance()->GetHeightMax();
	float MapGritSize = CMapSystem::GetInstance()->GetGritSize();
	D3DXVECTOR3 GritPos = INITVECTOR3;

	for (int nCntW = 0; nCntW < MapWightMax; nCntW++)
	{
		float fCountPosX = MapSystemPos.x + (nCntW * MapGritSize);

		if (fCountPosX > DevilPos.x + (DevilSize.x))
		{
			fCountPosX = fCountPosX - (DevilPos.x + (DevilSize.x * 2.0f)) - MapGritSize;
		}

		if (m_pos.x < fCountPosX + (MapGritSize * 0.5f) &&
			m_pos.x >= fCountPosX - (MapGritSize * 0.5f))
		{
			m_nMapWight = nCntW;
			break;
		}
	}

	for (int nCntH = 0; nCntH < MapHeightMax; nCntH++)
	{
		float fCountPosZ = MapSystemPos.z - (nCntH * MapGritSize);

		if (fCountPosZ < DevilPos.z - (DevilSize.z))
		{
			fCountPosZ = fCountPosZ + (DevilPos.z + (DevilSize.z * 2.0f)) + MapGritSize;
		}

		if (m_pos.z < fCountPosZ + (MapGritSize * 0.5f) &&
			m_pos.z >= fCountPosZ - (MapGritSize * 0.5f))
		{
			m_nMapHeight = nCntH;
			break;
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

	//Y軸の位置更新
	m_pos.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_Y);
	CollisionDevilHole(useful::COLLISION_Y);

	//X軸の位置更新
	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_X);
	CollisionDevilHole(useful::COLLISION_X);

	//Z軸の位置更新
	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_Z);
	CollisionDevilHole(useful::COLLISION_Z);
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
//ダメージ処理
//====================================================================
void CPlayer::Death(void)
{
	if (m_State != STATE_EGG && m_State != STATE_DEATH)
	{
		m_nLife--;

		if (m_nLife < 0)
		{
			CGame::SetGameEnd(true);
			CGame::SetGameClear(false);
		}
		else
		{
			if (m_pLifeUi != nullptr)
			{
				m_pLifeUi->GetNumber()->SetNumber(m_nLife);
			}

			m_State = STATE_DEATH;
			m_nStateCount = 150;

			// ダメージ音
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE_PLAYER);
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

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_CUBEBLOCK)
			{//種類がブロックの時
				CCubeBlock* pBlock = (CCubeBlock*)pObj;

				if (pBlock->GetPos().y <= 200.0f)
				{
					if (useful::CollisionCircle(m_pos, pBlock->GetPos(), 250.0f) == true)
					{
						D3DXVECTOR3 CrossPos = INITVECTOR3;
						float ObjDistance = 10000.0f;

						//交点の位置を求める
						for (int nCnt = 0; nCnt < 4; nCnt++)
						{
							switch (nCnt)
							{
							case 0:
								CrossPos = useful::CrossIntersection(m_pos, pos,
									D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
									D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
									250.0f);
								break;

							case 1:
								CrossPos = useful::CrossIntersection(m_pos, pos,
									D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
									D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
									250.0f);
								break;

							case 2:
								CrossPos = useful::CrossIntersection(m_pos, pos,
									D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
									D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
									250.0f);
								break;

							case 3:
								CrossPos = useful::CrossIntersection(m_pos, pos,
									D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
									D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
									250.0f);
								break;
							}

							ObjDistance = sqrtf((m_pos.x - CrossPos.x) * (m_pos.x - CrossPos.x) + (m_pos.z - CrossPos.z) * (m_pos.z - CrossPos.z));

							//CManager::GetInstance()->GetDebugProc()->Print("[敵：%f %f] [プ：%f %f] [壁：%f %f] \n", m_pos.x, m_pos.z, pos.x, pos.z, CrossPos.x, CrossPos.z);
							//CManager::GetInstance()->GetDebugProc()->Print("%f   :   %f\n", fDistance, ObjDistance);

							if (ObjDistance < fDistance)
							{
								return true;
							}
						}
					}
				}
			}

			pObj = pObjNext;
		}
	}
	return false;
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
