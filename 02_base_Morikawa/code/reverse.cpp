//========================================
//
// 障害物の処理[reverse.cpp]
// Author：相馬靜雅
//
//========================================
#include "reverse.h"
#include "model.h"
#include "calculation.h"
#include "billboard.h"
#include "manager.h"
#include "texture.h"
#include "reverse_direction.h"
#include "player.h"

//========================================
//名前空間
//========================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\reverse.png";	// モデルのパス
	const float ITEMSIZE = 150.0f;	// サイズ
	const float COLLISIONRANGE = 200.0f;	// 有効範囲
	const float ROTATION_RANGE = D3DX_PI * 0.2f;
}
CListManager<CReverse> CReverse::m_List = {};	// リスト

//========================================
//コンストラクタ
//========================================
CReverse::CReverse(int nPriority) : CStageObj(nPriority),
m_fRotationTime(0.0f),	// 回転時間
m_pBillboard(nullptr)
{

}

//========================================
//デストラクタ
//========================================
CReverse::~CReverse()
{

}

//========================================
// 初期化
//========================================
HRESULT CReverse::Init(void)
{
	// リストに追加
	m_List.Regist(this);

	// ビルボード生成
	m_pBillboard = CBillboard::Create(GetPos(), ITEMSIZE, ITEMSIZE);
	m_pBillboard->SetEnableFront(false);

	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	int texIdx = pTexture->Regist(TEXTURE);
	m_pBillboard->BindTexture(texIdx);

	if (FAILED(CStageObj::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CReverse::Uninit(void)
{
	// リストから削除
	m_List.Delete(this);

	// 終了
	CStageObj::Uninit();
}

//========================================
// 削除
//========================================
void CReverse::Kill()
{
	if (m_pBillboard != nullptr)
	{
		m_pBillboard->Uninit();
		m_pBillboard = nullptr;
	}

	// 終了
	CReverse::Uninit();
}

//==========================================================================
// 位置
//==========================================================================
void CReverse::SetPos(D3DXVECTOR3 pos)
{
	CObject::SetPos(pos);

	if (m_pBillboard != nullptr)
	{
		m_pBillboard->SetPos(pos);
	}
}

//========================================
// 更新
//========================================
void CReverse::Update(void)
{
	// 更新
	CStageObj::Update();

	if (m_pBillboard != nullptr)
	{
		m_pBillboard->SetPos(GetPos() + MyLib::Vector3(0.0f, ITEMSIZE, 0.0f));

		// くるくる
		MyLib::Vector3 rot = m_pBillboard->GetRot();
		m_fRotationTime += CManager::GetInstance()->GetDeltaTime();
		rot.y = cosf(m_fRotationTime / 1.0f) * ROTATION_RANGE;
		m_pBillboard->SetRot(rot);

		m_pBillboard->Update();
	}
}

//========================================
// 描画
//========================================
void CReverse::Draw(void)
{
	if (m_pBillboard != nullptr)
	{
		m_pBillboard->Draw();
	}

	// 描画
	CStageObj::Draw();
}

//========================================
// プレイヤーとの当たり判定
//========================================
bool CReverse::Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE /*state*/)
{
	bool bHit = false;

	// 位置取得
	MyLib::Vector3 pos = GetPos();
	MyLib::Vector3 posTarget = UtilFunc::Transformation::WorldMtxChangeToPosition(rMtx);

	if (UtilFunc::Collision::CircleRange3D(pos, posTarget, COLLISIONRANGE, rSize.x))
	{
		Kill();
		CReverse_Direction::Create();
		bHit = true;
	}

	return bHit;

}
