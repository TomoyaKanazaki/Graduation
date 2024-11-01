//============================================
//
//	オブジェクトＸモデルのサンプル [SampleObjX.cpp]
//	Author:morikawa shunya
//
//============================================
#include "fire.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "enemy.h"

//==========================================
// 定数定義
//==========================================
namespace
{
	const int FIRE_LIFE = 60;			// 炎の体力
	const float FIRE_SPEED = 10.0f;		// 炎の速度
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CFire>* CFire::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CFire::CFire(int nPriority) : CObjectX(nPriority)
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
	m_nLife = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CFire::~CFire()
{

}

//====================================================================
//生成処理
//====================================================================
CFire* CFire::Create(char* pModelName, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	CFire* pFire = nullptr;

	if (pFire == nullptr)
	{
		//オブジェクト2Dの生成
		pFire = new CFire();

		pFire->SetPos(pos);
		pFire->SetRot(rot);
		pFire->m_rot = rot;
	}

	//オブジェクトの初期化処理
	if (FAILED(pFire->Init(pModelName)))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pFire;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CFire::Init(char* pModelName)
{
	// オブジェクトの種類設定
	SetType(CObject::TYPE_FIRE);

	// 継承クラスの初期化
	CObjectX::Init(pModelName);

	// 炎の体力
	m_nLife = FIRE_LIFE;

	// 炎の速度
	D3DXVECTOR3 move = -D3DXVECTOR3(FIRE_SPEED * sinf(m_rot.y), 0.0f, FIRE_SPEED * cosf(m_rot.y));
	SetMove(move);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CFire>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CFire::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CFire::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();

	GameUpdate();

	//位置更新
	pos += m_move;

	// 位置・移動量設定
	SetPos(pos);
	
	// 敵との判定
	CollisionEnemy();

	// 減算
	m_nLife--;

	if (m_nLife < 0)
	{// 体力0以下になった
		//破棄する
		Uninit();
	}
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CFire::GameUpdate(void)
{
	//更新前の位置を過去の位置とする
	m_posOld = m_pos;

	//位置更新
	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

	//画面外判定
	if (m_pos.y < 0.0f)
	{
		Uninit();
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
void CFire::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
// 敵との判定
//====================================================================
void CFire::CollisionEnemy()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			// 種類を取得
			CObject::TYPE type = pObj->GetType();

			if (type == TYPE_ENEMY3D)
			{//種類が敵の時
				// 敵の情報の取得
				CEnemy* pEnemy = (CEnemy*)pObj;	

				D3DXVECTOR3 pos = pEnemy->GetPos();
				D3DXVECTOR3 Size = pEnemy->GetSize();

				// 円の当たり判定
				if (useful::CollisionCircle(m_pos, pos, Size.x) == true)
				{// 弾が当たった
					pEnemy->Hit(1);

					Uninit();

					return;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//状態管理
//====================================================================
void CFire::StateManager(void)
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
//リスト取得
//====================================================================
CListManager<CFire>* CFire::GetList(void)
{
	return m_pList;
}
