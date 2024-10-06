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

#define COLLISION_SIZE (D3DXVECTOR3(20.0f,40.0f,20.0f))		//横の当たり判定
#define PLAYER_SPEED (0.0f)		//プレイヤーの移動速度

namespace
{
	const float MAX_LIFE = 100.0f;			//体力最大値
	const float EVASION_MOVE = 10.0f;		//回避移動量
	const float EVASION_ROT = 0.4f;			//回避角度
	const int EVASION_FRAME = 7;			//回避フレーム
	const int EVASION_FRAME_JUST = 3;			//ジャスト回避フレーム
	const int INVINCIBLE_FRAME = 20;			//無敵フレーム

	const int STAMINA_COUNT = 30;					//スタミナ消費から回復以降フレーム
	
	const float DISTANCE_RECEDE = 200.0f;	//近づく距離
	const float DISTANCE_APPROACH = 150.0f;	//遠ざかる距離
	const float DISTANCE_APPROACH_BOSS = 150.0f;	//遠ざかる距離(ボス)

	const float ULTIMATE_SLOW = 0.0f;		//術使用時のスロー値
	const float TRACKING_LENGTH = 720.0f;	//戦闘に入る距離

	const float ROT_SPEED = 0.015f;				// 向き速度
	const float ROT_HEIGHT_UP_MAX = 0.50f;		// 縦軸向き(上)の最大値
	const float ROT_HEIGHT_DOWN_MAX = -0.25f;	// 縦軸向き(下)の最大値

	const float SYURIKENN_SPEED = 15.0f;	// 手裏剣速度

	const float SYURIKENN_MOUSE_SPEED_SHOT = 7.0f;	// 手裏剣の発射判定のマウス速度
	const float SYURIKENN_MOUSE_SPEED_STOP = 0.3f;	// 手裏剣の停止判定のマウス速度

	const float SYURIKENN_PLAYER_SPEED = 0.2f;

	const D3DXVECTOR3 SHURIKEN_UI_POS = D3DXVECTOR3(1150.0f, 550.0f, 0.0f);	// 手裏剣UIの位置
	const int SHURIKEN_MAX = 5;												// 手裏剣の最大所持数
	const float SHURIKEN_RELOAD_TIME = 150.0f;								// 手裏剣のリロード速度

	const float SYURIKENN_NEED_MOVE = 0.15f;		// ジョイコン手裏剣のジャイロ速度
	const float SYURIKENN_NEED_ROT = 15.0f;			// ジョイコン手裏剣の向き
	const float SYURIKENN_JOYCON_SPEED_STOP = 2.0f;	// ジョイコン手裏剣の停止判定速度

	const float EVASION_NEED_MOVE = 0.8f;	// ジョイコン回避のジャイロ速度
	const float EVASION_NEED_ROT = 20.0f;	// ジョイコン回避の向き

	const float AUTO_ROT_SPEED = 0.02f;		//自動移動の向き変更速度

	const D3DXVECTOR3 PARAMETER_POS = D3DXVECTOR3(370.0f,650.0f,0.0f);		//パラメータの位置
}

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
	m_Action = ACTION_WAIT;
	m_AtkAction = ACTION_WAIT;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_AtkPos = INITVECTOR3;
	m_CollisionRot = 0.0f;
	m_HeightCameraRot = 0.0f;
	m_fLife = MAX_LIFE;
	m_pMotion = nullptr;
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
	CPlayer* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//プレイヤーの生成
		pPlayer = new CPlayer();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPlayer->Init()))
	{//初期化処理が失敗した場合
		return NULL;
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
	LoadLevelData("data\\TXT\\motion_player.txt");

	//モーションの生成
	if (m_pMotion == NULL)
	{
		//モーションの生成
		m_pMotion = new CMotion;
	}

	//初期化処理
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData("data\\TXT\\motion_player.txt");

	SetModelDisp(false);

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
	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	// 過去の位置に代入
	m_posOld = m_pos;

	// 移動処理
	Move();

	// 向き移動処理
	Rot();

	// カメラ更新処理
	CameraPosUpdate();

	// 位置更新処理
	PosUpdate();

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
	CManager::GetInstance()->GetDebugProc()->Print("[自分]位置 %f : %f : %f\n", m_pos.x, m_pos.y, m_pos.z);
	CManager::GetInstance()->GetDebugProc()->Print("[自分]向き %f : %f : %f\n", m_rot.x, m_rot.y, m_rot.z);
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

#ifdef _DEBUG

	//キーボードの移動処理
	if (pInputKeyboard->GetPress(DIK_W))
	{
		NormarizeMove.z += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.x += 1.0f * sinf(CameraRot.y) * PLAYER_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_S))
	{
		NormarizeMove.z += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.x += -1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
	}
	if (pInputKeyboard->GetPress(DIK_A))
	{
		NormarizeMove.x += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.z -= -1.0f * sinf(CameraRot.y) * PLAYER_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_D))
	{
		NormarizeMove.x += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.z -= 1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_W) == false && pInputKeyboard->GetPress(DIK_A) == false && pInputKeyboard->GetPress(DIK_S) == false && pInputKeyboard->GetPress(DIK_D) == false)
	{
		//左スティックによる前後移動	
		m_move.z += pInputJoypad->Get_Stick_Left(0).y * cosf(CameraRot.y) * PLAYER_SPEED;
		m_move.x += pInputJoypad->Get_Stick_Left(0).y * sinf(CameraRot.y) * PLAYER_SPEED;

		//左スティックによる左右移動
		m_move.x += pInputJoypad->Get_Stick_Left(0).x * cosf(CameraRot.y) * PLAYER_SPEED;
		m_move.z -= pInputJoypad->Get_Stick_Left(0).x * sinf(CameraRot.y) * PLAYER_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= PLAYER_SPEED;
	}

#else

	//キーボードの移動処理
	if (pInputKeyboard->GetPress(DIK_W))
	{
		NormarizeMove.z += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.x += 1.0f * sinf(CameraRot.y) * PLAYER_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_S))
	{
		NormarizeMove.z += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.x += -1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
	}
	if (pInputKeyboard->GetPress(DIK_A))
	{
		NormarizeMove.x += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.z -= -1.0f * sinf(CameraRot.y) * PLAYER_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_D))
	{
		NormarizeMove.x += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.z -= 1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
	}
	if (pInputKeyboard->GetPress(DIK_W) == false && pInputKeyboard->GetPress(DIK_A) == false && pInputKeyboard->GetPress(DIK_S) == false && pInputKeyboard->GetPress(DIK_D) == false)
	{
		//左スティックによる前後移動	
		m_move.z += pInputJoypad->Get_Stick_Left(0).y * cosf(CameraRot.y) * PLAYER_SPEED;
		m_move.x += pInputJoypad->Get_Stick_Left(0).y * sinf(CameraRot.y) * PLAYER_SPEED;

		//左スティックによる左右移動
		m_move.x += pInputJoypad->Get_Stick_Left(0).x * cosf(CameraRot.y) * PLAYER_SPEED;
		m_move.z -= pInputJoypad->Get_Stick_Left(0).x * sinf(CameraRot.y) * PLAYER_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= PLAYER_SPEED;
	}

#endif // DEBUG

	m_move += NormarizeMove;

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

#ifdef _DEBUG
	if (pInputKeyboard->GetPress(DIK_I) == true)
	{
		m_HeightCameraRot += ROT_SPEED;
		if (m_HeightCameraRot > ROT_HEIGHT_UP_MAX)
		{
			m_HeightCameraRot = ROT_HEIGHT_UP_MAX;
		}
	}
	if (pInputKeyboard->GetPress(DIK_L) == true)
	{
		m_rot.y += ROT_SPEED;
	}
	if (pInputKeyboard->GetPress(DIK_J) == true)
	{
		m_rot.y -= ROT_SPEED;
	}
	if (pInputKeyboard->GetPress(DIK_K) == true)
	{
		m_HeightCameraRot -= ROT_SPEED;
		if (m_HeightCameraRot < ROT_HEIGHT_DOWN_MAX)
		{
			m_HeightCameraRot = ROT_HEIGHT_DOWN_MAX;
		}
	}
#else
	if (CManager::TYPE_MNK == CManager::GetInstance()->GetTypeInput())
	{
		if (pInputKeyboard->GetPress(DIK_I) == true)
		{
			m_HeightCameraRot += ROT_SPEED;
			if (m_HeightCameraRot > ROT_HEIGHT_UP_MAX)
			{
				m_HeightCameraRot = ROT_HEIGHT_UP_MAX;
			}
		}
		if (pInputKeyboard->GetPress(DIK_L) == true)
		{
			m_rot.y += ROT_SPEED;
		}
		if (pInputKeyboard->GetPress(DIK_J) == true)
		{
			m_rot.y -= ROT_SPEED;
		}
		if (pInputKeyboard->GetPress(DIK_K) == true)
		{
			m_HeightCameraRot -= ROT_SPEED;
			if (m_HeightCameraRot < ROT_HEIGHT_DOWN_MAX)
			{
				m_HeightCameraRot = ROT_HEIGHT_DOWN_MAX;
			}
		}
	}
#endif // DEBUG

	useful::NormalizeAngle(&m_rot);
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CPlayer::ActionState(void)
{
	//移動モーション
	if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
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
// 壁との当たり判定
//====================================================================
void CPlayer::CollisionWall(useful::COLLISION XYZ)
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
// マップモデルの当たり判定
//====================================================================
void CPlayer::CollisionMapModel(useful::COLLISION XYZ)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_MAPMODEL)
			{//種類がブロックの時

				CMapModel* pMapModel = (CMapModel*)pObj;	// ブロック情報の取得

				if (pMapModel->GetCollision() == true)
				{
					D3DXVECTOR3 pos = pMapModel->GetPos();
					D3DXVECTOR3 posOld = pMapModel->GetPosOld();
					D3DXVECTOR3 Move = pMapModel->GetMove();
					D3DXVECTOR3 Size = pMapModel->GetSize();

					// 矩形の当たり判定
					if (useful::CollisionBlock(pos, posOld, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
					{
						int a = pMapModel->GetEditIdx();

						pMapModel->Hit(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
					}
				}
			}

			pObj = pObjNext;
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
	m_move.x = m_move.x * 0.5f;
	m_move.z = m_move.z * 0.5f;

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
	CollisionMapModel(useful::COLLISION_Y);

	//X軸の位置更新
	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_X);
	CollisionMapModel(useful::COLLISION_X);

	//Z軸の位置更新
	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_Z);
	CollisionMapModel(useful::COLLISION_Z);
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
	m_HeightCameraRot = 0.0f;
}

//====================================================================
//ダメージ処理
//====================================================================
void CPlayer::HitDamage(float Damage)
{
	if (m_State == STATE_NORMAL)
	{
		m_fLife -= Damage;

		if (m_fLife <= 0.0f)
		{
			m_fLife = 0.0f;
			m_State = STATE_DEATH;
			CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_FOLLOW);
		}
		else
		{
			m_State = STATE_DAMAGE;
			m_nStateCount = 120;

			// 炎の音
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE_PLAYER);
		}
	}

	if (m_State == STATE_DEATH)
	{
		CGame::SetGameEnd(true);
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

		while (pObj != NULL)
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
