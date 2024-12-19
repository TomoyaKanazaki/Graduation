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
#include "game.h"
#include "tutorial.h"
#include "objmeshField.h"
#include "devil.h"
#include "MapMove.h"

//==========================================
// 定数定義
//==========================================
namespace
{
	const int FIRE_LIFE = 120;			// 炎の体力
	const float FIRE_SPEED = 10.0f;		// 炎の速度
	const float FIRE_HEIGHT = 50.0f;	// 炎の高さ
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CFire>* CFire::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CFire::CFire(int nPriority) : CObjectX(nPriority),
m_Grid(CMapSystem::GRID(0, 0)),
m_pEffect(nullptr)
{
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
	}

	//オブジェクトの初期化処理
	if (FAILED(pFire->Init(pModelName)))
	{//初期化処理が失敗した場合
		assert(false);
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

	// 自身の情報を取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 高さを調整
	pos.y = FIRE_HEIGHT;

	// グリッド座標を設定
	m_Grid = CMapSystem::GetInstance()->CalcGrid(pos);

	// 座標を設定
	SetPos(pos);

	// 継承クラスの初期化
	CObjectX::Init(pModelName);

	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// 炎の体力
	m_nLife = FIRE_LIFE;

	// エフェクトを生成する
	m_pEffect = MyEffekseer::EffectCreate(CMyEffekseer::TYPE_FIRE, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);

	// 炎の速度
	D3DXVECTOR3 move = -D3DXVECTOR3(FIRE_SPEED * sinf(rot.y), 0.0f, FIRE_SPEED * cosf(rot.y));
	SetMove(move);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CFire>::Create();
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
void CFire::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// エフェクトを消去
	if (m_pEffect != nullptr)
	{
		m_pEffect->SetDeath();
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
	D3DXVECTOR3 rot = GetRot();

	//更新前の位置を過去の位置とする
	SetPosOld(pos);

	//頂点情報の更新
	CObjectX::Update();

	//位置更新
	pos += m_move;

	// スクロールに合わせて移動する
	//CMapSystem::GetInstance()->GetMove()->FollowScroll(pos);

	// 位置・移動量設定
	SetPos(pos);

	// グリッド座標を設定
	m_Grid = CMapSystem::GetInstance()->CalcGrid(pos);
	
	// エフェクトを動かす
	if (m_pEffect != nullptr)
	{
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(pos, rot, *GetUseMultiMatrix());
		m_pEffect->SetPosition(ef);
	}

	// 寿命減算
	m_nLife--;

	// 消滅判定
	if (
		CollisionEnemy() ||
		CollisionWall() ||
		m_nLife < 0
		)
	{
		Uninit();
	}
}

//====================================================================
//描画処理
//====================================================================
void CFire::Draw(void)
{
	//CObjectX::Draw();
}

//====================================================================
// 敵との判定
//====================================================================
bool CFire::CollisionEnemy()
{
	// エネミーリスト構造が無ければ抜ける
	if (CEnemy::GetList() == nullptr) { return false; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // リストを取得

	// キューブブロックリストの中身を確認する
	for (CEnemy* pEnemy : list)
	{
		if (pEnemy->GetHitState() != CEnemy::HIT_STATE_NORMAL)
		{
			continue;
		}

		// 円の当たり判定
		if (m_Grid == pEnemy->GetGrid())
		{// 弾が当たった
			pEnemy->Hit();

			// エフェクトを生成
			D3DXVECTOR3 pos = pEnemy->GetPos();
			D3DXVECTOR3 rot = pEnemy->GetRot();
			if (pEnemy->GetEnemyType() == CEnemy::ENEMY_LITTLEDEVIL)
			{
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_HIT_SMALLDEVIL, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);
			}
			else
			{
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_HIT, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);
			}

			return true;
		}
	}

	return false;
}

//==========================================
//  壁の判定
//==========================================
bool CFire::CollisionWall()
{
	// 自身のグリッド座標が移動可能な場合関数を抜ける
	if (!CMapSystem::GetInstance()->GetGritBool(m_Grid)) { return false; }

	// 自身の情報を取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// エフェクトを生成
	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_HITTHEWALL, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot, D3DXVECTOR3(25.0f, 25.0f, 25.0f));

	return true;
}

//==========================================
//リスト取得
//==========================================
CListManager<CFire>* CFire::GetList(void)
{
	return m_pList;
}
