//============================================
//
//	壁モデルの処理 [wall.cpp]
//	Author:Satone Shion
//
//============================================
#include "wall.h"
#include "renderer.h"
#include "manager.h"
#include "objmeshField.h"
#include "game.h"
#include "devil.h"
#include "MapMove.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CWall>* CWall::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CWall::CWall(int nPriority) : CObjectX(nPriority)
{
	SetSize(INITVECTOR3);
	SetPos(INITVECTOR3);
	m_Grid.x = 0;
	m_Grid.z = 0;
}

//====================================================================
//コンストラクタ(オーバーロード)
//====================================================================
CWall::CWall(int nPriority, CMapSystem::GRID gridCenter) : CObjectX(nPriority)
{
	SetSize(INITVECTOR3);
	SetPos(INITVECTOR3);
	m_Grid = gridCenter;	// グリッドの位置
}

//====================================================================
//デストラクタ
//====================================================================
CWall::~CWall()
{

}

//====================================================================
//生成処理
//====================================================================
CWall* CWall::Create(CMapSystem::GRID gridCenter)
{
	CWall* pSample = nullptr;

	if (pSample == nullptr)
	{
		// 床の生成
		pSample = new CWall(3, gridCenter);
	}

	// 床の初期化処理
	if (FAILED(pSample->Init("data\\MODEL\\wall.x")))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pSample;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CWall::Init(char* pModelName)
{
	SetType(CObject::TYPE_TILE);

	CObjectX::Init(pModelName);

	//マップとのマトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// 位置設定
	CObjectX::SetPos(m_Grid.ToWorld());

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CWall>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CWall::Uninit(void)
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

	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CWall::Update(void)
{
	// グリッドに合わせて移動する
	D3DXVECTOR3 pos = GetPos();
	pos = m_Grid.ToWorld();
	SetPos(pos);

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//描画処理
//====================================================================
void CWall::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	
	// 継承クラスの描画
	CObjectX::Draw();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 <= (参照値 <= ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// 2回目の描画
	CObjectX::Draw();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 3);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 <= (参照値 <= ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// 3回目の描画
	CObjectX::Draw();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 101);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 <= (参照値 <= ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// 4回目の描画
	CObjectX::Draw();

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CWall>* CWall::GetList(void)
{
	return m_pList;
}
