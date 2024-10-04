//========================================
//
// ウォール[wall.cpp]
// Author：森川駿弥
//
//========================================
#include "wall.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================
// 静的メンバ変数
//========================================
LPDIRECT3DTEXTURE9 CWall::m_pTexture = nullptr;

//========================================
// コンストラクタ
//========================================
CWall::CWall(int nPriority) :
	CObject3D(nPriority),
	m_aTexU(0.0f)
{
}

//========================================
// デストラクタ
//========================================
CWall::~CWall()
{
}

//========================================
// 壁生成
//========================================
CWall* CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// CWall型のポインタ
	CWall* pWall = new CWall;

	//初期化
	pWall->Init();

	pWall->SetPos(pos);
	pWall->SetRot(rot);

	//ポインタを返す
	return pWall;
}

//========================================
// 初期化
//========================================
HRESULT CWall::Init(void)
{
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 継承の初期化
	CObject3D::Init();

	m_aTexU = 0.0f;

	//テクスチャ割り当て
	BindTexture(pTexture->Regist("data\\texture\\sky_02.jpg"));

	return S_OK;
}

//========================================
// 終了
//========================================
void CWall::Uninit(void)
{
	// 継承の初期化
	CObject3D::Uninit();
}

//========================================
// 更新
//========================================
void CWall::Update(void)
{
	CObject3D::Update();

	//テクスチャ座標の更新(U値)
	m_aTexU += 0.0001f;
}

//========================================
// 描画
//========================================
void CWall::Draw(void)
{
	//描画
	CObject3D::Draw();
}

//========================================
// 頂点情報設定
//========================================
void CWall::SetVertex3D()
{
	CObject3D::SetVertex3D();

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 Vertex = CObject3D::GetVtxBuff();

	//頂点情報へのポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	Vertex->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(m_aTexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_aTexU + 0.3f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_aTexU, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_aTexU + 0.3f, 1.0f);

	//頂点バッファをアンロック
	Vertex->Unlock();
}
