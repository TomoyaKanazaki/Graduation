//============================================
//
//	聖書の処理 [bible.cpp]
//	Author:morikawa shunya
//
//============================================
#include "bible.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "MapSystem.h"
#include "effect.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CBible>* CBible::m_pList = nullptr; // オブジェクトリスト

//====================================================================
// コンストラクタ
//====================================================================
CBible::CBible(int nPriority) : CItem(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_fMove = 0.0f;
}

//====================================================================
// デストラクタ
//====================================================================
CBible::~CBible()
{

}

//====================================================================
// 生成
//====================================================================
CBible* CBible::Create(char* pModelName)
{
	// インスタンス生成
	CBible* pBible = new CBible;

	// 初期化
	pBible->Init(pModelName);

	return pBible;
}

//====================================================================
// 初期化
//====================================================================
HRESULT CBible::Init(char* pModelName)
{
	// 継承クラスの初期化
	CItem::Init(pModelName);

	SetType(CObject::TYPE_BIBLE);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CBible>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
// 終了
//====================================================================
void CBible::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 継承クラスの終了
	CItem::Uninit();
}

//====================================================================
// 更新
//====================================================================
void CBible::Update(void)
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
// タイトルでの更新
//====================================================================
void CBible::TitleUpdate(void)
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
// ゲームでの更新
//====================================================================
void CBible::GameUpdate(void)
{
	//状態管理
	StateManager();

	//親クラスの更新
	CItem::Update();

	// プレイヤーとの判定
	CollisionPlayer();
}

//====================================================================
// 描画
//====================================================================
void CBible::Draw(void)
{
	// 継承クラスの描画
	CItem::Draw();
}

//====================================================================
// プレイヤーとの判定
//====================================================================
bool CBible::CollisionPlayer()
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

			// 聖書に設定
			pPlayer->SetItemType(CPlayer::TYPE_BIBLE);

			//// アイテムの位置をプレイヤーと同じ位置に設定
			//SetPos(pos);

			Uninit();
			return true;

			pObj = pObjNext;
		}
	}

	return true;
}

//====================================================================
// 動きの制御
//====================================================================
void CBible::Move()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosold();
	D3DXVECTOR3 rot = GetRot();

	m_fMove += D3DX_PI * 0.01f;

	D3DXVECTOR3 GritPos = INITVECTOR3;
	float MaxX = 0.0f;
	float MaxZ = 0.0f;

	//更新前の位置を過去の位置とする
	posOld = pos;

	switch (m_PosType)
	{
	case CBible::POS_TYPE_LEFTUP:

		GritPos = CMapSystem::GetInstance()->GetStartGritPos(1.5f, 1.5f);

		pos.x = GritPos.x - sinf(m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;
		pos.y = 0.0f;
		pos.z = GritPos.z + sinf(m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		break;
	case CBible::POS_TYPE_RIGHTUP:

		MaxX = (float)CMapSystem::GetInstance()->GetWightMax();
		GritPos = CMapSystem::GetInstance()->GetStartGritPos(MaxX - 1.5f, 1.5f);

		pos.x = GritPos.x + sinf(m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;
		pos.y = 0.0f;
		pos.z = GritPos.z + sinf(m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		break;
	case CBible::POS_TYPE_LEFTDOWN:

		MaxZ = (float)CMapSystem::GetInstance()->GetHeightMax();
		GritPos = CMapSystem::GetInstance()->GetStartGritPos(1.5f, MaxZ - 1.5f);

		pos.x = GritPos.x + sinf(-m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;
		pos.y = 0.0f;
		pos.z = GritPos.z + sinf(-m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		break;
	case CBible::POS_TYPE_RIGHTDOWN:

		MaxX = (float)CMapSystem::GetInstance()->GetWightMax();
		MaxZ = (float)CMapSystem::GetInstance()->GetHeightMax();
		GritPos = CMapSystem::GetInstance()->GetStartGritPos(MaxX - 1.5f, MaxZ - 1.5f);

		pos.x = GritPos.x - sinf(-m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;
		pos.y = 0.0f;
		pos.z = GritPos.z + sinf(-m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		break;

	default:
		break;
	}

	float Scaling = GetScaling();

#ifdef _DEBUG

	CEffect* pEffect = CEffect::Create();
	pEffect->SetPos(pos);
	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pEffect->SetRadius(50.0f);

#endif // _DEBUG

	//位置更新
	CItem::SetPos(pos);
	CObjectX::SetPos(pos);
	CObjectX::SetRot(rot);

	//大きさの設定
	SetScaling(Scaling);
}

//====================================================================
//状態管理
//====================================================================
void CBible::StateManager(void)
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
