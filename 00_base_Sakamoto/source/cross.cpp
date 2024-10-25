//============================================
//
//	オブジェクトＸモデルのサンプル [SampleObjX.cpp]
//	Author:sakamoto kai
//
//============================================
#include "cross.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const int CROSS_DELETTIME = 600;	// 十字架消滅時間
	const D3DXVECTOR3 INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 200.0f);		//当たり判定
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CCross>* CCross::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CCross::CCross(int nPriority) : CItem(nPriority)
{
	m_nDeletCont = 0;
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
}

//====================================================================
//デストラクタ
//====================================================================
CCross::~CCross()
{

}

//====================================================================
//生成処理
//====================================================================
CCross* CCross::Create(char* pModelName)
{
	CCross* pCross = nullptr;

	if (pCross == nullptr)
	{
		//オブジェクト2Dの生成
		pCross = new CCross();
	}

	//オブジェクトの初期化処理
	if (FAILED(pCross->Init(pModelName)))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pCross;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CCross::Init(char* pModelName)
{
	m_nDeletCont = 0;

	SetType(CObject::TYPE_CROSS);

	CItem::Init(pModelName);

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

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CCross>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CCross::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	CItem::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CCross::Update(void)
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
void CCross::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();

	//位置更新
	pos += move;

	SetPos(pos);

	//頂点情報の更新
	CItem::Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CCross::GameUpdate(void)
{
	// 大きさ取得
	float Scaling = GetScaling();

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosold();
	D3DXVECTOR3 rot = GetRot();

	//更新前の位置を過去の位置とする
	posOld = pos;

	//位置更新
	CObjectX::SetPos(pos);
	CObjectX::SetRot(rot);

	//画面外判定
	if (pos.y < 0.0f)
	{

	}

	//大きさの設定
	SetScaling(Scaling);

	//状態管理
	StateManager();

	//頂点情報の更新
	CItem::Update();

	// 判定
	CollisionPlayer();
}

//====================================================================
//描画処理
//====================================================================
void CCross::Draw(void)
{
	CItem::Draw();
}

//====================================================================
//プレイヤーとの判定
//====================================================================
bool CCross::CollisionPlayer()
{
	if (!CItem::CollisionPlayer())
	{
		return false;
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();	// 種類を取得

			if (type != TYPE_PLAYER3D)
			{
				pObj = pObjNext;
				continue;
			}

			CPlayer* pPlayer = (CPlayer*)pObj;		// アイテムの情報の取得

			if (pPlayer->GetItemType() == CPlayer::ITEM_TYPE::TYPE_BIBLE ||
				pPlayer->GetState() == CPlayer::STATE_DEATH)
			{
				pObj = pObjNext;
				continue;
			}

			D3DXVECTOR3 pos = pObj->GetPos();
			D3DXVECTOR3 Size = pObj->GetSize();

			// 十字架に設定、弾発射可能にする
			pPlayer->SetItemType(CPlayer::TYPE_CROSS);
			pPlayer->SetUseItem(true);

			// 指定パーツ表示
			pPlayer->SetPartsDisp(9, true);

			// アイテムの位置をプレイヤーの正面に設定
			SetPos(pos);

			// 加算
			m_nDeletCont++;

			if (m_nDeletCont > CROSS_DELETTIME)
			{// CROSS_DELETTIME秒経過
				// 使用不可
				pPlayer->SetUseItem(false);

				// 指定パーツ非表示
				pPlayer->SetPartsDisp(3, false);

				// 初期位置に戻す
				SetPos(INIT_POS);

				// カウントリセット
				m_nDeletCont = 0;
			}

			pObj = pObjNext;
		}
	}

	return true;
}

//====================================================================
//状態管理
//====================================================================
void CCross::StateManager(void)
{
	CItem::STATE State = GetState();

	int nStateCounter = GetStateCounter();

	switch (State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ACTION:
		break;
	}

	if (nStateCounter > 0)
	{
		nStateCounter--;
	}
}