//============================================
//
//	マウスUI [mouseUI.cpp]
//	Author : Takeru Ogasawara
//
//============================================

#include "cursorUI.h"
#include "manager.h"
#include "input.h"
#include "useful.h"
#include "debugproc.h"

// 定数定義
namespace
{
	const char* TEX_PATH = "data\\TEXTURE\\mouseCursor2.png";
	const D3DXVECTOR2 SIZE_UI = { 50.0f, 50.0f };	// 大きさ
	const D3DXVECTOR2 COLL_POS = { SIZE_UI.x * 0.5f, SIZE_UI.y * 0.5f };	// 判定の位置(左上判定)
	const float SPEED = 5.5f;	// カーソル速度
}

//====================================================================
// コンストラクタ
//====================================================================
CCursorUI::CCursorUI(int nPriority) : CObject2D(nPriority)
{
	m_move = { 0.0f, 0.0f, 0.0f }; 
}

//====================================================================
// デストラクタ
//====================================================================
CCursorUI::~CCursorUI()
{

}

//====================================================================
// 生成処理
//====================================================================
CCursorUI* CCursorUI::Create(D3DXVECTOR2 pos)
{
	CCursorUI* pMouseUI = nullptr;

	if (!pMouseUI)
	{
		pMouseUI = new CCursorUI;
	}

	// オブジェクトの初期化処理
	if (FAILED(pMouseUI->Init()))
	{// 初期化処理が失敗した場合
		return nullptr;
	}

	pMouseUI->SetPos({ pos.x, pos.y, 0.0f });

	return pMouseUI;
}

//====================================================================
// 初期化処理
//====================================================================
HRESULT CCursorUI::Init(void)
{
	// 親クラスの初期化処理
	CObject2D::Init();

	SetTexture(TEX_PATH);
	SetWidth(SIZE_UI.x);
	SetHeight(SIZE_UI.y);

	return S_OK;
}

//====================================================================
// 終了処理
//====================================================================
void CCursorUI::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();
}

//====================================================================
// 更新処理
//====================================================================
void CCursorUI::Update(void)
{
	// 移動
	move();

	// 親クラスの更新処理
	CObject2D::Update();
}

//====================================================================
// 描画処理
//====================================================================
void CCursorUI::Draw(void)
{
	// 親クラスの描画処理
	CObject2D::Draw();
}

//====================================================================
// 移動
//====================================================================
void CCursorUI::move()
{
	// 情報の取得
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (!pMouse)
	{
		return;
	}

	// 情報の取得
	D3DXVECTOR3 pos = GetPos();

	// 移動量の加算
	m_move = pMouse->GetMouseMove();

	// 位置に加算
	pos += m_move * SPEED;

	// ウィンドウ画面の当たり判定
	useful::CollisionWindow(&pos);

	// 位置の設定
	SetPos(pos);

	CManager::GetInstance()->GetDebugProc()->Print("\nカーソルUI位置 %f %f\n", pos.x, pos.y);
}

//====================================================================
// 当たり判定 : 位置、大きさ [どちらも判定する2Dを入れる]
//====================================================================
bool CCursorUI::Collsion(D3DXVECTOR3 pos2D, D3DXVECTOR3 size2D)
{
	// 情報の取得
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (!pMouse)
	{
		return false;
	}

	// 情報の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posCollision = {};

	// 判定位置
	posCollision = { pos.x - COLL_POS.x, pos.y - COLL_POS.y, 0.0f };

	if (useful::CollisionRectangle2D(posCollision, pos2D, D3DXVECTOR3(0.0f, 0.0f, 0.0f), size2D, useful::COLLISION_XY))
	{
		return true;
	}

	return false;
}
