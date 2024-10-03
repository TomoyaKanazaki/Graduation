//============================================
//
//	足軽の処理 [enemyAshigaru.cpp]
//	Author:酒井 南勝
//
//============================================
#include "LevelModelEffect.h"
#include "enemyAshigaru.h"
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
#include "player.h"
#include "useful.h"
#include "effect.h"

#define MOVE_SPEED (1.0f)
#define ASHIGARU_SEARCHDISTANCE (700.0f)	//	足軽の索敵距離

//====================================================================
//コンストラクタ
//====================================================================
CEnemyAshigaru::CEnemyAshigaru(int nPriority) : CEnemy(nPriority)
{
	m_Action = ACTION_BWAIT;
	m_AtkAction = ACTION_BWAIT;

	m_SState = SEARCHSTATE(0);
	m_BState = BATTLESTATE(0);
}

//====================================================================
//デストラクタ
//====================================================================
CEnemyAshigaru::~CEnemyAshigaru()
{

}

//====================================================================
//生成処理
//====================================================================
CEnemyAshigaru* CEnemyAshigaru::Create(const char* pFilename)
{
	// 生成
	CEnemyAshigaru* pInstance = new CEnemyAshigaru();

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	// モデル関連の初期化
	pInstance->InitModel(pFilename);

	return pInstance;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CEnemyAshigaru::Init(void)
{
	// 継承クラスの初期化処理
	CEnemy::Init();

	SetEnemyType(CEnemy::ENEMY_ASHIGARU);	//敵の種類設定
	SetSearchDistance(700.0f);				//索敵距離設定
	SetLife(30.0f);							//ライフの設定

	m_BState = CEnemyAshigaru::BATTLESTATE_WALK;

	return S_OK;
}

//====================================================================
//自分が保持するオブジェクトの生成
//====================================================================
void CEnemyAshigaru::MyObjCreate(void)
{

}

//====================================================================
//終了処理
//====================================================================
void CEnemyAshigaru::Uninit(void)
{
	// 継承クラスの終了処理
	CEnemy::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CEnemyAshigaru::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//タイトルでの更新処理
//====================================================================
void CEnemyAshigaru::TitleUpdate(void)
{
	// 継承クラスのタイトル更新処理
	CEnemy::TitleUpdate();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CEnemyAshigaru::GameUpdate(void)
{
	// 継承クラスのゲーム更新処理
	CEnemy::GameUpdate();

	//モーションの管理
	ActionState();
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CEnemyAshigaru::ActionState(void)
{
	// 情報取得
	D3DXVECTOR3 move = GetMove();	// 移動量

	switch (GetState())
	{
	case CEnemy::STATE_BATTLE:

		//死亡モーション
		if (GetDeath() == true)
		{
			if (m_Action != ACTION_BDEATH)
			{
				m_Action = ACTION_BDEATH;
				GetMotion()->Set(ACTION_BDEATH, 10);
			}
		}
		//攻撃モーション
		else if (m_BState == BATTLESTATE_ATTACK)
		{
			if (m_Action != ACTION_ATTACK)
			{
				m_Action = ACTION_ATTACK;
				GetMotion()->Set(ACTION_ATTACK, 0);
			}
		}
		//ダメージモーション
		else if (m_BState == BATTLESTATE_DAMAGE)
		{
			if (m_Action != ACTION_DAMAGE)
			{
				m_Action = ACTION_DAMAGE;
				GetMotion()->Set(ACTION_DAMAGE, 0);
			}

			if (GetMotion()->GetFinish() == true)
			{
				m_BState = BATTLESTATE_WALK;
			}
		}
		//移動モーション
		else if (move.x > 0.1f || move.x < -0.1f || move.z > 0.1f || move.z < -0.1f)
		{
			if (m_Action != ACTION_BMOVE)
			{
				m_Action = ACTION_BMOVE;
				GetMotion()->Set(ACTION_BMOVE, 5);
			}
		}
		//ニュートラルモーション
		else
		{
			if (m_Action != ACTION_BWAIT)
			{
				m_Action = ACTION_BWAIT;
				GetMotion()->Set(ACTION_BWAIT, 5);
			}
		}
		break;

	case CEnemy::STATE_SEATCH:

		//死亡モーション
		if (GetDeath() == true)
		{
			if (m_Action != ACTION_SDEATH)
			{
				m_Action = ACTION_SDEATH;
				GetMotion()->Set(ACTION_SDEATH, 30);
			}
		}
		//移動モーション
		else if (move.x > 0.1f || move.x < -0.1f || move.z > 0.1f || move.z < -0.1f)
		{
			if (m_Action != ACTION_SMOVE)
			{
				m_Action = ACTION_SMOVE;
				GetMotion()->Set(ACTION_SMOVE, 5);
			}
		}
		//ニュートラルモーション
		else
		{
			if (m_Action != ACTION_SWAIT)
			{
				m_Action = ACTION_SWAIT;
				GetMotion()->Set(ACTION_SWAIT, 5);
			}
		}

		break;
	default:
		break;
	}
}

//====================================================================
//戦闘状態管理
//====================================================================
void CEnemyAshigaru::BattleStateManager(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();

	switch (CScene::GetMode())
	{
	case CScene::MODE_GAME:

		pPlayer = CGame::GetPlayer();

		break;
	case CScene::MODE_TUTORIAL:

		pPlayer = CTutorial::GetPlayer();

		break;
	}

	D3DXVECTOR3 fRot = GetRot();
	CModel** pModel;

	switch (m_BState)
	{
	case BATTLESTATE_NORMAL:
		break;

	case BATTLESTATE_DEATH:
		break;

	case BATTLESTATE_WAIT:

		// 敵押し出し処理
		CEnemy::CollisionPush();

		//敵の向き修正
		fRot.y = useful::PosDirection(GetPos(), pPlayer->GetPos(), 0.0f);

		SetRot(fRot);

		if (m_nStateCount <= 0)
		{
			m_BState = BATTLESTATE_ATTACK;
			m_nStateCount = 1000;
			SetAttack(true);
		}

		break;

	case BATTLESTATE_ATTACK:

		pModel = GetLevelModel();
		pModel[17]->SetColor(D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f));
		pModel[17]->SetColorType(CModel::COLORTYPE_TRUE_ALL);

		if (m_nStateCount % 5 == 0)
		{
			CreateWeaponEffect(CEnemy::GetNumModel() - 1);
		}

		if (GetMotion()->GetFinish() == true)
		{
			pModel[17]->SetColorType(CModel::COLORTYPE_FALSE);

			SetAttack(false);
			SetDamage(10.0f);
			m_BState = BATTLESTATE_WAIT;
			m_nStateCount = 120;
		}

		break; 

	case BATTLESTATE_DAMAGE:

		break;

	case BATTLESTATE_WALK:

		// 敵押し出し処理
		CEnemy::CollisionPush();

		// プレイヤーと間合いを取る処理
		if (CEnemy::DistancePlayer() == true)
		{
			m_BState = BATTLESTATE_WAIT;
			m_nStateCount = 30;
		}

		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//探索状態管理
//====================================================================
void CEnemyAshigaru::SearchStateManager(void)
{
	switch (m_SState)
	{
	case SEARCHSTATE_NORMAL:
		break;

	case SEARCHSTATE_WALK:

		// 目的地に歩く
		WalkPoint();

		break;

	case SEARCHSTATE_TRUN:

		// 目的地から回転する
		TurnPoint();

		break;

	case SEARCHSTATE_WAIT:
		break;

	case SEARCHSTATE_SEARCH:
		break;
	}
}

//====================================================================
//ダメージ判定処理
//====================================================================
void CEnemyAshigaru::SetDamage(float Damage)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 AttackPos[3] = { INITVECTOR3 };

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		AttackPos[nCnt].x = pos.x + sinf(rot.y + D3DX_PI * -0.5f) * 70.0f * (nCnt + 1);
		AttackPos[nCnt].y = pos.y;
		AttackPos[nCnt].z = pos.z + cosf(rot.y + D3DX_PI * -0.5f) * 70.0f * (nCnt + 1);


		if (SortObject(AttackPos[nCnt]) == false)
		{
			CEffect* pEffect = CEffect::Create();
			pEffect->SetPos(D3DXVECTOR3(AttackPos[nCnt].x, AttackPos[nCnt].y + 50.0f, AttackPos[nCnt].z));
			pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			pEffect->SetLife(120);
			pEffect->SetRadius(20.0f);
			pEffect->SetDel(0.01f);
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得
			float fAngle = 0.0f;

			if (type == TYPE_PLAYER3D)
			{//種類がブロックの時
				CPlayer* pPlayer = (CPlayer*)pObj;

				for (int nCnt = 0; nCnt < 3; nCnt++)
				{
					if (useful::CollisionCircle(AttackPos[nCnt], pPlayer->GetPos(), 35.0f) == true)
					{
						if (SortObject(pPlayer->GetPos()) == false)
						{
							pPlayer->HitDamage(Damage);
						}
					}
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// 目的位置に歩く処理
//====================================================================
void CEnemyAshigaru::WalkPoint(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3* searchPos = GetSeatchPos();
	int nSeatchCount = GetSeatchCount();

	SetMove(D3DXVECTOR3(sinf(D3DX_PI * -0.5f + rot.y) * MOVE_SPEED,0.0f,cosf(D3DX_PI * -0.5f + rot.y) * MOVE_SPEED));

	if (searchPos[nSeatchCount + 1].y <= -10000.0f)
	{
		if (useful::CollisionCircle(pos, searchPos[0], 5.0f) == true)
		{
			m_SState = SEARCHSTATE_TRUN;
			SetSeatchCount(0);
		}
	}
	else
	{
		if (useful::CollisionCircle(pos, searchPos[nSeatchCount + 1], 5.0f) == true)
		{
			m_SState = SEARCHSTATE_TRUN;
			SetSeatchCount(nSeatchCount + 1);
		}
	}

	CManager::GetInstance()->GetDebugProc()->Print("[敵] %f : %f : %f\n", pos.x, pos.y, pos.z);
}

//====================================================================
// 目的地から方向転換処理
//====================================================================
void CEnemyAshigaru::TurnPoint(void)
{
	float fAngle = 0.0f;

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3* searchPos = GetSeatchPos();
	int nSeatchCount = GetSeatchCount();
	bool bSeatchLoop = GetSeatchLoop();

	if (searchPos[nSeatchCount + 1].y <= -10000.0f)
	{
		if (bSeatchLoop == true)
		{
			fAngle = atan2f(searchPos[0].z - pos.z, pos.x - searchPos[0].x);

			if (rot.y >= fAngle)
			{
				rot.y -= 0.04f;

				if (rot.y < fAngle)
				{
					rot.y = fAngle;
					m_SState = SEARCHSTATE_WALK;
				}
			}
			if (rot.y <= fAngle)
			{
				rot.y += 0.04f;

				if (rot.y > fAngle)
				{
					rot.y = fAngle;
					m_SState = SEARCHSTATE_WALK;
				}
			}
			SetRot(rot);
		}
		else
		{
			m_SState = SEARCHSTATE_WAIT;
		}

	}
	else
	{
		fAngle = atan2f(searchPos[nSeatchCount + 1].z - pos.z, pos.x - searchPos[nSeatchCount + 1].x);

		if (rot.y >= fAngle)
		{
			rot.y -= 0.04f;

			if (rot.y < fAngle)
			{
				rot.y = fAngle;
				m_SState = SEARCHSTATE_WALK;
			}
		}
		if(rot.y <= fAngle)
		{
			rot.y += 0.04f;

			if (rot.y > fAngle)
			{
				rot.y = fAngle;
				m_SState = SEARCHSTATE_WALK;
			}
		}
		SetRot(rot);
	}
}

//====================================================================
//自分以外の敵を押し出す処理
//====================================================================
void CEnemyAshigaru::CollisionPush(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3* screenPos = GetScreenPos();
	int nSeatchCount = GetSeatchCount();
	bool bSeatchLoop = GetSeatchLoop();

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得
			float fAngle = 0.0f;

			if (type == TYPE_ENEMY3D && pObj != this)
			{//種類がブロックの時
				CEnemyAshigaru* pEnemy = (CEnemyAshigaru*)pObj;

				if (useful::CollisionCircle(pos, pEnemy->GetPos(), 30.0f) == true)
				{
					fAngle = atan2f(pEnemy->GetPos().z - pos.z, pos.x - pEnemy->GetPos().x) + D3DX_PI * 0.5f;

					move.x = sinf(fAngle) * 0.2f;
					move.z = cosf(fAngle) * 0.2f;

					SetMove(move);
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//描画処理
//====================================================================
void CEnemyAshigaru::Draw(void)
{
	// 継承クラスの描画処理
	CEnemy::Draw();
}

//====================================================================
// 接触ダメージ処理
//====================================================================
void CEnemyAshigaru::HitDamage(float fDamage)
{
	CEnemy::HitDamage(fDamage);

	if (GetLife() > 0 && m_Action != ACTION_ATTACK)
	{
		m_BState = BATTLESTATE_DAMAGE;
		m_nStateCount = 1000;
	}
}