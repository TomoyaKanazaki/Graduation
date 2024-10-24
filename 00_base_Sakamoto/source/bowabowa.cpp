//============================================
//
//	ボワボワの処理 [SampleObjX.cpp]
//	Author:sakamoto kai
//
//============================================
#include "bowabowa.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "game.h"
#include "score.h"

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
CListManager<CBowabowa>* CBowabowa::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CBowabowa::CBowabowa(int nPriority) : CItem(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
}

//====================================================================
//デストラクタ
//====================================================================
CBowabowa::~CBowabowa()
{

}

//====================================================================
//生成処理
//====================================================================
CBowabowa* CBowabowa::Create(char* pModelName)
{
	CBowabowa* pBowabowa = nullptr;

	if (pBowabowa == nullptr)
	{
		//オブジェクト2Dの生成
		pBowabowa = new CBowabowa();
	}

	//オブジェクトの初期化処理
	if (FAILED(pBowabowa->Init(pModelName)))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pBowabowa;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CBowabowa::Init(char* pModelName)
{
	SetType(CObject::TYPE_BOWABOWA);

	CItem::Init(pModelName);

	//モードごとに初期値を設定出来る
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:
		CGame::AddBowabowa(1);
		break;

	case CScene::MODE_RESULT:
		break;
	}

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CBowabowa>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CBowabowa::Uninit(void)
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
void CBowabowa::Update(void)
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
void CBowabowa::TitleUpdate(void)
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
void CBowabowa::GameUpdate(void)
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

	if (CollisionPlayer())
	{// 当たったら消滅
		return;
	}
}

//====================================================================
//描画処理
//====================================================================
void CBowabowa::Draw(void)
{
	CItem::Draw();
}

//====================================================================
//状態管理
//====================================================================
void CBowabowa::StateManager(void)
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

//====================================================================
//状態管理
//====================================================================
void CBowabowa::Take(void)
{
	CGame::AddBowabowa(-1);
	CScore *pScore = CGame::GetScore();
	pScore->AddScore(100);
	Uninit();
}

//====================================================================
//プレイヤーとの判定
//====================================================================
bool CBowabowa::CollisionPlayer()
{
	// falseの時
	if (!CItem::CollisionPlayer())
	{
		return false;
	}

	//ステージのボワボワ数を減らす
	CGame::AddBowabowa(-1);
	// 消滅
	Uninit();
	return true;
}