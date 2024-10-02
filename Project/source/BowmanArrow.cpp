//============================================
//
//	弓兵の矢 [BowmanArrow.cpp]
//	Author:sakamoto kai
//
//============================================
#include "BowmanArrow.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "modelEffect.h"
#include "player.h"
#include "CubeBlock.h"
#include "MapModel.h"
#include "Effect.h"

//マクロ定義
#define COLLISION_SIZE (D3DXVECTOR3(20.0f,20.0f,20.0f))		//当たり判定
//====================================================================
//コンストラクタ
//====================================================================
CBowmanArrow::CBowmanArrow(int nPriority) : CObjectX(nPriority)
{
	SetSize(COLLISION_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = NULL;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_fLife = 150.0f;
}

//====================================================================
//デストラクタ
//====================================================================
CBowmanArrow::~CBowmanArrow()
{

}

//====================================================================
//生成処理
//====================================================================
CBowmanArrow* CBowmanArrow::Create(char* pModelName)
{
	CBowmanArrow* pSample = NULL;

	if (pSample == NULL)
	{
		//オブジェクト2Dの生成
		pSample = new CBowmanArrow();
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample->Init(pModelName)))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pSample;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CBowmanArrow::Init(char* pModelName)
{
	SetType(CObject::TYPE_ARROW);

	CObjectX::Init(pModelName);

	//モードごとに初期値を設定出来る
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


	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CBowmanArrow::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CBowmanArrow::Update(void)
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
void CBowmanArrow::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	//位置更新
	pos += m_move;

	SetPos(pos);

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CBowmanArrow::GameUpdate(void)
{
	//更新前の位置を過去の位置とする
	m_posOld = m_pos;

	if (!m_bStop)
	{
		// 移動量を加算
		m_pos += m_move;

		CollisionMapObject(m_pos, m_posOld, useful::COLLISION_X);
		CollisionMapObject(m_pos, m_posOld, useful::COLLISION_Z);

		//位置更新
		CObjectX::SetPos(m_pos);

		CModelEffect* pEffect = CModelEffect::Create("data\\MODEL\\weapon\\Arrow001.x");
		pEffect->SetPos(m_pos);
		pEffect->SetRot(GetRot());
		pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		//プレイヤーとの当たり判定
		CollisionPlayer();
	}

	//画面外判定
	if (m_pos.y < 0.0f)
	{

	}

	//大きさの設定
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//状態管理
	StateManager();

	//頂点情報の更新
	CObjectX::Update();

	if (m_fLife > 0)
	{
		m_fLife--;
	}
	else
	{
		SetDeathFlag(true);
	}
}

//====================================================================
//描画処理
//====================================================================
void CBowmanArrow::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//状態管理
//====================================================================
void CBowmanArrow::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ACTION:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//状態管理
//====================================================================
void CBowmanArrow::CollisionPlayer(void)
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

			if (type == TYPE_PLAYER3D)
			{//種類がプレイヤーの時
				CPlayer* pPlayer = (CPlayer*)pObj;

				if (useful::CollisionCircle(m_pos, pPlayer->GetPos(), 70.0f) == true)
				{
					pPlayer->HitDamage(10.0f);
					Uninit();
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// 壁との当たり判定
//====================================================================
void CBowmanArrow::CollisionMapObject(D3DXVECTOR3 Mypos, D3DXVECTOR3 MyposOld, useful::COLLISION XYZ)
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
				D3DXVECTOR3 MyMove = m_move;
				bool JumpNull = false;
				D3DXVECTOR3 ObjMoveNull = INITVECTOR3;

				// 矩形の当たり判定
				if (useful::CollisionBlock(pos, posOld, Move, Size, &Mypos, MyposOld, &MyMove, &ObjMoveNull, D3DXVECTOR3(5.0f, 5.0f, 5.0f), &JumpNull, XYZ) == true)
				{
					SetPos(MyposOld);
					m_bStop = true;
				}
			}
			else if (type == TYPE_MAPMODEL)
			{
				CMapModel* pMapModel = (CMapModel*)pObj; // マップブロック情報の取得

				D3DXVECTOR3 pos = pMapModel->GetPos();
				D3DXVECTOR3 posOld = pMapModel->GetPosOld();
				D3DXVECTOR3 Move = pMapModel->GetMove();
				D3DXVECTOR3 Size = pMapModel->GetSize();
				D3DXVECTOR3 MyMove = m_move;
				bool JumpNull = false;
				D3DXVECTOR3 ObjMoveNull = INITVECTOR3;

				// 矩形の当たり判定
				if (useful::CollisionBlock(pos, posOld, Move, Size, &Mypos, MyposOld, &MyMove, &ObjMoveNull, D3DXVECTOR3(5.0f, 5.0f, 5.0f), &JumpNull, XYZ) == true)
				{
					if (pMapModel->GetBreakEffect() == false)
					{
						SetPos(MyposOld);
						m_bStop = true;
					}
				}
			}


			pObj = pObjNext;
		}
	}
}