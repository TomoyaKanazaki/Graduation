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
#include "player.h"
#include "sound.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
	const char* MODEL_PASS = "data\\MODEL\\beef.x"; // モデルパス
	const float MOVE_HEIGHT = 25.0f; // 移動量
	const D3DXVECTOR3 ADD_ROT = D3DXVECTOR3(0.0f, 0.01f, 0.0f); // 回転量
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
//初期化処理
//====================================================================
HRESULT CBowabowa::Init()
{
	// 親クラスの初期化
	if(FAILED(CItem::Init(MODEL_PASS))){ assert(false); return E_FAIL; }

	// オブジェクトの種類を設定
	SetType(CObject::TYPE_BOWABOWA);

	// 初期の時間を設定する
	float time = (float)(rand() / 628) * 0.01f;
	SetMoveTime(time);

	// 描画をオフにする
	//SetDisp(false);

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

	// 親クラスの終了処理
	CItem::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CBowabowa::Update(void)
{
	// 親クラスの更新処理
	CItem::Update();
}

//====================================================================
//描画処理
//====================================================================
void CBowabowa::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 親クラスの描画処理
	CItem::Draw();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 3);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 => (参照値 => ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// 親クラスの描画処理
	CItem::Draw();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 105);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 => (参照値 => ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// 親クラスの描画処理
	CItem::Draw();

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//==========================================
// ヒット処理
//==========================================
bool CBowabowa::Hit(CPlayer* pPlayer)
{
	// 十字架を持ってない場合関数を抜ける
	if (pPlayer->GetItemType() != CPlayer::TYPE_CROSS)
	{
		return false;
	}

	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_EAT);

	// エフェクトを生成する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_EAT, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);

	// 削除
	Uninit();
	return true;
}

//========================================== 
//  移動処理
//==========================================
void CBowabowa::Move(D3DXVECTOR3& pos)
{
	// 現在の向きを取得する
	D3DXVECTOR3 rot = GetRot();

	// 向きを加算
	rot += ADD_ROT;

	// 向きを適用
	SetRot(rot);

	// 移動情報を取得
	D3DXVECTOR3 base = GetBase();
	float time = GetMoveTime();

	// 経過時間を取得
	time += DeltaTime::Get();

	// 移動幅に経過時間をかけ合わせる
	float fScale = sinf(time) * MOVE_HEIGHT;

	// 基準位置に移動量を加算する
	pos.y = base.y + fScale;

	// 親クラスの移動
	CItem::Move(pos);
}

//==========================================
//  エフェクトを生成
//==========================================
void CBowabowa::SetEffect()
{
	// 自身の情報を取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_BOABOA, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot, D3DXVECTOR3(20.0f, 20.0f, 20.0f)));
}

//==========================================
//  リストの取得
//==========================================
CListManager<CBowabowa>* CBowabowa::GetList(void)
{
	return m_pList;
}
