//============================================
//
//	階層構造でモーションを行うモデルのサンプル [SampleLevelModel.cpp]
//	Author:sakamoto kai
//
//============================================
#include "SampleLevelModel.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "character.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "object3D.h"
#include "debugproc.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 40.0f, 20.0f);		//横の当たり判定
}

//====================================================================
//コンストラクタ
//====================================================================
CSampleLvModel::CSampleLvModel(int nPriority) :CObject(nPriority)
{
	SetWidth(SAMPLE_SIZE.x);
	SetHeight(SAMPLE_SIZE.y);
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_bJump = false;
	m_nActionCount = 0;
	m_Action = ACTION_WAIT;
	m_AtkAction = ACTION_WAIT;
	m_nAttackHit = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_AtkPos = INITVECTOR3;
	m_CollisionRot = 0.0f;

	m_pCharacter = nullptr;
}

//====================================================================
//デストラクタ
//====================================================================
CSampleLvModel::~CSampleLvModel()
{

}

//====================================================================
//生成処理
//====================================================================
CSampleLvModel* CSampleLvModel::Create()
{
	CSampleLvModel* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//プレイヤーの生成
		pPlayer = new CSampleLvModel();
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
HRESULT CSampleLvModel::Init(void)
{
	//種類設定
	SetType(CObject::TYPE_SAMPLE);

	// キャラクターの生成
	if (m_pCharacter == nullptr)
	{
		m_pCharacter->Create("data\\TXT\\motion_player.txt");
	}

	CMotion* pMotion = m_pCharacter->GetMotion();

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:

		if (pMotion != nullptr)
		{
			// モーション設定
			pMotion->Set(ACTION_TITLE, 0);
		}

		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		break;

	case CScene::MODE_RESULT:
		break;
	}

	return S_OK;
}

//====================================================================
//自分が保持するオブジェクトの生成
//====================================================================
void CSampleLvModel::MyObjCreate(void)
{

}

//====================================================================
//終了処理
//====================================================================
void CSampleLvModel::Uninit(void)
{
	// キャラクターの終了処理
	if (m_pCharacter != nullptr)
	{
		// キャラクターの破棄
		m_pCharacter->Uninit();
		delete m_pCharacter;
		m_pCharacter = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CSampleLvModel::Update(void)
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
void CSampleLvModel::TitleUpdate(void)
{
	// キャラクターの更新
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Update();
	}
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CSampleLvModel::GameUpdate(void)
{
	//D3DXVECTOR3 ScreenCenter = D3DXVECTOR3(640.0f,360.0f,0.0f);
	//D3DXVECTOR3 ScreenPos[4] = { INITVECTOR3 };
	//D3DXVECTOR3 pos[4] = { INITVECTOR3 };

	//pos[0] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y, GetPos().z);
	//pos[1] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z);
	//pos[2] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y, GetPos().z);
	//pos[3] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z);

	////ビューポートの設定
	//D3DVIEWPORT9 viewport;
	//viewport.X = 0;
	//viewport.Y = 0;
	//viewport.Width = SCREEN_WIDTH;
	//viewport.Height = SCREEN_HEIGHT;
	//viewport.MaxZ = 0.0f;
	//viewport.MinZ = 0.0f;

	////マトリックスの取得
	//D3DXMATRIX ViewMatrix = CManager::GetInstance()->GetCamera()->GetViewMatrix();
	//D3DXMATRIX ProjectionMatrix = CManager::GetInstance()->GetCamera()->GetProjectionMatrix();

	////D3DXVec3Project(&screenPosition, &pos, &viewport, &projectionMatrix, &viewMatrix, D3DXMatrixIdentity(&mtx));

	////3Dの座標をスクリーン座標に変換
	//for (int nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	D3DXVec3Project(&ScreenPos[nCnt], &pos[nCnt], &viewport, &ProjectionMatrix, &ViewMatrix, D3DXMatrixIdentity(&m_mtxWorld));

	//	C2DEffect* pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(50.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//	pEffect->SetPos(ScreenPos[nCnt]);
	//}

	////角度変更
	//CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();

	//if (pKey->GetPress(DIK_RIGHT) == true)
	//{
	//	m_CollisionRot += 0.01f;
	//}

	//if (pKey->GetPress(DIK_LEFT) == true)
	//{
	//	m_CollisionRot -= 0.01f;
	//}

	////デバッグ用のエフェクト生成
	//for (int nCnt = 0; nCnt < 10; nCnt++)
	//{
	//	C2DEffect* pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(30.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	pEffect->SetPos(D3DXVECTOR3(
	//		ScreenCenter.x + sinf(m_CollisionRot) * 50.0f * nCnt,
	//		ScreenCenter.y + cosf(m_CollisionRot) * 50.0f * nCnt,
	//		0.0f));

	//	pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(30.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	pEffect->SetPos(D3DXVECTOR3(
	//		ScreenCenter.x + sinf(m_CollisionRot) * -50.0f * nCnt,
	//		ScreenCenter.y + cosf(m_CollisionRot) * -50.0f * nCnt,
	//		0.0f));
	//}

	////プレイヤーと敵の位置関係を示す外積
	//int Left = 0;
	//int	Right = 0;
	//for (int nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	D3DXVECTOR3 vecToPos;
	//	D3DXVECTOR3 vecLine;
	//	float A;

	//	vecToPos.x = (ScreenPos[nCnt].x) - (ScreenCenter.x);
	//	vecToPos.y = (ScreenPos[nCnt].y) - (ScreenCenter.y);

	//	vecLine.x = sinf(m_CollisionRot);
	//	vecLine.y = cosf(m_CollisionRot);
	//	vecLine.z = 0.0f;

	//	A = (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);

	//	if (A < 0)
	//	{
	//		Right++;
	//	}
	//	else
	//	{
	//		Left++;
	//	}
	//}

	//DebugProc::Print(DebugProc::POINT_LEFT, "左 : 右 → %d : %d\n", Left, Right);



	//モーションの管理
	ActionState();

	//状態の管理
	StateManager();

	// キャラクターの更新
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Update();
	}
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CSampleLvModel::ActionState(void)
{
	// キャラクターの生成
	if (m_pCharacter == nullptr)
	{
		return;
	}

	CMotion* pMotion = m_pCharacter->GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	//ジャンプモーション
	if (m_bJump == true)
	{
		if (m_Action != ACTION_JAMP)
		{
			m_Action = ACTION_JAMP;
			pMotion->Set(ACTION_JAMP, 5);
		}
	}
	//移動モーション
	else if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			pMotion->Set(ACTION_MOVE, 5);
		}
	}
	//ニュートラルモーション
	else
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			pMotion->Set(ACTION_WAIT, 5);
		}
	}
}

//====================================================================
//状態管理
//====================================================================
void CSampleLvModel::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;

	case STATE_DEATH:
		break;

	case STATE_WAIT:
		break;

	case STATE_DAMAGE:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//描画処理
//====================================================================
void CSampleLvModel::Draw(void)
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

	// キャラクターの描画
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Draw();
	}
}