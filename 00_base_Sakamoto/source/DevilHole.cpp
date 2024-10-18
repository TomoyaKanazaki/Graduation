//============================================
//
//	デビルホールの処理 [DevilHole.cpp]
//	Author:sakamoto kai
//
//============================================
#include "DevilHole.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "effect.h"
#include "game.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(50.0f, 0.0f, 50.0f);		//当たり判定
}

//====================================================================
//コンストラクタ
//====================================================================
CDevilHole::CDevilHole(int nPriority) : CObjectX(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = NULL;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_bSet[nCnt] = false;			//上下左右の穴が埋まっているかどうか
		m_pHoleKey[nCnt] = nullptr;		//上下左右の穴を埋めるポリゴン
	}
}

//====================================================================
//デストラクタ
//====================================================================
CDevilHole::~CDevilHole()
{

}

//====================================================================
//生成処理
//====================================================================
CDevilHole* CDevilHole::Create(char* pModelName)
{
	CDevilHole* pSample = NULL;

	if (pSample == NULL)
	{
		//オブジェクト2Dの生成
		pSample = new CDevilHole();
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
HRESULT CDevilHole::Init(char* pModelName)
{
	SetType(CObject::TYPE_DEVILHOLE);

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
void CDevilHole::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CDevilHole::Update(void)
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
void CDevilHole::TitleUpdate(void)
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
void CDevilHole::GameUpdate(void)
{
	//更新前の位置を過去の位置とする
	m_posOld = m_pos;

	//ホール解除判定処理
	CollisionOpen();

	//クリア判定処理
	ClearJudge();

	//位置更新
	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

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
}

//====================================================================
//描画処理
//====================================================================
void CDevilHole::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//状態管理
//====================================================================
void CDevilHole::StateManager(void)
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
// 壁との当たり判定
//====================================================================
void CDevilHole::CollisionOpen(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_PLAYER3D)
			{//種類がプレイヤーの時

				CPlayer *pPlayer = (CPlayer*)pObj;	// ブロック情報の取得

				D3DXVECTOR3 pos = pPlayer->GetPos();
				D3DXVECTOR3 MySize = GetSize();

				if (pPlayer->GetItemType() != CPlayer::TYPE_BIBLE)
				{
					pObj = pObjNext;

					continue;
				}

				for (int nCnt = 0; nCnt < 4; nCnt++)
				{
					D3DXVECTOR3 MyPos = m_pos;

					switch (nCnt)
					{
					case 0:	//上
						MyPos.z += 100.0f;
						break;

					case 1:	//下
						MyPos.z -= 100.0f;
						break;

					case 2:	//右
						MyPos.x += 100.0f;
						break;

					case 3:	//左
						MyPos.x -= 100.0f;
						break;
					}

					// 矩形の当たり判定
					if (useful::PointSquareXZ(pos, MyPos, MySize) == true &&
						m_bSet[nCnt] == false)
					{
						m_pHoleKey[nCnt] = CObjectX::Create("data\\MODEL\\DevilKey.x");

						switch (nCnt)
						{
						case 0:	//上
							m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + 20.0f));
							break;

						case 1:	//下
							m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z - 20.0f));
							break;

						case 2:	//右
							m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(m_pos.x + 20.0f, m_pos.y, m_pos.z));
							break;

						case 3:	//左
							m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(m_pos.x - 20.0f, m_pos.y, m_pos.z));
							break;
						}
						
						m_bSet[nCnt] = true;
						return;
					}
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// クリア判定処理
//====================================================================
void CDevilHole::ClearJudge(void)
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_bSet[nCnt] == false)
		{
			return;
		}
	}

	CGame::SetDevilHoleFinish(true);
}