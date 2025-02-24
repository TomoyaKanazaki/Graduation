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
#include "objmeshField.h"
#include "game.h"
#include "tutorial.h"
#include "devil.h"
#include "MapMove.h"
#include "sound.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(50.0f, 0.0f, 50.0f);		//当たり判定
	const int DIRECTION = 4;	// デビルホールの最大方向
	const float MODEL_SET_DISTANCE = 130.0f;	// デビルホールを埋めたときのモデル生成位置
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CDevilHole>* CDevilHole::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CDevilHole::CDevilHole(int nPriority) : CObjectX(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_Grid.x = 0;
	m_Grid.z = 0;
	m_Grid.x = 0;
	m_Grid.z = 0;

	for (int nCnt = 0; nCnt < DIRECTION; nCnt++)
	{
		m_bSet[nCnt] = false;			//上下左右の穴が埋まっているかどうか
		m_pHoleKey[nCnt] = nullptr;		//上下左右の穴を埋めるポリゴン
	}
}

//====================================================================
//コンストラクタ(オーバーロード)
//====================================================================
CDevilHole::CDevilHole(int nPriority, CMapSystem::GRID gridCenter) : CObjectX(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_Grid.x = 0;
	m_Grid.z = 0;
	m_Grid = gridCenter;		// グリッド

	for (int nCnt = 0; nCnt < DIRECTION; nCnt++)
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
CDevilHole* CDevilHole::Create(CMapSystem::GRID gridCenter)
{
	CDevilHole* pSample = nullptr;

	if (pSample == nullptr)
	{
		//オブジェクト2Dの生成
		pSample = new CDevilHole(3, gridCenter);
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample->Init("data\\MODEL\\04_devilhole\\devil_hole_base.x")))
	{//初期化処理が失敗した場合
		return nullptr;
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

	//マップとのマトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CDevilHole>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// スクロールをオンにする
	SetMapScroll(true);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CDevilHole::Uninit(void)
{
	if (m_pList != nullptr)
	{
		// リストから自身のオブジェクトを削除
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // オブジェクトが一つもない場合

			// リストマネージャーの破棄
			m_pList->Release(m_pList);
		}
	}

	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CDevilHole::Update(void)
{
	// 値を取得
	D3DXVECTOR3 posThis = GetPos();			// 位置
	D3DXVECTOR3 posOldThis = GetPosOld();	// 前回の位置
	D3DXVECTOR3 rotThis = GetRot();			// 向き
	D3DXVECTOR3 sizeThis = GetSize();		// 大きさ

	//更新前の位置を過去の位置とする
	posOldThis = posThis;

	//ホール解除判定処理
	CollisionOpen(posThis);

	//クリア判定処理
	ClearJudge();

	posThis = m_Grid.ToWorld();

	for (int nCnt = 0; nCnt < DIRECTION; nCnt++)
	{
		if (m_pHoleKey[nCnt] != nullptr)
		{
			switch (nCnt)
			{
			case 0:	//上
				m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(posThis.x, posThis.y, posThis.z + MODEL_SET_DISTANCE));
				break;

			case 1:	//下
				m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(posThis.x, posThis.y, posThis.z - MODEL_SET_DISTANCE));
				break;

			case 2:	//右
				m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(posThis.x + MODEL_SET_DISTANCE, posThis.y, posThis.z));
				break;

			case 3:	//左
				m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(posThis.x - MODEL_SET_DISTANCE, posThis.y, posThis.z));
				break;
			}
		}
	}

	//位置更新
	CObjectX::SetPos(posThis);

	//大きさの設定
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//状態管理
	StateManager();

	// スクロールに合わせて移動する
	//CMapSystem::GetInstance()->GetMove()->FollowScroll(posThis);

	// 値更新
	SetPos(posThis);		// 位置
	SetPosOld(posOldThis);	// 前回の位置
	SetRot(rotThis);		// 向き
	SetSize(sizeThis);		// 大きさ

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
void CDevilHole::CollisionOpen(D3DXVECTOR3& pos)
{
	// プレイヤーのリスト構造が無ければ抜ける
	if (CPlayer::GetList() == nullptr) { return; }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();    // リストを取得

	// プレイヤーリストの中身を確認する
	for (CPlayer* pPlayer : list)
	{
		// プレイヤーの位置・オブジェクトXサイズ取得
		CMapSystem::GRID PlayerGrid = pPlayer->GetGrid();
		D3DXVECTOR3 ObjXSize = GetSize();

		if (pPlayer->GetItemType() != CPlayer::TYPE_BIBLE)
		{// 聖書以外の時
			continue;
		}

		for (int nCnt = 0; nCnt < DIRECTION; nCnt++)
		{
			CMapSystem::GRID CollsionGrid = m_Grid;
			switch (nCnt)
			{
			case 0:	//上
				CollsionGrid.z -= 2;
				break;

			case 1:	//下
				CollsionGrid.z += 2;
				break;

			case 2:	//右
				CollsionGrid.x += 2;
				break;

			case 3:	//左
				CollsionGrid.x -= 2;
				break;
			}

			useful::RangeNumber(CMapSystem::GetInstance()->GetWightMax(), 0, CollsionGrid.x);
			useful::RangeNumber(CMapSystem::GetInstance()->GetHeightMax(), 0, CollsionGrid.z);

			// 矩形の当たり判定
			if (PlayerGrid == CollsionGrid &&
				m_bSet[nCnt] == false)
			{
				m_pHoleKey[nCnt] = CObjectX::Create("data\\MODEL\\04_devilhole\\devil_hole_fence.x");
				m_pHoleKey[nCnt]->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
				D3DXVECTOR3 posEffect = {};

				switch (nCnt)
				{
				case 0:	//上
					posEffect = D3DXVECTOR3(pos.x, pos.y, pos.z + MODEL_SET_DISTANCE);
					m_pHoleKey[nCnt]->SetPos(posEffect);
					m_pHoleKey[nCnt]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f));
					break;

				case 1:	//下
					posEffect = D3DXVECTOR3(pos.x, pos.y, pos.z - MODEL_SET_DISTANCE);
					m_pHoleKey[nCnt]->SetPos(posEffect);
					m_pHoleKey[nCnt]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f));
					break;

				case 2:	//右
					posEffect = D3DXVECTOR3(pos.x + MODEL_SET_DISTANCE, pos.y, pos.z);
					m_pHoleKey[nCnt]->SetPos(posEffect);
					m_pHoleKey[nCnt]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
					break;

				case 3:	//左
					posEffect = D3DXVECTOR3(pos.x - MODEL_SET_DISTANCE, pos.y, pos.z);
					m_pHoleKey[nCnt]->SetPos(posEffect);
					m_pHoleKey[nCnt]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
					break;
				}

				m_bSet[nCnt] = true;
				pPlayer->SetItemType(CPlayer::TYPE_NONE);
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_UNLOCKING, false, posEffect, GetRot());

				//サウンド生成
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_UNLOCK);
				return;
			}
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

	CGame::GetInstance()->SetDevilHoleFinish(true);
}

//====================================================================
// 埋まったかどうか
//====================================================================
bool CDevilHole::IsSet()
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_bSet[i] == true)
		{// 4箇所のどこかが埋まったら
			return m_bSet[i];
		}	
	}

	return false;
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CDevilHole>* CDevilHole::GetList(void)
{
	return m_pList;
}
