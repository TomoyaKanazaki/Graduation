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
#include "objectcharacter.h"
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
#include "bowabowa.h"
#include "Cross.h"
#include "objmeshField.h"
#include "railblock.h"
#include "RollRock.h"
#include "sound.h"
#include "scrollarrow.h"
#include "fire.h"
#include "Scene.h"
#include "tile.h"
#include "wall.h"
#include "MapSystem.h"
#include "ScrollDevice.h"
#include "SlopeDevice.h"
#include "effect.h"
#include "signal.h"

//===========================================
// 定数定義
//===========================================
namespace
{
	int SCROOL_TIME = 300;						// スクロール時間

	int SCROOL_MOVEGRID_01 = 3;					// スクロールの移動マス幅
	float SCROOL_SPEED_01 = (CMapSystem::GetGritSize() * SCROOL_MOVEGRID_01 / SCROOL_TIME);				// スクロールの移動速度

	int SCROOL_COUNT_02 = 12;					// スクロールの移動回数
	int SCROOL_MOVEGRID_02 = 3;					// スクロールの移動マス幅
	float SCROOL_SPEED_02 = (CMapSystem::GetGritSize() * SCROOL_MOVEGRID_02) / SCROOL_COUNT_02;				// スクロールの移動速度

	int SLOPE_TIME = 300;						// 傾き操作時間
	int SLOPE_RAND = 25;						// 傾き発生確率
	float STAGE_ROT_LIMIT = D3DX_PI * 0.15f;	// 傾きの角度制限

	float SLOPE_SPEED01 = 0.00075f;				// 傾きの移動速度

	float SLOPE_SPEED02 = 0.0125f;				// 傾きの移動速度
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CDevil>* CDevil::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CDevil::CDevil(int nPriority) : CObjectCharacter(nPriority)
{
	SetSize(D3DXVECTOR3(750.0f, 0.0f, 550.0f));
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_fActionCount = 0.0f;
	m_Action = ACTION_NEUTRAL;
	m_AtkAction = ACTION_NEUTRAL;
	m_State = STATE_WAIT;
	m_nStateCount = 0;
	m_CollisionRot = 0.0f;
	m_DevilPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MapDifference = INITVECTOR3;
	m_DevilRot = INITVECTOR3;
	m_bSlope = false;
	m_MinGrid = CMapSystem::GRID(0, 0);
	m_MaxGrid = CMapSystem::GRID(NUM_WIGHT - 1, NUM_HEIGHT - 1);
	m_DevilArrow = 0;
	m_ScrollArrowOld = 0;
	m_SlopwArrowOld = 0;
	m_ScrollArrow[0] = nullptr;
	m_ScrollArrow[1] = nullptr;
	m_ScrollType = SCROLL_TYPE_NORMAL;
	m_pSignal[0] = nullptr;
	m_pSignal[1] = nullptr;
	m_nStateNum = 0;
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

	// 影を不使用に設定
	SetShadow(false);

	// キャラクターテキストの設定処理
	CObjectCharacter::Init("data\\TXT\\MOTION\\01_enemy\\motion_devil.txt");
	
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

	if (m_ScrollArrow[0] == nullptr)
	{
		m_ScrollArrow[0] = CScrollArrow::Create();
		m_ScrollArrow[0]->SetPos((D3DXVECTOR3(80.0f, 120.0f, 0.0f)));
	}

	if (m_ScrollArrow[1] == nullptr)
	{
		m_ScrollArrow[1] = CScrollArrow::Create();
		m_ScrollArrow[1]->SetPos((D3DXVECTOR3(1200.0f, 120.0f, 0.0f)));
	}

	// 矢印生成
	if (m_pSignal[0] == nullptr)
	{
		m_pSignal[0] = CSignal::Create("data\\MODEL\\signal.x");
		m_pSignal[0]->SetPos(D3DXVECTOR3(-1000.0f,200.0f,500.0f));
	}

	if (m_pSignal[1] == nullptr)
	{
		m_pSignal[1] = CSignal::Create("data\\MODEL\\signal.x");
		m_pSignal[1]->SetPos(D3DXVECTOR3(1000.0f, 200.0f, 500.0f));
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

	// キャラクタークラスの終了（継承）
	CObjectCharacter::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CDevil::Update(void)
{
	// 値を取得
	D3DXVECTOR3 posMy = GetPos();			// 位置
	D3DXVECTOR3 posOldMy = GetPosOld();		// 前回の位置
	D3DXVECTOR3 rotMy = GetRot();			// 向き
	D3DXVECTOR3 sizeMy = GetSize();			// 大きさ

	// 過去の位置に代入
	posOldMy = posMy;

	// マップの傾き
	m_DevilRot = CObjmeshField::GetListTop()->GetRot();

	// 矢印の回転
	SignalScroll();

	//状態の管理
	StateManager();

	//デバッグキーの処理と設定
	DebugKey();

	//ステージ外にいるオブジェクトの処理
	CollisionOut();

	// キャラクタークラスの更新（継承）
	CObjectCharacter::Update();

	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();

	//for (int nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	pTestEffect = CEffect::Create();
	//
	//	switch (nCnt)
	//	{
	//	case 0:
	//		pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x + MapSize.x, m_DevilPos.y, m_DevilPos.z + MapSize.z));
	//		break;
	//	case 1:
	//		pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x - MapSize.x, m_DevilPos.y, m_DevilPos.z + MapSize.z));
	//		break;
	//	case 2:
	//		pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x + MapSize.x, m_DevilPos.y, m_DevilPos.z - MapSize.z));
	//		break;
	//	case 3:
	//		pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x - MapSize.x, m_DevilPos.y, m_DevilPos.z - MapSize.z));
	//		break;
	//	}
	//
	//	pTestEffect->SetLife(20);
	//	pTestEffect->SetRadius(20.0f);
	//	pTestEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//}

	//モーションの管理
	ActionState();

	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetInitPos();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

	//デバッグ表示
	DebugProc::Print(DebugProc::POINT_LEFT, "[マップ]　　　位置 %f : %f\n", MapPos.x, MapPos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[マップの差分]位置 %f : %f\n", m_MapDifference.x, m_MapDifference.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[マップのスクロール]：矢印キー\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[マップの傾きX]：[ X+ : 6 ] [ X- : 7 ]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[マップの傾きZ]：[ Z+ : 8 ] [ Z- : 9 ]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[マップの傾きリセット]：[ 5 ]\n");

	DebugProc::Print(DebugProc::POINT_RIGHT, "[最小番号]左 %d : 上 %d\n", m_MinGrid.x, m_MinGrid.z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "[最大番号]右 %d : 下 %d\n", m_MaxGrid.x, m_MaxGrid.z);

	// 値更新
	SetPos(posMy);			// 位置
	SetPosOld(posOldMy);	// 前回の位置
	SetRot(rotMy);			// 向き
	SetSize(sizeMy);		// 大きさ
}

//====================================================================
//描画処理
//====================================================================
void CDevil::Draw(void)
{
	// キャラクタークラスの描画（継承）
	CObjectCharacter::Draw();
}

//====================================================================
//移動処理
//====================================================================
void CDevil::Move(int Arroow)
{
	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	switch (m_ScrollType)
	{
	case CDevil::SCROLL_TYPE_NORMAL:

		switch (Arroow)
		{
		case 0:

			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, SCROOL_SPEED_01));
			m_move.z = SCROOL_SPEED_01;

			break;
		case 1:

			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, -SCROOL_SPEED_01));
			m_move.z = -SCROOL_SPEED_01;

			break;
		case 2:

			ObjectScroll(D3DXVECTOR3(-SCROOL_SPEED_01, 0.0f, 0.0f));
			m_move.x = -SCROOL_SPEED_01;

			break;
		case 3:

			ObjectScroll(D3DXVECTOR3(SCROOL_SPEED_01, 0.0f, 0.0f));
			m_move.x = SCROOL_SPEED_01;

			break;
		}

		break;

	case CDevil::SCROLL_TYPE_RETRO:

		switch (Arroow)
		{
		case 0:

			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, SCROOL_SPEED_02));
			m_move.z = SCROOL_SPEED_02;

			break;
		case 1:

			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, -SCROOL_SPEED_02));
			m_move.z = -SCROOL_SPEED_02;

			break;
		case 2:

			ObjectScroll(D3DXVECTOR3(-SCROOL_SPEED_02, 0.0f, 0.0f));
			m_move.x = -SCROOL_SPEED_02;

			break;
		case 3:

			ObjectScroll(D3DXVECTOR3(SCROOL_SPEED_02, 0.0f, 0.0f));
			m_move.x = SCROOL_SPEED_02;

			break;
		}

		break;

	default:

		switch (Arroow)
		{
		case 0:

			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, SCROOL_SPEED_01));
			m_move.z = SCROOL_SPEED_01;

			break;
		case 1:

			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, -SCROOL_SPEED_01));
			m_move.z = -SCROOL_SPEED_01;

			break;
		case 2:

			ObjectScroll(D3DXVECTOR3(-SCROOL_SPEED_01, 0.0f, 0.0f));
			m_move.x = -SCROOL_SPEED_01;

			break;
		case 3:

			ObjectScroll(D3DXVECTOR3(SCROOL_SPEED_01, 0.0f, 0.0f));
			m_move.x = SCROOL_SPEED_01;

			break;
		}

		break;
	}
}

//====================================================================
//移動方向処理
//====================================================================
void CDevil::BackSlope(void)
{
	CObjmeshField* pMapField = CObjmeshField::GetListTop();
	D3DXVECTOR3 MapRot = pMapField->GetRot();
	bool bBackOK = false;

	switch (m_ScrollType)
	{
	case CDevil::SCROLL_TYPE_NORMAL:

		if (MapRot.x > 0.0f)
		{
			MapRot.x -= D3DX_PI * SLOPE_SPEED01;
		}

		if (MapRot.x < 0.0f)
		{
			MapRot.x += D3DX_PI * SLOPE_SPEED01;
		}

		if (MapRot.z > 0.0f)
		{
			MapRot.z -= D3DX_PI * SLOPE_SPEED01;
		}

		if (MapRot.z < 0.0f)
		{
			MapRot.z += D3DX_PI * SLOPE_SPEED01;
		}

		break;

	case CDevil::SCROLL_TYPE_RETRO:

		if (MapRot.x > 0.0f)
		{
			MapRot.x -= D3DX_PI * SLOPE_SPEED02;

			if (MapRot.x <= 0.0f)
			{
				bBackOK = true;
			}
		}

		if (MapRot.x < 0.0f)
		{
			MapRot.x += D3DX_PI * SLOPE_SPEED02;

			if (MapRot.x >= 0.0f)
			{
				bBackOK = true;
			}
		}

		if (MapRot.z > 0.0f)
		{
			MapRot.z -= D3DX_PI * SLOPE_SPEED02;

			if (MapRot.z <= 0.0f)
			{
				bBackOK = true;
			}
		}

		if (MapRot.z < 0.0f)
		{
			MapRot.z += D3DX_PI * SLOPE_SPEED02;

			if (MapRot.z >= 0.0f)
			{
				bBackOK = true;
			}
		}

		break;

	default:

		if (MapRot.x > 0.0f)
		{
			MapRot.x -= D3DX_PI * SLOPE_SPEED01;
		}

		if (MapRot.x < 0.0f)
		{
			MapRot.x += D3DX_PI * SLOPE_SPEED01;
		}

		if (MapRot.z > 0.0f)
		{
			MapRot.z -= D3DX_PI * SLOPE_SPEED01;
		}

		if (MapRot.z < 0.0f)
		{
			MapRot.z += D3DX_PI * SLOPE_SPEED01;
		}

		break;
	}

	if (bBackOK)
	{
		STATE_WAIT;
		m_nStateCount = 120;
		MapRot.x = 0.0f;
		MapRot.z = 0.0f;
	}

	pMapField->SetRot(MapRot);
}

//====================================================================
//移動方向処理
//====================================================================
void CDevil::Slope(int Arroow)
{	
	CObjmeshField *pMapField = CObjmeshField::GetListTop();
	if (pMapField == nullptr) { assert(false); }

	D3DXVECTOR3 MapRot = pMapField->GetRot();

	switch (m_ScrollType)
	{
	case CDevil::SCROLL_TYPE_NORMAL:

		switch (Arroow)
		{
		case 0:

			MapRot.x += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x > STAGE_ROT_LIMIT)
			{
				MapRot.x = STAGE_ROT_LIMIT;
			}

			break;
		case 1:

			MapRot.x -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x < -STAGE_ROT_LIMIT)
			{
				MapRot.x = -STAGE_ROT_LIMIT;
			}

			break;
		case 2:

			MapRot.z += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z > STAGE_ROT_LIMIT)
			{
				MapRot.z = STAGE_ROT_LIMIT;
			}

			break;
		case 3:

			MapRot.z -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z < -STAGE_ROT_LIMIT)
			{
				MapRot.z = -STAGE_ROT_LIMIT;
			}

			break;
		}

		break;

	case CDevil::SCROLL_TYPE_RETRO:

		switch (Arroow)
		{
		case 0:

			MapRot.x += D3DX_PI * SLOPE_SPEED02;

			if (MapRot.x > STAGE_ROT_LIMIT)
			{
				MapRot.x = STAGE_ROT_LIMIT;
			}

			break;
		case 1:

			MapRot.x -= D3DX_PI * SLOPE_SPEED02;

			if (MapRot.x < -STAGE_ROT_LIMIT)
			{
				MapRot.x = -STAGE_ROT_LIMIT;
			}

			break;
		case 2:

			MapRot.z += D3DX_PI * SLOPE_SPEED02;

			if (MapRot.z > STAGE_ROT_LIMIT)
			{
				MapRot.z = STAGE_ROT_LIMIT;
			}

			break;
		case 3:

			MapRot.z -= D3DX_PI * SLOPE_SPEED02;

			if (MapRot.z < -STAGE_ROT_LIMIT)
			{
				MapRot.z = -STAGE_ROT_LIMIT;
			}

			break;
		}

		break;

	default:

		switch (Arroow)
		{
		case 0:

			MapRot.x += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x > STAGE_ROT_LIMIT)
			{
				MapRot.x = STAGE_ROT_LIMIT;
			}

			break;
		case 1:

			MapRot.x -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x < -STAGE_ROT_LIMIT)
			{
				MapRot.x = -STAGE_ROT_LIMIT;
			}

			break;
		case 2:

			MapRot.z += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z > STAGE_ROT_LIMIT)
			{
				MapRot.z = STAGE_ROT_LIMIT;
			}

			break;
		case 3:

			MapRot.z -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z < -STAGE_ROT_LIMIT)
			{
				MapRot.z = -STAGE_ROT_LIMIT;
			}

			break;
		}

		break;
	}

	pMapField->SetRot(MapRot);
}

//====================================================================
// ステージ外との当たり判定
//====================================================================
void CDevil::CollisionOut()
{
	// キューブブロックのリスト構造が無ければ抜ける
	if (CEnemy::GetList() == nullptr) { return; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // リストを取得

	// キューブブロックリストの中身を確認する
	for (CEnemy* pEnemy : list)
	{
		CMapSystem::GRID EnemyGrid = pEnemy->GetGrid();
		D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
		D3DXVECTOR3 EnemyMove = pEnemy->GetMove();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float GritSize = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		//m_MinGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x - MapSize.x - GritSize);	//左
		//m_MaxGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x + MapSize.x - GritSize);	//右
		//m_MinGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z + MapSize.z + GritSize - 1.0f);	//上
		//m_MaxGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z - MapSize.z + GritSize);	//下

		//if (EnemyGrid.x == m_MinGrid.x)		//左
		//{
		//	if (EnemyMove.x < 0.0f || m_DevilArrow == 3)
		//	{
		//		EnemyPos = CMapSystem::GRID(m_MaxGrid.x, pEnemy->GetGrid().z).ToWorld();
		//		EnemyPos.x += GritSize * 2;
		//	}
		//}
		//if (EnemyGrid.x == m_MaxGrid.x)		//右
		//{
		//	if (EnemyMove.x > 0.0f || m_DevilArrow == 2)
		//	{
		//		EnemyPos = CMapSystem::GRID(m_MinGrid.x, pEnemy->GetGrid().z).ToWorld();
		//		EnemyPos.x -= GritSize * 2;
		//	}
		//}

		//if (EnemyGrid.z == m_MinGrid.z)		//上
		//{
		//	if (EnemyMove.z > 0.0f || m_DevilArrow == 0)
		//	{
		//		EnemyPos = CMapSystem::GRID(pEnemy->GetGrid().x, m_MaxGrid.z).ToWorld();
		//		EnemyPos.z -= GritSize * 2;
		//	}
		//}
		//if (EnemyGrid.z == m_MaxGrid.z)		//下
		//{
		//	if (EnemyMove.z < 0.0f || m_DevilArrow == 1)
		//	{
		//		EnemyPos = CMapSystem::GRID(pEnemy->GetGrid().x, m_MinGrid.z).ToWorld();
		//		EnemyPos.z += GritSize * 2;
		//	}
		//}

		m_MinGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x - MapSize.x - GritSize);	//左
		m_MinGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z + MapSize.z + GritSize);	//上
		m_MaxGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x + MapSize.x - GritSize);	//右
		m_MaxGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z - MapSize.z + GritSize);	//下

		CEffect *Effect = CEffect::Create();
		Effect->SetPos(CMapSystem::GetInstance()->GetGritPos(m_MinGrid));
		//Effect->SetPos(D3DXVECTOR3(m_DevilPos.x - MapSize.x - GritSize, 0.0f, m_DevilPos.z + MapSize.z + GritSize));
		Effect->SetRadius(50.0f);

		Effect = CEffect::Create();
		Effect->SetPos(CMapSystem::GetInstance()->GetGritPos(m_MaxGrid));
		//Effect->SetPos(D3DXVECTOR3(m_DevilPos.x + MapSize.x - GritSize, 0.0f, m_DevilPos.z - MapSize.z + GritSize));
		Effect->SetRadius(50.0f);

		if (EnemyGrid.x == -1)
		{
			if (EnemyPos.x > 0.0f)
			{
				m_MinGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x - MapSize.x - GritSize);	//左
				EnemyPos = CMapSystem::GRID(m_MinGrid.x, pEnemy->GetGrid().z).ToWorld();
			}
			else
			{
				m_MaxGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x + MapSize.x - GritSize);	//右
				EnemyPos = CMapSystem::GRID(m_MaxGrid.x, pEnemy->GetGrid().z).ToWorld();
			}
		}
		else if (EnemyGrid.z == -1)
		{
			if (EnemyPos.z < 0.0f)
			{
				m_MinGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z + MapSize.z + GritSize);	//上
				EnemyPos = CMapSystem::GRID(pEnemy->GetGrid().x, m_MinGrid.z).ToWorld();
			}
			else
			{
				m_MaxGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z - MapSize.z + GritSize);	//下
				EnemyPos = CMapSystem::GRID(pEnemy->GetGrid().x, m_MaxGrid.z).ToWorld();
			}
		}

		pEnemy->SetPos(EnemyPos);
	}
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CDevil::ActionState(void)
{
	// 上モーション
	if (m_DevilArrow == 0)
	{
		if (m_Action != ACTION_SIGNAL_UP)
		{
			m_Action = ACTION_SIGNAL_UP;
			m_nStateNum = ACTION_SIGNAL_UP;
			GetMotion()->Set(ACTION_SIGNAL_UP, 5);
		}
	}
	// 下モーション
	if (m_DevilArrow == 1)
	{
		if (m_Action != ACTION_SIGNAL_DOWN)
		{
			m_Action = ACTION_SIGNAL_DOWN;
			m_nStateNum = ACTION_SIGNAL_DOWN;
			GetMotion()->Set(ACTION_SIGNAL_DOWN, 5);
		}
	}
	// 左モーション
	if (m_DevilArrow == 2)
	{
		if (m_Action != ACTION_SIGNAL_LEFT)
		{
			m_Action = ACTION_SIGNAL_LEFT;
			m_nStateNum = ACTION_SIGNAL_LEFT;
			GetMotion()->Set(ACTION_SIGNAL_LEFT, 5);
		}
	}
	// 右モーション
	if (m_DevilArrow == 3)
	{
		if (m_Action != ACTION_SIGNAL_RIGHT)
		{
			m_Action = ACTION_SIGNAL_RIGHT;
			m_nStateNum = ACTION_SIGNAL_RIGHT;
			GetMotion()->Set(ACTION_SIGNAL_RIGHT, 5);
		}
	}
	// ニュートラルモーション
	else
	{
		if (m_Action != ACTION_NEUTRAL)
		{
			m_Action = ACTION_NEUTRAL;
			GetMotion()->Set(ACTION_NEUTRAL, 5);
		}
	}
}

//====================================================================
//状態管理
//====================================================================
void CDevil::StateManager(void)
{
	switch (m_State)
	{
	case STATE_WAIT:

		// 状態終了時
		if (m_nStateCount <= 0)
		{
			// 100％のrand()をまわす
			int nRand = rand() % 101;

			// 傾きの指定％の時
			if (nRand <= SLOPE_RAND)
			{
				// 傾き状態設定
				m_State = STATE_SLOPE;

				// 傾いている状態かどうかを切り替える
				m_bSlope = !m_bSlope;

				if (m_bSlope)
				{// 傾き状態の時

					//傾き方向指定処理
					m_DevilArrow = rand() % 2;

					if (m_SlopwArrowOld == 0 || m_SlopwArrowOld == 1)
					{// 前回の傾き方向が左右だった場合

						// 今回の傾き方向は上下にする
						if (m_DevilArrow == 0)
						{
							m_DevilArrow = 2;
						}
						else if (m_DevilArrow == 1)
						{
							m_DevilArrow = 3;
						}
					}

					// 今回の傾き方向を記録する
					m_SlopwArrowOld = m_DevilArrow;

					m_nStateCount = SLOPE_TIME;

					m_nStateNum = m_DevilArrow;

					// 傾き装置のリスト構造が無ければ抜ける
					if (CSlopeDevice::GetList() == nullptr) { return; }
					std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // リストを取得

					// 傾き装置のリストの中身を確認する
					for (CSlopeDevice* pSlopeDevice : list)
					{
						// 方向の傾き装置を上昇状態に変更
						pSlopeDevice->SetState(CSlopeDevice::STATE_ASCENT, (CScrollArrow::Arrow)m_DevilArrow);
					}
				}
				else
				{// 傾き戻し状態の時

					// 傾きを戻す時だけ倍の時間を指定し、戻り切ったら傾き状態を終了とする
					m_nStateCount = SLOPE_TIME * 2;

					// 傾き装置のリスト構造が無ければ抜ける
					if (CSlopeDevice::GetList() == nullptr) { return; }
					std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // リストを取得

					// 傾き装置のリストの中身を確認する
					for (CSlopeDevice* pSlopeDevice : list)
					{
						// 方向の傾き装置を上昇状態に変更
						pSlopeDevice->SetState(CSlopeDevice::STATE_DESCENT, (CScrollArrow::Arrow)m_SlopwArrowOld);
					}
				}
			}
			else
			{// 傾きの指定％じゃない時

				// スクロール状態設定
				m_State = STATE_SCROLL;

				// スクロール時間設定
				m_nStateCount = SCROOL_TIME;

				// スクロール方向指定
				m_DevilArrow = rand() % 2;

				if (m_ScrollArrowOld == 0 || m_ScrollArrowOld == 1)
				{// 前回の傾き方向が左右だった場合

					// 今回の傾き方向は上下にする
					if (m_DevilArrow == 0)
					{
						m_DevilArrow = 2;
					}
					else if (m_DevilArrow == 1)
					{
						m_DevilArrow = 3;
					}
				}
				m_ScrollArrowOld = m_DevilArrow;
				m_nStateNum = m_DevilArrow;

				// マップ移動装置のリスト構造が無ければ抜ける
				if (CScrollDevice::GetList() == nullptr) { return; }
				std::list<CScrollDevice*> list = CScrollDevice::GetList()->GetList();    // リストを取得

				// マップ移動装置のリストの中身を確認する
				for (CScrollDevice* pScrollDevice : list)
				{
					// 回転状態に変更
					pScrollDevice->SetState(CScrollDevice::STATE_ROTATE);
				}
			}

			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				if (m_ScrollArrow[nCnt] != nullptr)
				{
					switch (m_DevilArrow)
					{
					case 0:
						m_ScrollArrow[nCnt]->SetState(CScrollArrow::Arrow::STATE_UP);

						break;

					case 1:
						m_ScrollArrow[nCnt]->SetState(CScrollArrow::Arrow::STATE_DOWN);
						break;

					case 2:
						m_ScrollArrow[nCnt]->SetState(CScrollArrow::Arrow::STATE_LEFT);
						break;

					case 3:
						m_ScrollArrow[nCnt]->SetState(CScrollArrow::Arrow::STATE_RIGHT);
						break;
					}
				}
			}
		}

		break;

	case STATE_SCROLL:

		switch (m_ScrollType)
		{
		case CDevil::SCROLL_TYPE_NORMAL:
			Move(m_DevilArrow);
			break;

		case CDevil::SCROLL_TYPE_RETRO:

			if (m_nStateCount % (SCROOL_TIME / SCROOL_COUNT_02) == 0)
			{
				Move(m_DevilArrow);
			}

			break;

		default:

			break;
		}

		if (m_nStateCount <= 0)
		{
			m_State = STATE_WAIT;
			m_nStateCount = 120;

			// マップ移動装置のリスト構造が無ければ抜ける
			if (CScrollDevice::GetList() == nullptr) { return; }
			std::list<CScrollDevice*> list = CScrollDevice::GetList()->GetList();    // リストを取得

			// マップ移動装置のリストの中身を確認する
			for (CScrollDevice* pScrollDevice : list)
			{
				// 通常状態に変更
				pScrollDevice->SetState(CScrollDevice::STATE_NORMAL);
			}
		}

		break;

	case STATE_SLOPE:

		switch (m_ScrollType)
		{
		case CDevil::SCROLL_TYPE_NORMAL:

			if (m_bSlope)
			{
				Slope(m_DevilArrow);
			}
			else
			{
				BackSlope();
			}
			break;

		case CDevil::SCROLL_TYPE_RETRO:

			if (m_nStateCount % 25 == 0)
			{
				if (m_bSlope)
				{
					Slope(m_DevilArrow);
				}
				else
				{
					BackSlope();
				}
			}
			break;

		default:

			break;
		}

		if (m_nStateCount <= 0)
		{
			m_State = STATE_WAIT;
			m_nStateCount = 120;
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

	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//キーボードの移動処理
	if (pInputKeyboard->GetPress(DIK_UP))
	{
		m_DevilArrow = 0;
		Move(0);

	}
	if (pInputKeyboard->GetPress(DIK_DOWN))
	{
		m_DevilArrow = 1;
		Move(1);
	}

	if (pInputKeyboard->GetPress(DIK_LEFT))
	{
		m_DevilArrow = 2;
		Move(2);
	}
	if (pInputKeyboard->GetPress(DIK_RIGHT))
	{
		m_DevilArrow = 3;
		Move(3);
	}

	if (pInputKeyboard->GetPress(DIK_5))
	{
		CObjmeshField* pMapField = CGame::GetInstance()->GetMapField();
		D3DXVECTOR3 MapRot = pMapField->GetRot();
		MapRot = INITVECTOR3;
		pMapField->SetRot(MapRot);
	}

	if (pInputKeyboard->GetPress(DIK_6))
	{
		Slope(0);
	}
	if (pInputKeyboard->GetPress(DIK_7))
	{
		Slope(1);
	}
	if (pInputKeyboard->GetPress(DIK_8))
	{
		Slope(2);
	}
	if (pInputKeyboard->GetPress(DIK_9))
	{
		Slope(3);
	}
#endif // !_DEBUG
}

//====================================================================
// モデルのモーション設定
//====================================================================
void CDevil::SetAction(ACTION_TYPE Action, float BlendTime)
{
	// モーションの取得処理
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
void CDevil::SetModelDisp(bool Sst)
{
	// モデル数を取得
	int nNumModel = GetNumModel();

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// モデルの取得処理
		CModel* pModel = GetModel(nCnt);

		if (pModel != nullptr)
		{
			// 表示設定
			pModel->SetDisp(Sst);
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

	// 転がる岩のスクロール
	RollRockScroll(Move, m_GritSize);

	// プレイヤーのスクロール
	PlayerScroll(Move, m_GritSize);

	// ファイアボールのスクロール
	FireScroll(Move, m_GritSize);

	// 床のスクロール
	TileScroll(Move, m_GritSize);

	// 壁のスクロール
	WallScroll(Move, m_GritSize);
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
		// マップ変数
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		
		// オブジェ変数
		D3DXVECTOR3 pos = pCross->GetPos();
		D3DXVECTOR3 Size = pCross->GetSize();

		// 移動	
		pos += Move;

		// x座標判定
		if (Move.x > 0.0f)
		{// 移動量がプラス
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

		// z座標判定
		if (Move.z > 0.0f)
		{// 移動量がプラス
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
// 転がる岩のスクロール
//====================================================================
void CDevil::RollRockScroll(D3DXVECTOR3 Move, float GritSize)
{
	// レールブロックのリスト構造が無ければ抜ける
	if (CRollRock::GetList() == nullptr) { return; }
	std::list<CRollRock*> list = CRollRock::GetList()->GetList();    // リストを取得

	// レールブロックのリストの中身を確認する
	for (CRollRock* pRailBlock : list)
	{
		D3DXVECTOR3 pos = pRailBlock->GetPos();
		pos += Move;
		pRailBlock->SetPos(pos);
	}
}

//==========================================
//  ファイアボールのスクロール
//==========================================
void CDevil::FireScroll(D3DXVECTOR3 Move, float GritSize)
{
	// ファイアボールのリストが無ければ抜ける
	if (CFire::GetList() == nullptr) { return; }
	std::list<CFire*> list = CFire::GetList()->GetList();    // リストを取得

	// ファイアボールのリストの中身を確認する
	for (CFire* fire : list)
	{
		D3DXVECTOR3 pos = fire->GetPos();
		pos += Move;
		fire->SetPos(pos);
	}
}

//====================================================================
// 床のスクロール
//====================================================================
void CDevil::TileScroll(D3DXVECTOR3 Move, float GritSize)
{
	// 十字架のリスト構造が無ければ抜ける
	if (CTile::GetList() == nullptr) { return; }
	std::list<CTile*> list = CTile::GetList()->GetList();    // リストを取得

	// キューブブロックのリストの中身を確認する
	for (CTile* pTile : list)
	{
		// 縦横のナンバーと高さを設定する
		D3DXVECTOR3 pos = INITVECTOR3;

		//グリット番号を位置に変換
		pos = pTile->GetGrid().ToWorld();
		pos.y = 0.0f;

		pTile->SetPos(pos);
	}
}

//====================================================================
// 壁のスクロール
//====================================================================
void CDevil::WallScroll(D3DXVECTOR3 Move, float GritSize)
{
	// 十字架のリスト構造が無ければ抜ける
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // リストを取得

	// キューブブロックのリストの中身を確認する
	for (CWall* pWall : list)
	{
		// 縦横のナンバーと高さを設定する
		D3DXVECTOR3 pos = INITVECTOR3;

		//グリット番号を位置に変換
		pos = pWall->GetGrid().ToWorld();
		pos.y = 0.0f;

		pWall->SetPos(pos);
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
			float G_Size = CMapSystem::GetInstance()->GetGritSize();

			pos += Move;

			if (Move.x > 0.0f)
			{
				if (pos.x + G_Size > m_DevilPos.x + MapSize.x)	// 右
				{
					pos.x = m_DevilPos.x + MapSize.x - G_Size;
					CollisionPressPlayer(pPlayer, pos, Size);
				}
			}
			if (Move.x < 0.0f)
			{
				if (pos.x - G_Size < m_DevilPos.x - MapSize.x)	// 左
				{
					pos.x = m_DevilPos.x - MapSize.x + G_Size;
					CollisionPressPlayer(pPlayer, pos, Size);
				}
			}
			if (Move.z > 0.0f)
			{
				if (pos.z + G_Size > m_DevilPos.z + MapSize.z)	// 上
				{
					pos.z = m_DevilPos.z + MapSize.z - G_Size;
					CollisionPressPlayer(pPlayer, pos, Size);
				}
			}
			if (Move.z < 0.0f)
			{
				if (pos.z - G_Size < m_DevilPos.z - MapSize.z)	// 下
				{
					pos.z = m_DevilPos.z - MapSize.z + G_Size;
					CollisionPressPlayer(pPlayer, pos, Size);
				}
			}

			pPlayer->SetPos(pos);
		}
		else
		{// 卵状態の時、ブロックが存在しない位置にホーミングする

			CMapSystem::GRID Grit = pPlayer->GetGrid();;
			if (!CMapSystem::GetInstance()->GetGritBool(Grit))
			{
				D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
				D3DXVECTOR3 AnswerPos = INITVECTOR3;
				AnswerPos = Grit.ToWorld();

				if (pPlayer->GetGritCenter())
				{
					AnswerPos.y = PlayerPos.y;
					pPlayer->SetPos(AnswerPos);
				}
				else
				{
					PlayerPos.x += (AnswerPos.x - PlayerPos.x);
					PlayerPos.z += (AnswerPos.z - PlayerPos.z);
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

#ifdef _DEBUG

	////　グリットの位置にエフェクトを表示
	//for (int nCntW = 0; nCntW < CMapSystem::GetInstance()->GetWightMax(); nCntW++)
	//{
	//	for (int nCntH = 0; nCntH < CMapSystem::GetInstance()->GetHeightMax(); nCntH++)
	//	{
	//		if (CMapSystem::GetInstance()->GetGritBool(nCntW, nCntH))
	//		{// ブロックが存在するグリットのみエフェクトを表示

	//			CEffect* pEffect = CEffect::Create();
	//			pEffect->SetPos(CMapSystem::GetInstance()->GetGritPos(CMapSystem::GRID(nCntW, nCntH)));
	//			pEffect->SetRadius(20.0f);
	//			pEffect->SetLife(10);
	//		}
	//	}
	//}

#endif // _DEBUG

	CMapSystem::GetInstance()->SetMapPos(MapPos);

	// キューブブロックのリスト構造が無ければ抜ける
	if (CCubeBlock::GetList() == nullptr) { return; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // リストを取得

	// キューブブロックのリストの中身を確認する
	for (CCubeBlock* pCubeBlock : list)
	{
		// 縦横のナンバーと高さを設定する
		D3DXVECTOR3 pos = INITVECTOR3;
		CMapSystem::GRID grid = CMapSystem::GRID
		(
			pCubeBlock->GetWightNumber(),
			pCubeBlock->GetHeightNumber()
		);

		//グリット番号を位置に変換
		pos = grid.ToWorld();
		pos.y = 50.0f;

		pCubeBlock->SetPos(pos);
	}
}

//====================================================================
// 矢印が回転する処理
//====================================================================
void CDevil::SignalScroll(void)
{
	switch (m_nStateNum)
	{
	case ACTION_SIGNAL_UP:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI,0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		break;

	case ACTION_SIGNAL_DOWN:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); 
		break;

	case ACTION_SIGNAL_RIGHT:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		break;

	case ACTION_SIGNAL_LEFT:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f)); 
		break;
	}
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
float CDevil::MoveSlopeX(float Move)
{
	float fSlopeMove = 1.0f;

	D3DXVECTOR3 DevilRot = m_DevilRot;

	if (Move > 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.z));
	}
	else if (Move < 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.z));
	}

	return fSlopeMove;
}

//====================================================================
//傾き中の移動量変動
//====================================================================
float CDevil::MoveSlopeZ(float Move)
{
	float fSlopeMove = 1.0f;

	D3DXVECTOR3 DevilRot = m_DevilRot;

	if (Move > 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.x));
	}
	else if (Move < 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.x));
	}

	return fSlopeMove;
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CDevil>* CDevil::GetList(void)
{
	return m_pList;
}

//====================================================================
//リストの先頭取得
//====================================================================
CDevil* CDevil::GetListTop(void)
{
	if (CDevil::GetList() == nullptr) { return nullptr; }
	std::list<CDevil*> list = CDevil::GetList()->GetList();    // リストを取得
	CDevil* pDevil = list.front();

	return pDevil;
}
