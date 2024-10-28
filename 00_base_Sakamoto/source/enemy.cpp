//============================================
//
//	敵の処理 [enemy.cpp]
//	Author: sakamoto kai
//
//============================================
#include "enemy.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"
#include "CubeBlock.h"
#include "CubeColl.h"
#include "LevelModelEffect.h"
#include "slowManager.h"
#include "2DEffect.h"
#include "score.h"
#include "modelEffect.h"
#include "Effect.h"
#include "devil.h"
#include "DevilHole.h"
#include "MapSystem.h"
#include "objmeshField.h"
#include "sound.h"

namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(45.0f, 60.0f, 45.0f);		//横の当たり判定
	const float LIFE = 1.0f;	// 体力

	const float DISTANCE_RECEDE = 200.0f;	//近づく距離
	const float DISTANCE_APPROACH = 100.0f;	//遠ざかる距離
}

//====================================================================
//コンストラクタ
//====================================================================
CEnemy::CEnemy(int nPriority) :CObject(nPriority)
{
	m_pos = INITVECTOR3;
	m_posOld = INITVECTOR3;

	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_nActionCount = 0;
	SetSize(COLLISION_SIZE);
	m_nStateCount = 0;

	m_ColorA = 1.0f;

	for (int nCnt = 0; nCnt < MODEL_NUM; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
		m_aModelName[nCnt] = "";
	}

	m_size = COLLISION_SIZE;
	m_pMotion = nullptr;
	m_nNumModel = 0;
	m_EnemyType = ENEMY_MEDAMAN;
	m_State = E_STATE_WAIT;
	m_pSlow = nullptr;
	m_SelectMove = SELECT_MOVE_MAX;

	m_OKL = true;
	m_OKR = true;
	m_OKU = true;
	m_OKD = true;

	m_nMapWight = 0;
	m_nMapHeight = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CEnemy::~CEnemy()
{

}

//====================================================================
//生成処理
//====================================================================
CEnemy* CEnemy::Create(char* aModelName)
{
	// 生成
	CEnemy* pInstance = new CEnemy();

	//初期化処理
	if (FAILED(pInstance->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}
	
	// モデル関連初期化処理
	pInstance->InitModel(aModelName);

	return pInstance;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CEnemy::Init(void)
{
	//種類設定
	SetType(CObject::TYPE_ENEMY3D);

	//マップとのマトリックスの掛け合わせをオンにする
	SetMultiMatrix(true);

	// スローの生成(配属、タグの設定)
	m_pSlow = CSlowManager::Create(m_pSlow->CAMP_ENEMY, m_pSlow->TAG_ENEMY);

	return S_OK;
}

//====================================================================
//自分が保持するオブジェクトの生成
//====================================================================
void CEnemy::MyObjCreate(void)
{

}

//====================================================================
//終了処理
//====================================================================
void CEnemy::Uninit(void)
{
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
void CEnemy::Update(void)
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
void CEnemy::TitleUpdate(void)
{
	//モーションの更新
	m_pMotion->Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CEnemy::GameUpdate(void)
{
	// 過去の位置を記録
	m_posOld = m_pos;

	//壁の索敵判定
	SearchWall();

	// 状態の更新
	StateManager();

	// 移動方向処理
	Rot();

	// 位置更新処理
	UpdatePos();

	// 自分の番号を設定
	MapSystemNumber();

	//床の判定
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}

	if (m_pMotion != nullptr)
	{
		//モーションの更新
		m_pMotion->Update();
	}

	DebugProc::Print(DebugProc::POINT_LEFT, "[敵]横 %d : 縦 %d\n", m_nMapWight, m_nMapHeight);
}

//====================================================================
//描画処理
//====================================================================
void CEnemy::Draw(void)
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

	if (m_bMultiMatrix)
	{
		SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());

		//算出したマトリクスをかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&m_UseMultiMatrix);
	}

	//ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデルの描画(全パーツ)
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}
}

//====================================================================
// ヒット処理
//====================================================================
bool CEnemy::Hit(int nLife)
{
	m_nLife -= nLife;

	if (m_nLife < 0)
	{// 体力0以下
		Uninit();
	}

	return true;
}

//====================================================================
// モデル関連の初期化処理
//====================================================================
void CEnemy::InitModel(const char* pFilename)
{
	strcpy(&m_cFileName[0], pFilename);

	//モデルの生成
	LoadLevelData(pFilename);

	//モーションの生成
	if (m_pMotion == nullptr)
	{
		//モーションの生成
		m_pMotion = new CMotion;
	}

	//初期化処理
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData(pFilename);
}

//====================================================================
// 位置更新処理
//====================================================================
void CEnemy::UpdatePos(void)
{
	//重力
	m_move.y -= 0.5f;

	// 変数宣言
	float fSpeed = 1.0f;	// スロー用 default1.0fで初期化
	if (m_pSlow)
	{
		fSpeed = m_pSlow->GetValue();

		if (m_pMotion)
		{
			m_pMotion->SetSlowVaule(fSpeed);
		}
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

	//ステージ外との当たり判定
	CollisionOut();
}

//====================================================================
//移動方向処理
//====================================================================
void CEnemy::Rot(void)
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

	m_rot.y = atan2f(m_move.z, -m_move.x);

	useful::NormalizeAngle(&m_rot);
}

//====================================================================
// 壁との当たり判定
//====================================================================
void CEnemy::CollisionWall(useful::COLLISION XYZ)
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
				bool bNullJump;

				// 矩形の当たり判定
				if (useful::CollisionBlock(pos, posOld, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &bNullJump, XYZ) == true)
				{
					m_State = E_STATE_WAIT;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// マップモデルの当たり判定
//====================================================================
void CEnemy::CollisionDevilHole(useful::COLLISION XYZ)
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
				bool bNullJump;

				// 矩形の当たり判定
				if (useful::CollisionBlock(pos, posOld, INITVECTOR3, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &bNullJump, XYZ) == true)
				{
					//待機状態にする
					m_State = E_STATE_WAIT;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// ステージ外との当たり判定
//====================================================================
void CEnemy::CollisionOut()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_DEVIL)
			{//種類がブロックの時

				CDevil* pDevil = (CDevil*)pObj;	// ブロック情報の取得

				D3DXVECTOR3 Pos = pDevil->GetDevilPos();
				D3DXVECTOR3 Size = pDevil->GetDevilSize();

				// ステージ外の当たり判定
				if (Pos.x + Size.x < m_pos.x)
				{
					m_pos.x = -Size.x + Pos.x;
				}
				if (Pos.x - Size.x > m_pos.x)
				{
					m_pos.x = Size.x + Pos.x;
				}
				if (Pos.z + Size.z < m_pos.z)
				{
					m_pos.z = -Size.z + Pos.z;
				}
				if (Pos.z - Size.z > m_pos.z)
				{
					m_pos.z = Size.z + Pos.z;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// 死亡処理
//====================================================================
void CEnemy::Death(void)
{
	if (m_pMotion->GetFinish() == true)
	{
		m_ColorA -= 0.005f;

		if (m_ColorA <= 0.0f)
		{
			Uninit();
		}
	}
}

//====================================================================
// 状態更新
//====================================================================
void CEnemy::StateManager()
{
	int nRand = 0;

	//状態の管理
	switch (m_State)
	{
	case CEnemy::E_STATE_WAIT:

		MoveSelect();
		m_State = E_STATE_WALK;

		break;

	case CEnemy::E_STATE_TRUN:

		m_move.x = m_move.x * -1.0f;
		m_move.z = m_move.z * -1.0f;

		break;

	case CEnemy::E_STATE_WALK:
		break;

	case CEnemy::E_STATE_EGG:
		break;

	case CEnemy::E_STATE_DEATH:

		m_move.x = 0.0f;
		m_move.z = 0.0f;

		break;

	default:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// 移動方向の選択
//====================================================================
void CEnemy::MoveSelect()
{
	float OKRot[4];
	int RotNumber = 0;

	if (m_OKL && m_SelectMove != SELECT_MOVE_RIGHT)
	{
		OKRot[RotNumber] = D3DX_PI * -0.5f;
		RotNumber++;
	}
	if (m_OKR && m_SelectMove != SELECT_MOVE_LEFT)
	{
		OKRot[RotNumber] = D3DX_PI * 0.5f;
		RotNumber++;
	}
	if (m_OKU && m_SelectMove != SELECT_MOVE_DOWN)
	{
		OKRot[RotNumber] = D3DX_PI * 0.0f;
		RotNumber++;
	}
	if (m_OKD && m_SelectMove != SELECT_MOVE_UP)
	{
		OKRot[RotNumber] = D3DX_PI * 1.0f;
		RotNumber++;
	}

	if (RotNumber != 0)
	{
		int nRand = rand() % RotNumber;

		m_move.x = sinf(OKRot[nRand]) * 3.0f;
		m_move.z = cosf(OKRot[nRand]) * 3.0f;

		if (m_move.x >= 3.0f)
		{
			m_SelectMove = SELECT_MOVE_RIGHT;
		}
		else if (m_move.x <= -3.0f)
		{
			m_SelectMove = SELECT_MOVE_LEFT;
		}
		else if (m_move.z >= 3.0f)
		{
			m_SelectMove = SELECT_MOVE_UP;
		}
		else if (m_move.z <= -3.0f)
		{
			m_SelectMove = SELECT_MOVE_DOWN;
		}
	}
}

//====================================================================
// 壁との当たり判定
//====================================================================
void CEnemy::SearchWall(void)
{
	bool OKR = true;	//右
	bool OKL = true;	//左
	bool OKU = true;	//上
	bool OKD = true;	//下

	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	int nMapWightMax = pMapSystem->GetWightMax();
	int nMapHeightMax = pMapSystem->GetHeightMax();
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

	int nRNumber = m_nMapWight + 1;
	int nLNumber = m_nMapWight - 1;
	int nUNumber = m_nMapHeight - 1;
	int nDNumber = m_nMapHeight + 1;

	nRNumber = useful::RangeNumber(nMapWightMax, 0, nRNumber);
	nLNumber = useful::RangeNumber(nMapWightMax, 0, nLNumber);
	nUNumber = useful::RangeNumber(nMapHeightMax, 0, nUNumber);
	nDNumber = useful::RangeNumber(nMapHeightMax, 0, nDNumber);

	OKR = !pMapSystem->GetGritBool(nRNumber, m_nMapHeight);
	OKL = !pMapSystem->GetGritBool(nLNumber, m_nMapHeight);
	OKU = !pMapSystem->GetGritBool(m_nMapWight, nUNumber);
	OKD = !pMapSystem->GetGritBool(m_nMapWight, nDNumber);

	//自分の立っているグリットの中心位置を求める
	D3DXVECTOR3 MyGritPos = CMapSystem::GetInstance()->GetGritPos(m_nMapWight, m_nMapHeight);;
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "ああああ %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if (m_pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - m_size.x) &&
		m_pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - m_size.x) &&
		m_pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - m_size.z) &&
		m_pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - m_size.z))
	{// グリットの中心位置に立っているなら操作を受け付ける

		if (!m_OKR && OKR)
		{
			m_State = E_STATE_WAIT;
		}
		if (!m_OKL && OKL)
		{
			m_State = E_STATE_WAIT;
		}
		if (!m_OKU && OKU)
		{
			m_State = E_STATE_WAIT;
		}
		if (!m_OKD && OKD)
		{
			m_State = E_STATE_WAIT;
		}

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

	//bool OKR = true;	//右
	//bool OKL = true;	//左
	//bool OKU = true;	//上
	//bool OKD = true;	//下

	//for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	//{
	//	//オブジェクトを取得
	//	CObject* pObj = CObject::GetTop(nCntPriority);

	//	while (pObj != nullptr)
	//	{
	//		CObject* pObjNext = pObj->GetNext();

	//		CObject::TYPE type = pObj->GetType();			//種類を取得

	//		if (type == TYPE_CUBEBLOCK)
	//		{//種類がブロックの時

	//			CCubeBlock* pBlock = (CCubeBlock*)pObj;	// ブロック情報の取得

	//			D3DXVECTOR3 pos = pBlock->GetPos();
	//			D3DXVECTOR3 posOld = pBlock->GetPosOld();
	//			D3DXVECTOR3 Move = pBlock->GetMove();
	//			D3DXVECTOR3 Size = pBlock->GetSize();

	//			D3DXVECTOR3 MyPos = INITVECTOR3;

	//			for (int nCnt = 0; nCnt < 4; nCnt++)
	//			{
	//				switch (nCnt)
	//				{
	//				case 0:
	//					MyPos = D3DXVECTOR3(m_pos.x + Size.x * 2.0f, m_pos.y, m_pos.z);	//右
	//					break;
	//				case 1:
	//					MyPos = D3DXVECTOR3(m_pos.x - Size.x * 2.0f, m_pos.y, m_pos.z);	//左
	//					break;
	//				case 2:
	//					MyPos = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + Size.z * 2.0f);	//上
	//					break;
	//				case 3:
	//					MyPos = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z - Size.z * 2.0f);	//下
	//					break;
	//				}

	//				// 矩形の当たり判定
	//				if (useful::CollisionRectangle2D(MyPos, pos, COLLISION_SIZE, Size, useful::COLLISION::COLLISION_ZX) == true)
	//				{
	//					switch (nCnt)
	//					{
	//					case 0:
	//						OKR = false;
	//						break;
	//					case 1:
	//						OKL = false;
	//						break;
	//					case 2:
	//						OKU = false;
	//						break;
	//					case 3:
	//						OKD = false;
	//						break;
	//					}

	//					CEffect* pEffect = CEffect::Create();
	//					pEffect->SetPos(MyPos);
	//				}
	//			}
	//		}

	//		if (type == TYPE_DEVILHOLE)
	//		{//種類がデビルホールの時

	//			CDevilHole* pDevilHole = (CDevilHole*)pObj;	// ブロック情報の取得

	//			D3DXVECTOR3 pos = pDevilHole->GetPos();
	//			D3DXVECTOR3 Size = pDevilHole->GetSize();

	//			D3DXVECTOR3 MyPos = INITVECTOR3;

	//			for (int nCnt = 0; nCnt < 4; nCnt++)
	//			{
	//				switch (nCnt)
	//				{
	//				case 0:
	//					MyPos = D3DXVECTOR3(m_pos.x + Size.x * 2.0f, m_pos.y, m_pos.z);	//右
	//					break;
	//				case 1:
	//					MyPos = D3DXVECTOR3(m_pos.x - Size.x * 2.0f, m_pos.y, m_pos.z);	//左
	//					break;
	//				case 2:
	//					MyPos = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + Size.z * 2.0f);	//上
	//					break;
	//				case 3:
	//					MyPos = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z - Size.z * 2.0f);	//下
	//					break;
	//				}

	//				// 矩形の当たり判定
	//				if (useful::CollisionRectangle2D(MyPos, pos, COLLISION_SIZE, Size, useful::COLLISION::COLLISION_ZX) == true)
	//				{
	//					switch (nCnt)
	//					{
	//					case 0:
	//						OKR = false;
	//						break;
	//					case 1:
	//						OKL = false;
	//						break;
	//					case 2:
	//						OKU = false;
	//						break;
	//					case 3:
	//						OKD = false;
	//						break;
	//					}

	//					CEffect* pEffect = CEffect::Create();
	//					pEffect->SetPos(MyPos);
	//				}
	//			}
	//		}

	//		pObj = pObjNext;
	//	}
	//}

	//if (!m_OKR && OKR)
	//{
	//	m_State = E_STATE_WAIT;
	//}
	//if (!m_OKL && OKL)
	//{
	//	m_State = E_STATE_WAIT;
	//}
	//if (!m_OKU && OKU)
	//{
	//	m_State = E_STATE_WAIT;
	//}
	//if (!m_OKD && OKD)
	//{
	//	m_State = E_STATE_WAIT;
	//}

	//m_OKR = OKR;	//右
	//m_OKL = OKL;	//左
	//m_OKU = OKU;	//上
	//m_OKD = OKD;	//下
}

//====================================================================
// マップのどのマスに存在しているか設定する
//====================================================================
void CEnemy::MapSystemNumber(void)
{
	m_nMapWight = CMapSystem::GetInstance()->GetGritWightNumber(m_pos.x);
	m_nMapHeight = CMapSystem::GetInstance()->GetGritHeightNumber(m_pos.z);
}

//====================================================================
//ロード処理
//====================================================================
void CEnemy::LoadLevelData(const char* pFilename)
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
						m_apModel[nCntModel]->SetColorType(CModel::COLORTYPE_FALSE);
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

