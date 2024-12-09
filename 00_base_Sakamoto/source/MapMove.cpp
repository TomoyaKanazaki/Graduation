//============================================
//
//	マップの動き [MapMove.cpp]
//	Author:sakamoto kai
//
//============================================
#include "MapMove.h"
#include "object2D.h"
#include "Fade.h"
#include "sound.h"
#include "objmeshField.h"
#include "enemy.h"
#include "ScrollDevice.h"
#include "SlopeDevice.h"
#include "signal.h"
#include "effect.h"

#include "bowabowa.h"
#include "Cross.h"
#include "player.h"
#include "enemy.h"
#include "railblock.h"
#include "RollRock.h"
#include "tile.h"
#include "friedegg.h"
#include "fire.h"
#include "wall.h"

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
	float SCROOL_SPEED_02 = (CMapSystem::GetGritSize() * SCROOL_MOVEGRID_02) / SCROOL_COUNT_02;			// スクロールの移動速度

	int SLOPE_TIME = 300;						// 傾き操作時間
	int SLOPE_RAND = 75;						// 傾き発生確率
	float STAGE_ROT_LIMIT = D3DX_PI * 0.15f;	// 傾きの角度制限

	float SLOPE_SPEED01 = 0.00075f;				// 傾きの移動速度

	float SLOPE_SPEED02 = 0.0125f;				// 傾きの移動速度
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CMapMove>* CMapMove::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CMapMove::CMapMove()
{
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_fActionCount = 0.0f;
	m_State = MOVE_WAIT;
	m_SlopeOld = MOVE_WAIT;
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
	m_ScrollType = SCROLL_TYPE_NORMAL;
	m_pSignal[0] = nullptr;
	m_pSignal[1] = nullptr;
	m_pSignal[2] = nullptr;
	m_pSignal[3] = nullptr;
	m_nStateNum = 0;
	m_SlopeType = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CMapMove::~CMapMove()
{

}

//====================================================================
//生成処理
//====================================================================
CMapMove* CMapMove::Create()
{
	CMapMove* pPause = nullptr;

	if (pPause == nullptr)
	{
		//敵の生成
		pPause = new CMapMove();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPause->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pPause;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CMapMove::Init(void)
{
	m_ScrollType = (SCROLL_TYPE)CManager::GetInstance()->GetScrollType();

	//矢印の生成
	SignalCreate();

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CMapMove>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CMapMove::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 自身を削除する
	delete this;
}

//====================================================================
//更新処理
//====================================================================
void CMapMove::Update(void)
{
#ifdef _DEBUG
	//m_ScrollType = SCROLL_TYPE_MAX;
#endif // _DEBUG

	// マップの傾き
	m_DevilRot = CObjmeshField::GetListTop()->GetRot();

	// 矢印の回転
	SignalManager();

	//状態の管理
	StateManager();

	//デバッグキーの処理と設定
	DebugKey();

	//ステージ外にいるオブジェクトの処理
	CollisionOut();

	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
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
}

//====================================================================
//描画処理
//====================================================================
void CMapMove::Draw(void)
{

}

//====================================================================
//移動処理
//====================================================================
void CMapMove::Move(int Arroow)
{
	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	switch (m_ScrollType)
	{
	case CMapMove::SCROLL_TYPE_NORMAL:

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

	case CMapMove::SCROLL_TYPE_RETRO:

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
void CMapMove::BackSlope(void)
{
	CObjmeshField* pMapField = CObjmeshField::GetListTop();
	D3DXVECTOR3 MapRot = pMapField->GetRot();
	bool bBackOK = false;

	switch (m_ScrollType)
	{
	case CMapMove::SCROLL_TYPE_NORMAL:

		if (MapRot.x > 0.0f)
		{
			MapRot.x -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x <= 0.0f)
			{
				MapRot.x = 0.0f;
				bBackOK = true;
			}
		}

		if (MapRot.x < 0.0f)
		{
			MapRot.x += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x >= 0.0f)
			{
				MapRot.x = 0.0f;
				bBackOK = true;
			}
		}

		if (MapRot.z > 0.0f)
		{
			MapRot.z -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z <= 0.0f)
			{
				MapRot.z = 0.0f;
				bBackOK = true;
			}
		}

		if (MapRot.z < 0.0f)
		{
			MapRot.z += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z >= 0.0f)
			{
				MapRot.z = 0.0f;
				bBackOK = true;
			}
		}

		break;

	case CMapMove::SCROLL_TYPE_RETRO:

		if (MapRot.x > 0.0f)
		{
			MapRot.x -= D3DX_PI * SLOPE_SPEED02;

			if (MapRot.x <= 0.0f)
			{
				MapRot.x = 0.0f;
				bBackOK = true;
			}
		}

		if (MapRot.x < 0.0f)
		{
			MapRot.x += D3DX_PI * SLOPE_SPEED02;

			if (MapRot.x >= 0.0f)
			{
				MapRot.x = 0.0f;
				bBackOK = true;
			}
		}

		if (MapRot.z > 0.0f)
		{
			MapRot.z -= D3DX_PI * SLOPE_SPEED02;

			if (MapRot.z <= 0.0f)
			{
				MapRot.z = 0.0f;
				bBackOK = true;
			}
		}

		if (MapRot.z < 0.0f)
		{
			MapRot.z += D3DX_PI * SLOPE_SPEED02;

			if (MapRot.z >= 0.0f)
			{
				MapRot.z = 0.0f;
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
		MOVE_WAIT;
		m_nStateCount = 60;
		MapRot.x = 0.0f;
		MapRot.z = 0.0f;
	}

	pMapField->SetRot(MapRot);
}

//====================================================================
//移動方向処理
//====================================================================
void CMapMove::Slope(int Arroow)
{
	CObjmeshField* pMapField = CObjmeshField::GetListTop();
	if (pMapField == nullptr) { assert(false); }

	D3DXVECTOR3 MapRot = pMapField->GetRot();

	switch (m_ScrollType)
	{
	case CMapMove::SCROLL_TYPE_NORMAL:

		switch (Arroow)
		{
		case 0:

			MapRot.x += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x > STAGE_ROT_LIMIT)
			{
				MapRot.x = STAGE_ROT_LIMIT;
			}
			// 右側
			m_SlopeType = 0;

			break;
		case 1:

			MapRot.x -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x < -STAGE_ROT_LIMIT)
			{
				MapRot.x = -STAGE_ROT_LIMIT;
			}
			// 左側
			m_SlopeType = 1;

			break;
		case 2:

			MapRot.z += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z > STAGE_ROT_LIMIT)
			{
				MapRot.z = STAGE_ROT_LIMIT;
			}
			// 奥側
			m_SlopeType = 2;

			break;
		case 3:

			MapRot.z -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z < -STAGE_ROT_LIMIT)
			{
				MapRot.z = -STAGE_ROT_LIMIT;
			}
			// 手前側
			m_SlopeType = 3;

			break;
		}

		m_SlopeOld = m_State;

		break;

	case CMapMove::SCROLL_TYPE_RETRO:

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

		m_SlopeOld = m_State;

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
void CMapMove::CollisionOut()
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

		m_MinGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x - MapSize.x - GritSize);	//左
		m_MinGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z + MapSize.z + GritSize);	//上
		m_MaxGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x + MapSize.x - GritSize);	//右
		m_MaxGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z - MapSize.z + GritSize);	//下

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
//状態管理
//====================================================================
void CMapMove::StateManager(void)
{
	switch (m_State)
	{
	case MOVE_WAIT:

		// 状態終了時
		if (m_nStateCount <= 0)
		{
			// 100％のrand()をまわす
			int nRand = rand() % 101;

			// 傾きの指定％の時
			if (nRand <= SLOPE_RAND)
			{
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

					// 状態設定
					switch (m_DevilArrow)
					{
					case 0:
						m_State = MOVE_SLOPE_UP;
						break;
					case 1:	  
						m_State = MOVE_SLOPE_DOWN;
						break;
					case 2:	   
						m_State = MOVE_SLOPE_LEFT;
						break;
					case 3:	   
						m_State = MOVE_SLOPE_RIGHT;
						break;
					}

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

					m_State = m_SlopeOld;

					// 傾き装置のリスト構造が無ければ抜ける
					if (CSlopeDevice::GetList() == nullptr) { return; }
					std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // リストを取得

					// 傾き装置のリストの中身を確認する
					for (CSlopeDevice* pSlopeDevice : list)
					{
						// 方向の傾き装置を下降状態に変更
						pSlopeDevice->SetState(CSlopeDevice::STATE_DESCENT, (CScrollArrow::Arrow)m_SlopwArrowOld);
					}
				}
			}
			else
			{// 傾きの指定％じゃない時

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

				// 状態設定
				switch (m_DevilArrow)
				{
				case 0:
					m_State = MOVE_SCROLL_UP;
					break;
				case 1:
					m_State = MOVE_SCROLL_DOWN;
					break;
				case 2:
					m_State = MOVE_SCROLL_LEFT;
					break;
				case 3:
					m_State = MOVE_SCROLL_RIGHT;
					break;
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
		}

		break;

	case MOVE_SCROLL_UP:
	case MOVE_SCROLL_DOWN:
	case MOVE_SCROLL_RIGHT:
	case MOVE_SCROLL_LEFT:

		switch (m_ScrollType)
		{
		case CMapMove::SCROLL_TYPE_NORMAL:
			Move(m_DevilArrow);
			break;

		case CMapMove::SCROLL_TYPE_RETRO:

			if (m_nStateCount % (SCROOL_TIME / SCROOL_COUNT_02) == 0)
			{
				Move(m_DevilArrow);
			}

			break;

		default:

			break;
		}

		if (m_nStateCount <= 1)
		{
			m_State = MOVE_WAIT;
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

	case MOVE_SLOPE_UP:
	case MOVE_SLOPE_DOWN:
	case MOVE_SLOPE_RIGHT:
	case MOVE_SLOPE_LEFT:

		switch (m_ScrollType)
		{
		case CMapMove::SCROLL_TYPE_NORMAL:

			if (m_bSlope)
			{
				Slope(m_DevilArrow);

				// 矢印傾き
				SignalManager();
			}
			else
			{
				BackSlope();
			}
			break;

		case CMapMove::SCROLL_TYPE_RETRO:

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
			m_State = MOVE_WAIT;
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
void CMapMove::DebugKey(void)
{
#ifdef _DEBUG

	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

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
		CObjmeshField* pMapField = CObjmeshField::GetListTop();
		D3DXVECTOR3 MapRot = pMapField->GetRot();
		MapRot = INITVECTOR3;
		pMapField->SetRot(MapRot);
	}

	if (pInputKeyboard->GetPress(DIK_6))
	{
		m_pSignal[0]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));

		Slope(0);
	}
	if (pInputKeyboard->GetPress(DIK_7))
	{
		m_pSignal[0]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));

		Slope(1);
	}
	if (pInputKeyboard->GetPress(DIK_8))
	{
		m_pSignal[0]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));

		Slope(2);
	}
	if (pInputKeyboard->GetPress(DIK_9))
	{
		m_pSignal[0]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));


		Slope(3);
	}
#endif // !_DEBUG
}

//====================================================================
// オブジェクトのスクロール
//====================================================================
void CMapMove::ObjectScroll(D3DXVECTOR3 Move)
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

	// 目玉焼きのスクロール
	FriedEggScroll(Move, m_GritSize);
}

//====================================================================
// 十字架のスクロール
//====================================================================
void CMapMove::CrossScroll(D3DXVECTOR3 Move, float GritSize)
{
	// 十字架のリスト構造が無ければ抜ける
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // リストを取得

	// 十字架のリストの中身を確認する
	for (CCross* pCross : list)
	{
		D3DXVECTOR3 pos = CMapSystem::GetInstance()->GetGritPos(pCross->GetGrid());
		pos.y = pCross->GetPos().y;
		pCross->SetPos(pos);
	}
}

//====================================================================
// ボワボワのスクロール
//====================================================================
void CMapMove::BowabowaScroll(D3DXVECTOR3 Move, float GritSize)
{
	// ボワボワのリスト構造が無ければ抜ける
	if (CBowabowa::GetList() == nullptr) { return; }
	std::list<CBowabowa*> list = CBowabowa::GetList()->GetList();    // リストを取得

	// ボワボワのリストの中身を確認する
	for (CBowabowa* pBowabowa : list)
	{
		D3DXVECTOR3 pos = CMapSystem::GetInstance()->GetGritPos(pBowabowa->GetGrid());
		pBowabowa->SetPos(pos);
	}
}

//====================================================================
// 敵のスクロール
//====================================================================
void CMapMove::EnemyScroll(D3DXVECTOR3 Move, float GritSize)
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
void CMapMove::RailBlockScroll(D3DXVECTOR3 Move, float GritSize)
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
void CMapMove::RollRockScroll(D3DXVECTOR3 Move, float GritSize)
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
void CMapMove::FireScroll(D3DXVECTOR3 Move, float GritSize)
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
void CMapMove::TileScroll(D3DXVECTOR3 Move, float GritSize)
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
void CMapMove::WallScroll(D3DXVECTOR3 Move, float GritSize)
{
	// 壁のリスト構造が無ければ抜ける
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // リストを取得

	// 壁のリストの中身を確認する
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

//==========================================
//  目玉焼きのスクロール
//==========================================
void CMapMove::FriedEggScroll(D3DXVECTOR3 Move, float GritSize)
{
	// 目玉焼きのリスト構造が無ければ抜ける
	if (CFriedEgg::GetList() == nullptr) { return; }
	std::list<CFriedEgg*> list = CFriedEgg::GetList()->GetList();    // リストを取得

	// 目玉焼きのリストの中身を確認する
	for (CFriedEgg* pEgg : list)
	{
		// 縦横のナンバーと高さを設定する
		D3DXVECTOR3 pos = INITVECTOR3;

		//グリット番号を位置に変換
		pos = pEgg->GetGrid().ToWorld();

		pEgg->SetPos(pos);
	}
}

//====================================================================
// プレイヤーのスクロール
//====================================================================
void CMapMove::PlayerScroll(D3DXVECTOR3 Move, float GritSize)
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
void CMapMove::GritScroll(D3DXVECTOR3 Move)
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

	//　グリットの位置にエフェクトを表示
	for (int nCntW = 0; nCntW < CMapSystem::GetInstance()->GetWightMax(); nCntW++)
	{
		for (int nCntH = 0; nCntH < CMapSystem::GetInstance()->GetHeightMax(); nCntH++)
		{
			if (CMapSystem::GetInstance()->GetGritBool(nCntW, nCntH))
			{// ブロックが存在するグリットのみエフェクトを表示

				//CEffect* pEffect = CEffect::Create();
				//pEffect->SetPos(CMapSystem::GetInstance()->GetGritPos(CMapSystem::GRID(nCntW, nCntH)));
				//pEffect->SetRadius(20.0f);
				//pEffect->SetLife(10);
			}
		}
	}

#endif // _DEBUG

	CMapSystem::GetInstance()->SetMapPos(MapPos);
}

//====================================================================
// スクロールによる移動
//====================================================================
void CMapMove::FollowPos(D3DXVECTOR3& Pos)
{
	Pos += m_move;
}

//====================================================================
// 矢印の生成
//====================================================================
void CMapMove::SignalCreate(void)
{
	// 矢印生成
	if (m_pSignal[0] == nullptr)
	{
		m_pSignal[0] = CSignal::Create(0);
		m_pSignal[0]->SetPos(D3DXVECTOR3(-1000.0f, 200.0f, 500.0f));
	}
	if (m_pSignal[1] == nullptr)
	{
		m_pSignal[1] = CSignal::Create(0);
		m_pSignal[1]->SetPos(D3DXVECTOR3(1000.0f, 200.0f, 500.0f));
	}
	if (m_pSignal[2] == nullptr)
	{
		m_pSignal[2] = CSignal::Create(0);
		m_pSignal[2]->SetPos(D3DXVECTOR3(-1000.0f, 200.0f, -500.0f));
	}
	if (m_pSignal[3] == nullptr)
	{
		m_pSignal[3] = CSignal::Create(0);
		m_pSignal[3]->SetPos(D3DXVECTOR3(1000.0f, 200.0f, -500.0f));
	}
}

//====================================================================
// 矢印の管理
//====================================================================
void CMapMove::SignalManager(void)
{
	switch (m_State)
	{
	case MOVE_SCROLL_UP:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		break;

	case MOVE_SCROLL_DOWN:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;

	case MOVE_SCROLL_RIGHT:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		break;

	case MOVE_SCROLL_LEFT:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		break;

	case MOVE_SLOPE_UP:
		m_pSignal[0]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		break;

	case MOVE_SLOPE_DOWN:
		m_pSignal[0]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		break;

	case MOVE_SLOPE_LEFT:
		m_pSignal[0]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));

		break;

	case MOVE_SLOPE_RIGHT:
		m_pSignal[0]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		break;
	}
}

//====================================================================
// プレイヤーが潰される時の処理
//====================================================================
void CMapMove::CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size)
{
	//// キューブブロックのリスト構造が無ければ抜ける
	//if (CCubeBlock::GetList() == nullptr) { return; }
	//std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // リストを取得

	//// キューブブロックのリストの中身を確認する
	//for (CCubeBlock* pCubeBlock : list)
	//{
	//	D3DXVECTOR3 Blockpos = pCubeBlock->GetPos();
	//	D3DXVECTOR3 BlockSize = pCubeBlock->GetSize();

	//	if (useful::CollisionRectangle2D(pos, Blockpos, Size, BlockSize, useful::COLLISION::COLLISION_ZX))
	//	{
	//		pPlayer->Death();
	//		return;
	//	}
	//}
}

//====================================================================
//傾き中の移動量変動
//====================================================================
float CMapMove::MoveSlopeX(float Move)
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
float CMapMove::MoveSlopeZ(float Move)
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
CListManager<CMapMove>* CMapMove::GetList(void)
{
	return m_pList;
}

//====================================================================
//リストの先頭取得
//====================================================================
CMapMove* CMapMove::GetListTop(void)
{
	if (CMapMove::GetList() == nullptr) { return nullptr; }
	std::list<CMapMove*> list = CMapMove::GetList()->GetList();    // リストを取得
	CMapMove* pDevil = list.front();

	return pDevil;
}
