//============================================
//
//	影処理 [shadow.cpp]
//	Author:morikawa shunya
//
//============================================

#include "shadow.h"
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
	const float LIMIT_HEIGHT = 2000.0f; // 影を描画する上限の高さ
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CShadow>* CShadow::m_pList = nullptr; // オブジェクトリスト

//===========================================
// コンストラクタ
//===========================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority),
m_fHeight(0.0f),
m_sizeBase(INITVECTOR3)
{
}

//===========================================
// デストラクタ
//===========================================
CShadow::~CShadow()
{
}

//===========================================
// 生成
//===========================================
CShadow* CShadow::Create(const D3DXVECTOR3& pos, float fWidth, float fHeight)
{
	// インスタンス生成
	CShadow* pShadow = new CShadow;

	// 初期化
	pShadow->Init();

	// 位置設定
	pShadow->SetPos(pos);

	// 大きさ
	pShadow->SetpVtx(fWidth, fHeight);
	pShadow->m_sizeBase.x = fWidth;
	pShadow->m_sizeBase.z = fHeight;

	return pShadow;
}

//===========================================
// 初期化
//===========================================
HRESULT CShadow::Init()
{
	// 継承クラスの初期化
	CObject3D::Init();

	// マップとのマトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CGame::GetInstance()->GetMapField()->GetMatrix());

	// テクスチャ設定
	SetTexture("data\\TEXTURE\\player\\shadow000.jpg");

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CShadow>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//===========================================
// 終了
//===========================================
void CShadow::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 継承クラスの終了
	CObject3D::Uninit();
}

//===========================================
// 更新
//===========================================
void CShadow::Update(void)
{
	// サイズを変更する
	float fScale = (LIMIT_HEIGHT - m_fHeight) / LIMIT_HEIGHT;
	D3DXVECTOR3 size = m_sizeBase * fScale;
	SetpVtx(size.x, size.z);

	// 透明度を変更する
	SetColorA(1.0f - fScale);
}

//===========================================
// 描画
//===========================================
void CShadow::Draw(void)
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

//==========================================
//  リストの取得
//==========================================
CListManager<CShadow>* CShadow::GetList(void)
{
	return m_pList;
}
