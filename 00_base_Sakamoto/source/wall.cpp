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
	m_Grid.x = gridCenter.x;	// グリッドの位置X
	m_Grid.z = gridCenter.z;	// グリッドの位置Z
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
	CMapSystem* pMapSystem = CMapSystem::GetInstance();		// マップシステムの情報
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

	SetType(CObject::TYPE_TILE);

	CObjectX::Init(pModelName);

	//マップとのマトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CGame::GetInstance()->GetMapField()->GetMatrix());

	D3DXVECTOR3 size = GetSize() * 2.0f;		// モデルのサイズ取得

	// 位置設定
	CObjectX::SetPos(D3DXVECTOR3(MapSystemPos.x + m_Grid.x * size.x, 0.0f, MapSystemPos.z - m_Grid.z * size.z));

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
void CWall::Update(void)
{
	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//描画処理
//====================================================================
void CWall::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CWall>* CWall::GetList(void)
{
	return m_pList;
}
