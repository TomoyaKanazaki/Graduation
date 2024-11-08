//============================================
//
//	デビルの処理(仮) [devil.cpp]
//	Author:sakamoto kai
//
//============================================
#include "devil.h"
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
#include "bowabowa.h"
#include "Cross.h"
#include "MapSystem.h"
#include "debugproc.h"
#include "objmeshField.h"
#include "railblock.h"
#include "sound.h"

//===========================================
// 定数定義
//===========================================
namespace
{
	float SCROOL_SPEED = 1.5f;					// スクロールの移動速度
	float STAGE_ROT_LIMIT = D3DX_PI * 0.25f;	// スクロールの移動速度
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CDevil>* CDevil::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CDevil::CDevil(int nPriority) : CObject(nPriority)
{
	SetSize(D3DXVECTOR3(750.0f, 0.0f, 550.0f));
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_fActionCount = 0.0f;
	m_Action = ACTION_NEUTRAL;
	m_AtkAction = ACTION_NEUTRAL;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_CollisionRot = 0.0f;
	m_pMotion = nullptr;
	m_DevilPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MapDifference = INITVECTOR3;
	m_DevilRot = INITVECTOR3;
}

//====================================================================
//デストラクタ
//====================================================================
CDevil::~CDevil()
{

}

//====================================================================
//生成処理
//====================================================================
CDevil* CDevil::Create()
{
	CDevil* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//プレイヤーの生成
		pPlayer = new CDevil();
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
HRESULT CDevil::Init(void)
{
	//種類設定
	SetType(CObject::TYPE_DEVIL);

	//モデルの生成
	LoadLevelData("data\\TXT\\MOTION\\01_enemy\\motion_devil.txt");

	//モーションの生成
	if (m_pMotion == nullptr)
	{
		//モーションの生成
		m_pMotion = new CMotion;
	}

	//初期化処理
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData("data\\TXT\\MOTION\\01_enemy\\motion_devil.txt");

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

	// スローの生成
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_PLAYER, CSlowManager::TAG_PLAYER);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CDevil>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CDevil::Uninit(void)
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
void CDevil::Update(void)
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
void CDevil::TitleUpdate(void)
{
	//モーションの更新
	m_pMotion->Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CDevil::GameUpdate(void)
{
	// TODO : ランダム移動やめる
#ifndef _DEBUG
		// カウンターのリセット
		if (m_fActionCount >= 5.0f)
		{
			ACTION_TYPE action = m_Action;

			do
			{
				m_Action = (ACTION_TYPE)(rand() % ACTION_MAX);
			} while (m_Action == action);

			m_fActionCount = 0.0f;
		}

		// カウンターの加算
		m_fActionCount += DeltaTime::Get();
		DebugProc::Print(DebugProc::POINT_CENTER, "移動カウンター : %f\n", m_fActionCount);

		// 移動
		switch (m_Action)
		{
		case ACTION_NEUTRAL:
			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			DebugProc::Print(DebugProc::POINT_CENTER, "待機\n");
			break;
		case ACTION_SIGNAL_UP:
			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, SCROOL_SPEED));
			DebugProc::Print(DebugProc::POINT_CENTER, "上\n");
			break;
		case ACTION_SIGNAL_DOWN:
			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, -SCROOL_SPEED));
			DebugProc::Print(DebugProc::POINT_CENTER, "下\n");
			break;
		case ACTION_SIGNAL_LEFT:
			ObjectScroll(D3DXVECTOR3(-SCROOL_SPEED, 0.0f, 0.0f));
			DebugProc::Print(DebugProc::POINT_CENTER, "左\n");
			break;
		case ACTION_SIGNAL_RIGHT:
			ObjectScroll(D3DXVECTOR3(SCROOL_SPEED, 0.0f, 0.0f));
			DebugProc::Print(DebugProc::POINT_CENTER, "右\n");
			break;
		default:
			assert(false);
			break;
		}
#endif // !_DEBUG

	// 過去の位置に代入
	m_posOld = m_pos;

	// 移動処理
	Move();

	// 向き移動処理
	Rot();

	// マップの傾き
	m_DevilRot = CGame::GetMapField()->GetRot();

	//状態の管理
	StateManager();

	if (m_pMotion != nullptr)
	{
		//モーションの更新
		m_pMotion->Update();
	}

	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	CEffect* pTestEffect = nullptr;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pTestEffect = CEffect::Create();

		switch (nCnt)
		{
		case 0:
			pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x + MapSize.x, m_DevilPos.y, m_DevilPos.z + MapSize.z));
			break;
		case 1:
			pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x - MapSize.x, m_DevilPos.y, m_DevilPos.z + MapSize.z));
			break;
		case 2:
			pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x + MapSize.x, m_DevilPos.y, m_DevilPos.z - MapSize.z));
			break;
		case 3:
			pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x - MapSize.x, m_DevilPos.y, m_DevilPos.z - MapSize.z));
			break;
		}

		pTestEffect->SetLife(20);
		pTestEffect->SetRadius(20.0f);
		pTestEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	//モーションの管理
	ActionState();

	//デバッグキーの処理と設定
	DebugKey();

	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetInitPos();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

	//デバッグ表示
	DebugProc::Print(DebugProc::POINT_LEFT, "[マップ]　　　位置 %f : %f\n", MapPos.x, MapPos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[マップの差分]位置 %f : %f\n", m_MapDifference.x, m_MapDifference.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[マップのスクロール]：矢印キー\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[マップの傾きX]：[ X+ : 6 ] [ X- : 7 ]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[マップの傾きZ]：[ Z+ : 8 ] [ Z- : 9 ]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[マップの傾きリセット]：[ 5 ]\n");
}

//====================================================================
//チュートリアルでの更新処理
//====================================================================
void CDevil::TutorialUpdate(void)
{

}

//====================================================================
//描画処理
//====================================================================
void CDevil::Draw(void)
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
void CDevil::Move(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//キーボードの移動処理
	if (pInputKeyboard->GetPress(DIK_UP))
	{
		ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, SCROOL_SPEED));

	}
	if (pInputKeyboard->GetPress(DIK_DOWN))
	{
		ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, -SCROOL_SPEED));
	}

	if (pInputKeyboard->GetPress(DIK_LEFT))
	{
		ObjectScroll(D3DXVECTOR3(-SCROOL_SPEED, 0.0f, 0.0f));
	}
	if (pInputKeyboard->GetPress(DIK_RIGHT))
	{
		ObjectScroll(D3DXVECTOR3(SCROOL_SPEED, 0.0f, 0.0f));
	}

	if (pInputKeyboard->GetPress(DIK_UP) == false && pInputKeyboard->GetPress(DIK_LEFT) == false && pInputKeyboard->GetPress(DIK_DOWN) == false && pInputKeyboard->GetPress(DIK_RIGHT) == false)
	{
		//左スティックによる前後移動	
		m_DevilPos.z += pInputJoypad->Get_Stick_Left(0).y * SCROOL_SPEED;
		m_DevilPos.x += pInputJoypad->Get_Stick_Left(0).y * SCROOL_SPEED;

		//左スティックによる左右移動
		m_DevilPos.x += pInputJoypad->Get_Stick_Left(0).x * SCROOL_SPEED;
		m_DevilPos.z -= pInputJoypad->Get_Stick_Left(0).x * SCROOL_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_LEFT) == true || pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= SCROOL_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= SCROOL_SPEED;
	}

	m_move += NormarizeMove;

}

//====================================================================
//移動方向処理
//====================================================================
void CDevil::Rot(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
	
	CObjmeshField *pMapField = CGame::GetMapField();
	D3DXVECTOR3 MapRot = pMapField->GetRot();

	if (pInputKeyboard->GetPress(DIK_5))
	{
		MapRot = INITVECTOR3;
	}

	if (pInputKeyboard->GetPress(DIK_6))
	{
		MapRot.x += D3DX_PI * 0.005f;

		if (MapRot.x > STAGE_ROT_LIMIT)
		{
			MapRot.x = STAGE_ROT_LIMIT;
		}
	}
	if (pInputKeyboard->GetPress(DIK_7))
	{
		MapRot.x -= D3DX_PI * 0.005f;

		if (MapRot.x < -STAGE_ROT_LIMIT)
		{
			MapRot.x = -STAGE_ROT_LIMIT;
		}
	}

	if (pInputKeyboard->GetPress(DIK_8))
	{
		MapRot.z += D3DX_PI * 0.005f;

		if (MapRot.z > STAGE_ROT_LIMIT)
		{
			MapRot.z = STAGE_ROT_LIMIT;
		}
	}
	if (pInputKeyboard->GetPress(DIK_9))
	{
		MapRot.z -= D3DX_PI * 0.005f;

		if (MapRot.z < -STAGE_ROT_LIMIT)
		{
			MapRot.z = -STAGE_ROT_LIMIT;
		}
	}

	pMapField->SetRot(MapRot);
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CDevil::ActionState(void)
{
	// 移動モーション
	//if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
	//{
	//	if (m_Action != ACTION_SIGNAL_UP)
	//	{
	//		m_Action = ACTION_SIGNAL_UP;
	//		m_pMotion->Set(ACTION_SIGNAL_UP, 5);
	//	}
	//}
	//// ニュートラルモーション
	//else
	//{
	//	if (m_Action != ACTION_NEUTRAL)
	//	{
	//		m_Action = ACTION_NEUTRAL;
	//		m_pMotion->Set(ACTION_NEUTRAL, 5);
	//	}
	//}
}

//====================================================================
//状態管理
//====================================================================
void CDevil::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		//	スローをdefaultへ
		CSlowManager::SetValueDefault();
		break;

	case STATE_DEATH:
		break;

	case STATE_WAIT:
		break;

	case STATE_DAMAGE:
		if (m_nStateCount == 0)
		{
			m_State = STATE_NORMAL;
		}
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// デバッグボタン
//====================================================================
void CDevil::DebugKey(void)
{
#ifdef _DEBUG

	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse->GetTrigger(pMouse->PUSH_WHEEL))
	{
		HitDamage(10.0f);
	}

#endif // !_DEBUG
}

//====================================================================
// モデルのモーション設定
//====================================================================
void CDevil::SetAction(ACTION_TYPE Action, float BlendTime)
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
void CDevil::SetModelDisp(bool Sst)
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
// オブジェクトのスクロール
//====================================================================
void CDevil::ObjectScroll(D3DXVECTOR3 Move)
{
	// グリッドのスクロール
	GritScroll(Move);

	// グリッドのサイズ
	float m_GritSize = CMapSystem::GetInstance()->GetGritSize();

	// 十字架のスクロール
	CrossScroll(Move, m_GritSize);

	// ボワボワのスクロール
	BowabowaScroll(Move, m_GritSize);

	// 敵のスクロール
	EnemyScroll(Move, m_GritSize);

	// レールブロックのスクロール
	RailBlockScroll(Move, m_GritSize);

	// プレイヤーのスクロール
	PlayerScroll(Move, m_GritSize);
}

//====================================================================
// 十字架のスクロール
//====================================================================
void CDevil::CrossScroll(D3DXVECTOR3 Move, float GritSize)
{
	// 十字架のリスト構造が無ければ抜ける
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // リストを取得

	// 十字架のリストの中身を確認する
	for (CCross* pCross : list)
	{
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		D3DXVECTOR3 pos = pCross->GetPos();
		D3DXVECTOR3 Size = pCross->GetSize();

		pos += Move;

		if (Move.x > 0.0f)
		{
			if (m_DevilPos.x + MapSize.x < pos.x - GritSize)
			{
				pos.x = -MapSize.x + m_DevilPos.x - GritSize + Move.x;
			}
		}
		if (Move.x < 0.0f)
		{
			if (m_DevilPos.x - MapSize.x > pos.x + GritSize)
			{
				pos.x = MapSize.x + m_DevilPos.x + GritSize + Move.x;
			}
		}
		if (Move.z > 0.0f)
		{
			if (m_DevilPos.z + MapSize.z < pos.z - GritSize)
			{
				pos.z = -MapSize.z + m_DevilPos.z - GritSize + Move.z;
			}
		}
		if (Move.z < 0.0f)
		{
			if (m_DevilPos.z - MapSize.z > pos.z + GritSize)
			{
				pos.z = MapSize.z + m_DevilPos.z + GritSize + Move.z;
			}
		}

		// 位置設定
		pCross->SetPos(pos);
	}
}

//====================================================================
// ボワボワのスクロール
//====================================================================
void CDevil::BowabowaScroll(D3DXVECTOR3 Move, float GritSize)
{
	// ボワボワのリスト構造が無ければ抜ける
	if (CBowabowa::GetList() == nullptr) { return; }
	std::list<CBowabowa*> list = CBowabowa::GetList()->GetList();    // リストを取得

	// ボワボワのリストの中身を確認する
	for (CBowabowa* pBowabowa : list)
	{
		D3DXVECTOR3 pos = pBowabowa->GetPos();
		D3DXVECTOR3 Size = pBowabowa->GetSize();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();

		pos += Move;

		if (Move.x > 0.0f)
		{
			if (MapSize.x < pos.x)
			{
				pos.x = -MapSize.x + Move.x - 100.0f;
			}
		}
		if (Move.x < 0.0f)
		{
			if (-MapSize.x > pos.x)
			{
				pos.x = MapSize.x + Move.x + 100.0f;
			}
		}
		if (Move.z > 0.0f)
		{
			if (MapSize.z < pos.z)
			{
				pos.z = -MapSize.z + Move.z - 100.0f;
			}
		}
		if (Move.z < 0.0f)
		{
			if (-MapSize.z > pos.z)
			{
				pos.z = MapSize.z + Move.z + 100.0f;
			}
		}

		pBowabowa->SetPos(pos);
	}
}

//====================================================================
// 敵のスクロール
//====================================================================
void CDevil::EnemyScroll(D3DXVECTOR3 Move, float GritSize)
{
	// 敵のリスト構造が無ければ抜ける
	if (CEnemy::GetList() == nullptr) { return; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // リストを取得

	// 敵のリストの中身を確認する
	for (CEnemy* pEnemy : list)
	{
		D3DXVECTOR3 pos = pEnemy->GetPos();
		D3DXVECTOR3 Size = pEnemy->GetSize();

		pos += Move;

		pEnemy->SetPos(pos);
	}
}

//====================================================================
// レールブロックのスクロール
//====================================================================
void CDevil::RailBlockScroll(D3DXVECTOR3 Move, float GritSize)
{
	// レールブロックのリスト構造が無ければ抜ける
	if (CRailBlock::GetList() == nullptr) { return; }
	std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // リストを取得

	// レールブロックのリストの中身を確認する
	for (CRailBlock* pRailBlock : list)
	{
		D3DXVECTOR3 pos = pRailBlock->GetPos();
		pos += Move;
		pRailBlock->SetPos(pos);
	}
}

//====================================================================
// プレイヤーのスクロール
//====================================================================
void CDevil::PlayerScroll(D3DXVECTOR3 Move, float GritSize)
{
	// プレイヤーのリスト構造が無ければ抜ける
	if (CPlayer::GetList() == nullptr) { return; }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();    // リストを取得

	// プレイヤーのリストの中身を確認する
	for (CPlayer* pPlayer : list)
	{
		if (pPlayer->GetState() != CPlayer::STATE_EGG)
		{
			D3DXVECTOR3 pos = pPlayer->GetPos();
			D3DXVECTOR3 Size = pPlayer->GetSize();
			D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();

			pos += Move;

			if (Move.x > 0.0f)
			{
				if (pos.x + (GritSize * 0.5f) > m_DevilPos.x + MapSize.x)
				{
					pos.x = m_DevilPos.x + MapSize.x - (GritSize * 0.5f) + Move.x;
					CollisionPressPlayer(pPlayer, pos, Size);

					CEffect* pEffect = CEffect::Create();
					pEffect->SetPos(pos);
					pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
					pEffect->SetRadius(100.0f);
				}
			}
			if (Move.x < 0.0f)
			{
				if (pos.x - (GritSize * 0.5f) < m_DevilPos.x - MapSize.x)
				{
					pos.x = m_DevilPos.x - MapSize.x + (GritSize * 0.5f) + Move.x;
					CollisionPressPlayer(pPlayer, pos, Size);

					CEffect* pEffect = CEffect::Create();
					pEffect->SetPos(pos);
					pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
					pEffect->SetRadius(100.0f);
				}
			}
			if (Move.z > 0.0f)
			{
				if (pos.z + (GritSize * 0.5f) > m_DevilPos.z + MapSize.z)
				{
					pos.z = m_DevilPos.z + MapSize.z - (GritSize * 0.5f) + Move.x;
					CollisionPressPlayer(pPlayer, pos, Size);

					CEffect* pEffect = CEffect::Create();
					pEffect->SetPos(pos);
					pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
					pEffect->SetRadius(100.0f);
				}
			}
			if (Move.z < 0.0f)
			{
				if (pos.z - (GritSize * 0.5f) < m_DevilPos.z - MapSize.z)
				{
					pos.z = m_DevilPos.z - MapSize.z + (GritSize * 0.5f) + Move.z;
					CollisionPressPlayer(pPlayer, pos, Size);

					CEffect* pEffect = CEffect::Create();
					pEffect->SetPos(pos);
					pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
					pEffect->SetRadius(100.0f);
				}
			}

			pPlayer->SetPos(pos);
		}
		else
		{// 卵状態の時、ブロックが存在しない位置にホーミングする

			CMapSystem::GRID Grit = pPlayer->GetGrid();;
			if (!CMapSystem::GetInstance()->GetGritBool(Grit.x, Grit.z))
			{
				D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
				D3DXVECTOR3 AnswerPos = INITVECTOR3;
				AnswerPos = CMapSystem::GetInstance()->GetGritPos(Grit.x, Grit.z);

				if (pPlayer->GetGritCenter())
				{
					pPlayer->SetPos(AnswerPos);
				}
				else
				{
					PlayerPos += (AnswerPos - PlayerPos) * 0.5f;
					pPlayer->SetPos(PlayerPos);
				}
			}
		}
	}
}

//====================================================================
// グリットのスクロール
//====================================================================
void CDevil::GritScroll(D3DXVECTOR3 Move)
{
	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetInitPos();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();
	int MapWightMax = CMapSystem::GetInstance()->GetWightMax();
	int MapHeightMax = CMapSystem::GetInstance()->GetHeightMax();
	float MapGrit = CMapSystem::GetInstance()->GetGritSize();
	MapPos += Move;
	m_MapDifference = -(InitPos - MapPos);
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();

	if ((InitPos.x - MapPos.x) > 0.0f)
	{// 左範囲
		MapPos.x = InitPos.x + (MapSize.x * 2.0f) + MapGrit + Move.x;
	}
	if ((InitPos.x - MapPos.x + MapGrit) < (-MapSize.x * 2.0f))
	{// 右範囲
		MapPos.x = InitPos.x + Move.x;
	}

	if ((InitPos.z - MapPos.z) < 0.0f)
	{// 上範囲
		MapPos.z = InitPos.z + (-MapSize.z * 2.0f) - MapGrit + Move.z;
	}
	if ((InitPos.z - MapPos.z - MapGrit) > (MapSize.z * 2.0f))
	{// 下範囲
		MapPos.z = InitPos.z + Move.z;
	}

	CMapSystem::GetInstance()->SetMapPos(MapPos);

	// キューブブロックのリスト構造が無ければ抜ける
	if (CCubeBlock::GetList() == nullptr) { return; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // リストを取得

	// キューブブロックのリストの中身を確認する
	for (CCubeBlock* pCubeBlock : list)
	{
		// 縦横のナンバーと高さを設定する
		D3DXVECTOR3 pos = INITVECTOR3;
		int BlockWight = pCubeBlock->GetWightNumber();
		int BlockHeight = pCubeBlock->GetHeightNumber();

		//グリット番号を位置に変換
		pos = CMapSystem::GetInstance()->GetGritPos(BlockWight, BlockHeight);
		pos.y = 50.0f;

		pCubeBlock->SetPos(pos);
	}

#ifdef _DEBUG

	////　グリットの位置にエフェクトを表示
	//for (int nCntW = 0; nCntW < MapWightMax; nCntW++)
	//{
	//	for (int nCntH = 0; nCntH < MapHeightMax; nCntH++)
	//	{
	//		//グリット番号を位置に変換
	//		D3DXVECTOR3 CountPos = CMapSystem::GetInstance()->GetGritPos(nCntW, nCntH);
	//		CountPos.y = 50.0f;

	//		if (CMapSystem::GetInstance()->GetGritBool(nCntW, nCntH))
	//		{// ブロックが存在するグリットのみエフェクトを表示

	//			CEffect* pEffect = CEffect::Create();
	//			pEffect->SetPos(CountPos);
	//			pEffect->SetLife(10);
	//		}
	//	}
	//}

#endif // _DEBUG
}

//====================================================================
// プレイヤーが潰される時の処理
//====================================================================
void CDevil::CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size)
{
	// キューブブロックのリスト構造が無ければ抜ける
	if (CCubeBlock::GetList() == nullptr) { return; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // リストを取得

	// キューブブロックのリストの中身を確認する
	for (CCubeBlock* pCubeBlock : list)
	{
		D3DXVECTOR3 Blockpos = pCubeBlock->GetPos();
		D3DXVECTOR3 BlockSize = pCubeBlock->GetSize();

		if (useful::CollisionRectangle2D(pos, Blockpos, Size, BlockSize, useful::COLLISION::COLLISION_ZX))
		{
			pPlayer->Death();
			return;
		}
	}
}

//====================================================================
//傾き中の移動量変動
//====================================================================
float CDevil::MoveSlopeX(void)
{
	float fSlopeMove = 1.0f;

	D3DXVECTOR3 DevilRot = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{
		DevilRot = m_DevilRot;
	}

	if (m_move.x > 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.z));
	}
	else if (m_move.x < 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.z));
	}

	return fSlopeMove;
}

//====================================================================
//傾き中の移動量変動
//====================================================================
float CDevil::MoveSlopeZ(void)
{
	float fSlopeMove = 1.0f;

	D3DXVECTOR3 DevilRot = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{
		DevilRot = CGame::GetDevil()->GetDevilRot();
	}

	if (m_move.z > 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.x));
	}
	else if (m_move.z < 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.x));
	}

	return fSlopeMove;
}

//====================================================================
//ロード処理
//====================================================================
void CDevil::LoadLevelData(const char* pFilename)
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
								fscanf(pFile, "%f", &ModelRot.x);				//向きの初期設定
								fscanf(pFile, "%f", &ModelRot.y);				//向きの初期設定
								fscanf(pFile, "%f", &ModelRot.z);				//向きの初期設定

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
//リスト取得
//====================================================================
CListManager<CDevil>* CDevil::GetList(void)
{
	return m_pList;
}
