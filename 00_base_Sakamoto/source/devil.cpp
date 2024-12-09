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
#include "slowManager.h"
#include "Number.h"
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
#include "friedegg.h"
#include "MapMove.h"

//===========================================
// 定数定義
//===========================================
namespace
{

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
	m_nStateCount = 0;
	m_CollisionRot = 0.0f;
	m_DevilPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MapDifference = INITVECTOR3;
	m_bSlope = false;
	m_MinGrid = CMapSystem::GRID(0, 0);
	m_MaxGrid = CMapSystem::GRID(NUM_WIGHT - 1, NUM_HEIGHT - 1);
	m_DevilArrow = 0;
	m_ScrollArrowOld = 0;
	m_SlopwArrowOld = 0;
	m_pSignal[0] = nullptr;
	m_pSignal[1] = nullptr;
	m_pSignal[2] = nullptr;
	m_pSignal[3] = nullptr;
	m_nStateNum = 0;
	m_SlopeType = 0;
	m_pMapMove = nullptr;
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

	// キャラクタークラスの初期化（継承）
	if (FAILED(CObjectCharacter::Init())) { assert(false); }

	// キャラクターテキスト読み込み処理
	SetTxtCharacter("data\\TXT\\MOTION\\01_enemy\\motion_devil.txt", 0);

	if (m_pMapMove == nullptr)
	{
		m_pMapMove = CMapMove::Create();
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
	if (m_pMapMove != nullptr)
	{
		m_pMapMove->Uninit();
		m_pMapMove = nullptr;
	}

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
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (CGame::GetInstance()->GetEvent() == false &&
			m_pMapMove != nullptr)
		{
			//マップの動き
			m_pMapMove->Update();
		}
	}

	// キャラクタークラスの更新（継承）
	CObjectCharacter::Update();

	//イベントモーション
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (CGame::GetInstance()->GetEvent() == false)
		{
			//モーションの管理
			ActionState();
		}
	}
	else
	{
		//モーションの管理
		ActionState();
	}
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
//モーションと状態の管理
//====================================================================
void CDevil::ActionState(void)
{
	if (m_pMapMove != nullptr)
	{
		// 上モーション
		if (m_pMapMove->GetState() == CMapMove::MOVE_SCROLL_UP ||
			m_pMapMove->GetState() == CMapMove::MOVE_SLOPE_UP)
		{
			if (m_Action != ACTION_SIGNAL_UP)
			{
				m_Action = ACTION_SIGNAL_UP;
				m_nStateNum = ACTION_SIGNAL_UP;
				GetMotion()->Set(ACTION_SIGNAL_UP, 5);
			}
		}
		// 下モーション
		else if (m_pMapMove->GetState() == CMapMove::MOVE_SCROLL_DOWN ||
			m_pMapMove->GetState() == CMapMove::MOVE_SLOPE_DOWN)
		{
			if (m_Action != ACTION_SIGNAL_DOWN)
			{
				m_Action = ACTION_SIGNAL_DOWN;
				m_nStateNum = ACTION_SIGNAL_DOWN;
				GetMotion()->Set(ACTION_SIGNAL_DOWN, 5);
			}
		}
		// 左モーション
		else if (m_pMapMove->GetState() == CMapMove::MOVE_SCROLL_LEFT ||
			m_pMapMove->GetState() == CMapMove::MOVE_SLOPE_LEFT)
		{
			if (m_Action != ACTION_SIGNAL_LEFT)
			{
				m_Action = ACTION_SIGNAL_LEFT;
				m_nStateNum = ACTION_SIGNAL_LEFT;
				GetMotion()->Set(ACTION_SIGNAL_LEFT, 5);
			}
		}
		// 右モーション
		else if (m_pMapMove->GetState() == CMapMove::MOVE_SCROLL_RIGHT ||
			m_pMapMove->GetState() == CMapMove::MOVE_SLOPE_RIGHT)
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
