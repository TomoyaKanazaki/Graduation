//============================================
//
//	敵の処理 [enemy.cpp]
//	Author:sakamoto kai
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
#include "debugproc.h"
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
#include "sound.h"

#define COLLISION_SIZE (D3DXVECTOR3(5.0f,60.0f,5.0f))		//横の当たり判定
#define LIFE (30.0f)	// 体力

namespace
{
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
	m_AtkPos = INITVECTOR3;

	m_nSeatchCount = 0;
	m_bSeatchLoop = false;

	m_nActionCount = 0;
	m_nAttackHit = false;

	SetSize(COLLISION_SIZE);

	m_State = STATE_SEATCH;
	m_nStateCount = 0;
	m_bJump = false;
	m_bDeath = false;
	m_bAttack = false;

	m_fLife = LIFE;

	m_CollisionRot = 0.0f;

	m_ColorA = 1.0f;

	for (int nCnt = 0; nCnt < SCREEN_POS_NUM; nCnt++)
	{
		m_ScreenPos[nCnt] = D3DXVECTOR3(-100.0f, -100.0f, 0.0f);
	}

	for (int nCnt = 0; nCnt < SEARCH_POS_NUM; nCnt++)
	{
		m_SeatchPos[nCnt] = D3DXVECTOR3(0.0f, -10000.0f, 0.0f);
	}

	for (int nCnt = 0; nCnt < MODEL_NUM; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
		m_aModelName[nCnt] = "";
	}

	m_size = COLLISION_SIZE;
	m_SearchDistance = 0.0f;

	m_pMotion = nullptr;
	m_nNumModel = 0;
	m_EnemyType = ENEMY_ASHIGARU;

	m_pSlow = nullptr;
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
		m_apModel[nCntModel] = NULL;
	}

	//モーションの終了処理
	if (m_pMotion != NULL)
	{
		//モーションの破棄
		delete m_pMotion;
		m_pMotion = NULL;
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
	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	//スクリーン座標への変換処理
	ScreenCollision();

	// 過去の位置を記録
	m_posOld = m_pos;

	// 位置更新処理
	UpdatePos();

	if (m_bDeath == false)
	{
		UpdateState();
	}

	if (m_pos.y <= 0.0f)
	{
		m_bJump = false;
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}

	if (m_bDeath == true)
	{
		// 死亡処理
		Death();
	}

	if (m_pMotion != nullptr)
	{
		//モーションの更新
		m_pMotion->Update();
	}
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

	//ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_bDeath == true)
	{
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			m_apModel[nCntModel]->SetColorType(CModel::COLORTYPE_TRUE_A);
			m_apModel[nCntModel]->SetColorA(m_ColorA);
		}
	}
	else
	{
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			m_apModel[nCntModel]->SetColorType(CModel::COLORTYPE_FALSE);
		}
	}

	//モデルの描画(全パーツ)
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}
}

//====================================================================
// 接触ダメージ処理
//====================================================================
void CEnemy::HitDamage(float fDamage)
{
	m_fLife -= fDamage;

	if (m_State == STATE_SEATCH)
	{
		m_State = STATE_BATTLE;
	}

	if (m_fLife <= 0.0f && m_bDeath == false)
	{
		m_fLife = 0.0f;
		m_bAttack = false;

		if (m_State == STATE_BATTLE)
		{
			m_bJump = true;
			m_move.y = 8.0f;
		}

		switch (m_EnemyType)
		{
		case CEnemy::ENEMY_ASHIGARU:
			CGame::GetScore()->AddScore(500);
			break;

		case CEnemy::ENEMY_BOWMAN:
			CGame::GetScore()->AddScore(800);
			break;

		case CEnemy::ENEMY_SAMURAI:
			CGame::GetScore()->AddScore(1200);
			break;

		default:
			break;
		}

		m_bDeath = true;
	}
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
	if (m_pMotion == NULL)
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

	//死亡時の吹っ飛び
	if(m_bDeath == true && m_bJump == true)
	{
		m_move.x = sinf(m_rot.y + D3DX_PI * 0.5f) * 10.0f;
		m_move.z = cosf(m_rot.y + D3DX_PI * 0.5f) * 10.0f;
	}

	//減衰係数
	m_move.x = m_move.x * 0.5f;
	if (m_move.x <= 0.0001f && m_move.x >= -0.0001f)
	{
		m_move.x = 0.0f;
	}

	m_move.z = m_move.z * 0.5f;
	if (m_move.z <= 0.0001f && m_move.z >= -0.0001f)
	{
		m_move.z = 0.0f;
	}

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

	//X軸の位置更新
	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_X);

	//Z軸の位置更新
	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_Z);
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

		while (pObj != NULL)
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

				}
			}

			if (type == TYPE_CUBECOLL)
			{//種類がブロックの時

				CCubeColl* pBlock = (CCubeColl*)pObj;	// ブロック情報の取得

				D3DXVECTOR3 pos = pBlock->GetPos();
				D3DXVECTOR3 posOld = pBlock->GetPosOld();
				D3DXVECTOR3 Move = pBlock->GetMove();
				D3DXVECTOR3 Size = pBlock->GetSize();

				// 矩形の当たり判定
				if (useful::CollisionBlock(pos, posOld, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
				{

				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//プレイヤー索敵処理
//====================================================================
void CEnemy::SearchPlayer(void)
{
	bool SetBattle = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_PLAYER3D)
			{//種類が敵の時
				CPlayer* pPlayer = (CPlayer*)pObj;

				if (useful::CollisionCircle(m_pos, pPlayer->GetPos(), m_SearchDistance) == true)
				{
					if (useful::CollisionLine(m_pos, pPlayer->GetPos(), m_rot.y - D3DX_PI * 0.25f) == true &&
						useful::CollisionLine(m_pos, pPlayer->GetPos(), m_rot.y + D3DX_PI * 0.25f) == false)
					{
						SetBattle = true;

						CManager::GetInstance()->GetDebugProc()->Print("索敵範囲内！\n");

						if (SortObject(pPlayer->GetPos()) == true)
						{
							SetBattle = false;

							CManager::GetInstance()->GetDebugProc()->Print("間に壁がある！\n");
						}
					}
				}
			}

			pObj = pObjNext;
		}
	}

	if (SetBattle == true)
	{
		// 戦闘状態
		m_State = STATE_BATTLE;

		// 回避の音
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BATTLE_ENTRY);

	}
}

//====================================================================
//マップオブジェクトと壁とのソート処理
//====================================================================
bool CEnemy::SortObject(D3DXVECTOR3 pos)
{
	float fDistance = sqrtf((m_pos.x - pos.x) * (m_pos.x - pos.x) + (m_pos.z - pos.z) * (m_pos.z - pos.z));

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_CUBEBLOCK)
			{//種類がブロックの時
				CCubeBlock* pBlock = (CCubeBlock*)pObj;

				if (pBlock->GetPos().y <= 200.0f)
				{
					if (useful::CollisionCircle(m_pos, pBlock->GetPos(), m_SearchDistance) == true)
					{
						if (useful::CollisionLine(m_pos, pBlock->GetPos(), m_rot.y - D3DX_PI * 0.50f) == true &&
							useful::CollisionLine(m_pos, pBlock->GetPos(), m_rot.y + D3DX_PI * 0.50f) == false)
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
										m_SearchDistance);
									break;

								case 1:
									CrossPos = useful::CrossIntersection(m_pos, pos,
										D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
										D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
										m_SearchDistance);
									break;

								case 2:
									CrossPos = useful::CrossIntersection(m_pos, pos,
										D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
										D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
										m_SearchDistance);
									break;

								case 3:
									CrossPos = useful::CrossIntersection(m_pos, pos,
										D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
										D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
										m_SearchDistance);
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
			}

			pObj = pObjNext;
		}
	}
	return false;
}

//====================================================================
//プレイヤーと間合いを取る処理
//====================================================================
bool CEnemy::DistancePlayer(void)
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

	bool bOK = false;

	m_rot.y = useful::PosDirection(m_pos, pPlayer->GetPos(), 0.0f);

	if (useful::CollisionCircle(m_pos, pPlayer->GetPos(), DISTANCE_RECEDE) == false)
	{
		m_move.x = sinf(D3DX_PI * -0.5f + m_rot.y) * 6.0f;
		m_move.z = cosf(D3DX_PI * -0.5f + m_rot.y) * 6.0f;
	}
	else if (useful::CollisionCircle(m_pos, pPlayer->GetPos(), DISTANCE_APPROACH) == true)
	{
		m_move.x = -sinf(D3DX_PI * -0.5f + m_rot.y) * 6.0f;
		m_move.z = -cosf(D3DX_PI * -0.5f + m_rot.y) * 6.0f;
	}
	else
	{
		bOK = true;
	}
	return bOK;
}

//====================================================================
//自分以外の敵を押し出す処理
//====================================================================
void CEnemy::CollisionPush(void)
{
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
				CEnemy* pEnemy = (CEnemy*)pObj;

				if (useful::CollisionCircle(m_pos, pEnemy->GetPos(), 30.0f) == true)
				{
					fAngle = atan2f(pEnemy->GetPos().z - m_pos.z, m_pos.x - pEnemy->GetPos().x) + D3DX_PI * 0.5f;
					m_move.x = sinf(fAngle) * 0.2f;
					m_move.z = cosf(fAngle) * 0.2f;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//スクリーン座標に変換する処理
//====================================================================
void CEnemy::ScreenCollision()
{
	//頂点設定
	D3DXVECTOR3 pos[SCREEN_POS_NUM] = { INITVECTOR3 };

	pos[0] = D3DXVECTOR3(GetPos().x - m_size.x, GetPos().y, GetPos().z + m_size.z);
	pos[1] = D3DXVECTOR3(GetPos().x - m_size.x, GetPos().y + m_size.y, GetPos().z + m_size.z);
	pos[2] = D3DXVECTOR3(GetPos().x + m_size.x, GetPos().y, GetPos().z + m_size.z);
	pos[3] = D3DXVECTOR3(GetPos().x + m_size.x, GetPos().y + m_size.y, GetPos().z + m_size.z);
	pos[4] = D3DXVECTOR3(GetPos().x - m_size.x, GetPos().y, GetPos().z - m_size.z);
	pos[5] = D3DXVECTOR3(GetPos().x - m_size.x, GetPos().y + m_size.y, GetPos().z - m_size.z);
	pos[6] = D3DXVECTOR3(GetPos().x + m_size.x, GetPos().y, GetPos().z - m_size.z);
	pos[7] = D3DXVECTOR3(GetPos().x + m_size.x, GetPos().y + m_size.y, GetPos().z - m_size.z);

	//ビューポートの設定
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;

	//マトリックスの取得
	D3DXMATRIX ViewMatrix = CManager::GetInstance()->GetCamera()->GetViewMatrix();
	D3DXMATRIX ProjectionMatrix = CManager::GetInstance()->GetCamera()->GetProjectionMatrix();

	//D3DXVec3Project(&screenPosition, &pos, &viewport, &projectionMatrix, &viewMatrix, D3DXMatrixIdentity(&mtx));

	//3Dの座標をスクリーン座標に変換
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		D3DXVec3Project(&m_ScreenPos[nCnt], &pos[nCnt], &viewport, &ProjectionMatrix, &ViewMatrix, D3DXMatrixIdentity(&m_mtxWorld));

		//C2DEffect* pEffect = C2DEffect::Create();
		//pEffect->SetPos(m_ScreenPos[nCnt]);
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
void CEnemy::UpdateState()
{
	//状態の管理
	switch (m_State)
	{
	case CEnemy::STATE_BATTLE:

		//戦闘状態の管理
		BattleStateManager();

		break;

	case CEnemy::STATE_SEATCH:

		//探索状態の管理
		SearchStateManager();

		//プレイヤーの索敵
		SearchPlayer();

		break;

	default:
		break;
	}
}

//====================================================================
// 武器の残像エフェクトを生成する
//====================================================================
void CEnemy::CreateWeaponEffect(int ModelNumber)
{
	CModelEffect *pEffect = CModelEffect::Create(m_apModel[ModelNumber]->GetModelName());
	pEffect->SetMtxWorld(m_apModel[ModelNumber]->GetMtxWorld());
	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}

//====================================================================
//ロード処理
//====================================================================
void CEnemy::LoadLevelData(const char* pFilename)
{
	FILE* pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(pFilename, "r");

	if (pFile != NULL)
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
									m_apModel[nCntModel]->SetParent(NULL);
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

