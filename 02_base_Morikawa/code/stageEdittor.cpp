//=============================================================================
// 
//  ステージエディター処理 [stageEdittor.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "stageEdittor.h"
#include "stageobj.h"
#include "obstacle.h"

#include "manager.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\box.x";
	const float ENABLERANGE = 1000.0f;	// 有効範囲
}

//==========================================================================
// コンストラクタ
//==========================================================================
CStageEdittor::CStageEdittor() : CObject(),
m_pStageObj(nullptr),
m_nType(0)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CStageEdittor::~CStageEdittor()
{

}

//==========================================================================
// 反転ポイントの生成処理
//==========================================================================
CStageEdittor* CStageEdittor::Create()
{
	// メモリの確保
	CStageEdittor* pObj = new CStageEdittor;

	pObj->Init();

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CStageEdittor::Init()
{
	m_pStageObj = CStageObj::CreateBg(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CStageEdittor::Uninit()
{
	if (m_pStageObj == nullptr)
	{
		m_pStageObj->Uninit();
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CStageEdittor::Update()
{
	// CInputKeyboard型のポインタ
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_9))
	{
		// 種類変え
		SetType();
	}

	// 操作
	Control();

	DebugProc::Print(DebugProc::EPoint::POINT_CENTER, "種類：%d\n位置：%f %f %f\n", m_nType, m_pStageObj->GetPos().x, m_pStageObj->GetPos().y, m_pStageObj->GetPos().z);
}

//==========================================================================
// 描画処理
//==========================================================================
void CStageEdittor::Draw()
{
	if (m_pStageObj != nullptr)
	{// モデルの描画
		m_pStageObj->Draw();
	}
}

//==========================================================================
// 種類設定
//==========================================================================
void CStageEdittor::SetType()
{
	D3DXVECTOR3 pos = m_pStageObj->GetPos();

	// 種類を変える
	m_pStageObj->Uninit();
	m_nType = (m_nType + 1) % CStageObj::TYPE_MAX;

	switch (m_nType)
	{
	case CStageObj::TYPE_BG:

		m_pStageObj = CStageObj::CreateBg(pos);

		break;

	case CStageObj::TYPE_OBSTACLE:

		m_pStageObj = CStageObj::CreateObstacle(pos, CObstacle::TYPE_RIVER);

		break;

	case CStageObj::TYPE_REVERSE:

		m_pStageObj = CStageObj::CreateReverse(pos);

		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// 種類設定
//==========================================================================
void CStageEdittor::Control()
{
	// CInputKeyboard型のポインタ
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	D3DXVECTOR3 pos = m_pStageObj->GetPos();
	D3DXVECTOR3 posOrigin = m_pStageObj->GetOriginPosition();

	if (pInputKeyboard->GetPress(DIK_RIGHT))
	{
		pos.x += 3.0f;
		posOrigin.x += 3.0f;
	}
	if (pInputKeyboard->GetPress(DIK_LEFT))
	{
		pos.x -= 3.0f;
		posOrigin.x -= 3.0f;
	}
	if (pInputKeyboard->GetPress(DIK_UP))
	{
		pos.y += 3.0f;
		posOrigin.y += 3.0f;
	}
	if (pInputKeyboard->GetPress(DIK_DOWN))
	{
		pos.y -= 3.0f;
		posOrigin.y -= 3.0f;
	}

	m_pStageObj->SetPos(pos);
	m_pStageObj->SetOriginPosition(posOrigin);
}