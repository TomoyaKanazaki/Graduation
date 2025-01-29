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
#include "game.h"
#include "objmeshField.h"
#include "sound.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
	const char* MODEL_PASS = "data\\MODEL\\key.x"; // モデルパス
	const float MOVE_SCALE = sqrtf(100.0f * 100.0f * 4.0f); // 移動幅
	const D3DXVECTOR3 ADD_ROT = D3DXVECTOR3(0.0f, 0.05f, 0.0f); // 回転量

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
}

//====================================================================
// デストラクタ
//====================================================================
CBible::~CBible()
{

}

//====================================================================
// 初期化
//====================================================================
HRESULT CBible::Init()
{
	// 親クラスの初期化
	if (FAILED(CItem::Init(MODEL_PASS))) { assert(false); return E_FAIL; }

	// オブジェクトの種類を設定
	SetType(CObject::TYPE_BIBLE);

	// スクロールの対象から外す
	SetMapScroll(false);

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

	// 継承クラスの終了
	CItem::Uninit();
}

//====================================================================
// 更新
//====================================================================
void CBible::Update(void)
{
	// 回転
	Rotation();

	//親クラスの更新
	CItem::Update();
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
// 動きの制御
//====================================================================
void CBible::Move(D3DXVECTOR3& pos)
{
	// 移動情報を取得
	D3DXVECTOR3 base = GetBase();
	float time = GetMoveTime();

	// フィールドの座標を取得
	D3DXVECTOR3 posField = CObjmeshField::GetListTop()->GetPos();

	// 自身の座標とフィールドを結ぶ時の向きを算出
	D3DXVECTOR3 vec = base - posField;
	float rot = atan2f(vec.z, vec.x);

	// 経過時間を取得
	time += DeltaTime::Get();

	// 移動幅に経過時間をかけ合わせる
	float fScale = sinf(time) * MOVE_SCALE;

	// 移動幅をxz成分に分割する
	float x = fScale * cosf(rot);
	float z = fScale * sinf(rot);

	// 基準位置に移動量を加算する
	pos.x = base.x + x;
	pos.z = base.z + z;

	// グリッドを設定
	SetGrid(CMapSystem::GetInstance()->CalcGrid(pos));
}

//==========================================
//  回転する処理
//==========================================
void CBible::Rotation()
{
	// 現在の向きを取得する
	D3DXVECTOR3 rot = GetRot();

	// 向きを加算
	rot += ADD_ROT;

	// 向きを適用
	SetRot(rot);
}

//==========================================
//  エフェクトを生成
//==========================================
void CBible::SetEffect()
{
	// 自身の情報を取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_BIBLE, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot, D3DXVECTOR3(30.0f, 30.0f, 30.0f)));
}

//====================================================================
// 状態管理
//====================================================================
bool CBible::Hit(CPlayer* pPlayer)
{
	// 既に聖書を持っている場合関数を抜ける
	if (pPlayer->GetItemType() == CPlayer::TYPE_BIBLE) { return false; }

	// プレイヤーのアイテムを設定
	pPlayer->SetItemType(CPlayer::TYPE_BIBLE);

	// サウンドの再生
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_GET_BIBLE);

	// エフェクトを生成する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_GETITEM, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);

	// 自身の削除
	Uninit();
	return true;
}

//==========================================
//  リストの取得
//==========================================
CListManager<CBible>* CBible::GetList(void)
{
	return m_pList;
}
