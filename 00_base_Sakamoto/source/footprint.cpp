//============================================
//
//	影処理 [shadow.cpp]
//	Author:morikawa shunya
//
//============================================
#include "footprint.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "objmeshField.h"
#include "Scene.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 DEFAULT_SIZE = { 80.0f, 0.0f, 80.0f };
	const char* TEXTURE_PASS = "data\\TEXTURE\\foot_print.png";
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CFootPrint>* CFootPrint::m_pList = nullptr; // オブジェクトリスト

//===========================================
// コンストラクタ
//===========================================
CFootPrint::CFootPrint(int nPriority) : CObject3D(nPriority)
{
}

//===========================================
// デストラクタ
//===========================================
CFootPrint::~CFootPrint()
{
}

//===========================================
// 初期化
//===========================================
HRESULT CFootPrint::Init()
{
	// 継承クラスの初期化
	if (FAILED(CObject3D::Init()))
	{
		assert(false);
		return E_FAIL;
	}

	// 不透明度を1で設定する
	SetAlpha(1.0f);

	//マップとのマトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// テクスチャ設定
	SetTexture(TEXTURE_PASS);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CFootPrint>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//===========================================
// 終了
//===========================================
void CFootPrint::Uninit(void)
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
	CObject3D::Uninit();
}

//===========================================
// 更新
//===========================================
void CFootPrint::Update(void)
{
	// 不透明度を取得
	float fAlpha = GetAlpha();

	// 完全に透過されていた場合関数を抜ける
	if (fAlpha <= 0.0f)
	{
		Uninit();
		return;
	}

	// 不透明度を下げる
	SetAlpha(fAlpha - DeltaTime::Get());

	// 親クラスの更新処理
	CObject3D::Update();
}

//===========================================
// 描画
//===========================================
void CFootPrint::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 継承クラスの描画
	CObject3D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===========================================
// 生成
//===========================================
CFootPrint* CFootPrint::Create(CMapSystem::GRID& grid, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CFootPrint* pFootPrint = new CFootPrint;

	// 初期化
	if (FAILED(pFootPrint->Init()))
	{
		assert(false);
		return nullptr;
	}

	// 位置設定
	pFootPrint->SetPos(grid.ToWorld());

	// 向きの設定
	pFootPrint->SetRot(rot);

	// サイズの設定
	pFootPrint->SetSize(DEFAULT_SIZE);

	return pFootPrint;
}

//==========================================
//  リストの取得
//==========================================
CListManager<CFootPrint>* CFootPrint::GetList(void)
{
	return m_pList;
}

//==========================================
//  座標の設定
//==========================================
void CFootPrint::SetPos(const D3DXVECTOR3& pos)
{
	// 変数を一時保存
	D3DXVECTOR3 temp = pos;

	// y座標を少し上げる
	temp.y += 15.0f;

	// 座標を設定
	CObject3D::SetPos(temp);
}
