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

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
}

//====================================================================
// コンストラクタ
//====================================================================
CBible::CBible(int nPriority) : CItem(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
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

	return S_OK;
}

//====================================================================
// 終了
//====================================================================
void CBible::Uninit(void)
{
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
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosold();
	D3DXVECTOR3 rot = GetRot();

	float Scaling = GetScaling();

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

			D3DXVECTOR3 pos = pObj->GetPos();
			D3DXVECTOR3 Size = pObj->GetSize();

			// 聖書かどうか
			pPlayer->SetItemType(CPlayer::TYPE_BIBLE);

			// 指定パーツ表示
			pPlayer->SetPartsDisp(0, true);

			// アイテムの位置をプレイヤーと同じ位置に設定
			SetPos(pos);

			pObj = pObjNext;
		}
	}

	return true;
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
