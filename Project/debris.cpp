//============================================
//
//	破片の処理 [debris.cpp]
//	Author:hashimoto kenta
//
//============================================
#include "debris.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "XModel.h"
#include "MapModel.h"

#include "CubeBlock.h"
#include "CubeColl.h"

#include "useful.h"

//マクロ定義
#define COLLISION_SIZE (D3DXVECTOR3(20.0f,20.0f,20.0f))		//当たり判定
namespace
{
	char* MODEL_FILE_PATH_00 = "data\\MODEL\\debris\\CrashGlass00.x";	// モデルのパス
	char* MODEL_FILE_PATH_01 = "data\\MODEL\\debris\\CrashGlass01.x";	// モデルのパス
	char* MODEL_FILE_PATH_02 = "data\\MODEL\\debris\\CrashGlass02.x";	// モデルのパス

	int SIZE_RANGE = 9;
	int SIZE_MIN = 8;
	int ROTMOVE_RANGE = 100;
}

//====================================================================
//コンストラクタ
//====================================================================
CDebris::CDebris(int nPriority) : CObjectX(nPriority)
{
	SetSize(COLLISION_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = NULL;			//マテリアルの数
	m_Scaling = 1.0f;
}

//====================================================================
//デストラクタ
//====================================================================
CDebris::~CDebris()
{

}

//====================================================================
//生成処理
//====================================================================
CDebris* CDebris::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int ModelIdx)
{
	CDebris* pDebris = NULL;

	if (pDebris == NULL)
	{
		//オブジェクト2Dの生成
		pDebris = new CDebris();
	}

	//オブジェクトの初期化処理
	if (FAILED(pDebris->Init(pos, move, ModelIdx)))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pDebris;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CDebris::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int ModelIdx)
{
	char* pModelName = {};

	SetType(CObject::TYPE_DEBRIS);

	SetPos(pos);
	SetMove(move);
	SetRamScaling();
	SetRotMove();

	//Xモデルの取得
	CXModel::XModel* pXmodel = CManager::GetInstance()->GetXModel()->GetXModel(ModelIdx);

	//マテリアルデータへのポインタを所得する
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)pXmodel->m_pBuffMat->GetBufferPointer();

	SetMatColor(pMat[(rand() % (int)pXmodel->m_dwNumMat)].MatD3D.Diffuse);

	pModelName = SetModelName();

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
void CDebris::Uninit()
{
	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CDebris::Update()
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
void CDebris::TitleUpdate()
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
void CDebris::GameUpdate()
{
	//更新前の位置を過去の位置とする
	m_posOld = m_pos;

	//位置更新
	m_move.y -= 0.5f;
	m_pos += m_move;

	//向き更新
	m_rot += m_rotmove;

	CollisionMapObject(m_pos, m_posOld, useful::COLLISION_X);
	CollisionMapObject(m_pos, m_posOld, useful::COLLISION_Y);
	CollisionMapObject(m_pos, m_posOld, useful::COLLISION_Z);

	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

	//画面外判定
	if (m_pos.y < 0.0f)
	{
		//m_pos.y = 0.0f;
		//m_move.y = 0.0f;

		Uninit();
	}

	//大きさの設定
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//描画処理
//====================================================================
void CDebris::Draw()
{
	CObjectX::Draw();
}

//====================================================================
//モデルのパスを設定
//====================================================================
char* CDebris::SetModelName()
{
	char* pModelName = {};
	int i = rand() % 3;

	switch (i)
	{
	case 0:
		pModelName = MODEL_FILE_PATH_00;
		break;

	case 1:
		pModelName = MODEL_FILE_PATH_01;
		break;

	case 2:
		pModelName = MODEL_FILE_PATH_02;
		break;

	default:
		break;
	}

	return pModelName;
}

//====================================================================
//モデルのサイズをランダムで設定
//====================================================================
void CDebris::SetRamScaling()
{
	m_Scaling = (float)(rand() % SIZE_RANGE + SIZE_MIN) / 10;
}

//====================================================================
//回転の移動量を設定
//====================================================================
void CDebris::SetRotMove()
{
	m_rotmove.x = (float)(rand() % ROTMOVE_RANGE - ROTMOVE_RANGE / 2) / 100;
	m_rotmove.y = (float)(rand() % ROTMOVE_RANGE - ROTMOVE_RANGE / 2) / 100;
	m_rotmove.z = (float)(rand() % ROTMOVE_RANGE - ROTMOVE_RANGE / 2) / 100;
}

//====================================================================
// 壁との当たり判定
//====================================================================
void CDebris::CollisionMapObject(D3DXVECTOR3 Mypos, D3DXVECTOR3 MyposOld, useful::COLLISION XYZ)
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
				if (useful::CollisionBlock(pos, posOld, Move, Size, &Mypos, MyposOld, &MyMove, &ObjMoveNull, GetSize() * 2.0f, &JumpNull, XYZ) == true)
				{
					m_move = MyMove;
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
				if (useful::CollisionBlock(pos, posOld, Move, Size, &Mypos, MyposOld, &MyMove, &ObjMoveNull, GetSize() * 2.0f, &JumpNull, XYZ) == true)
				{
					Uninit();
				}
			}


			pObj = pObjNext;
		}
	}
}
