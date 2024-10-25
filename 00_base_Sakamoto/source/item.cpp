//============================================
//
//	アイテムの処理 [item.cpp]
//	Author:morikawa shunya
//
//============================================
#include "item.h"
#include "cross.h"
#include "bowabowa.h"
#include "player.h"
#include "useful.h"
#include "effect.h"
#include "MapSystem.h"

//====================================================================
// コンストラクタ
//====================================================================
CItem::CItem(int nPriority) : CObjectX(nPriority)
{
	m_eState = STATE_NORMAL;	// 状態
	m_eType = TYPE_NONE;		// 種類
	m_nIdxXModel = 0;			// Xモデル番号
	m_nIdxTexture = -1;			// テクスチャ番号
	m_nStateCount = 0;			// 状態管理
	m_fColorA = 0.0f;			// 不透明度
	m_Scaling = 0.0f;			// 大きさ
	m_bCollision = false;		// 当たり判定
	m_CollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 判定座標
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 過去の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_nMapWight = 1;			// マップの横番号
	m_nMapHeight = 0;			// マップの縦番号
	m_bMapScroll = true;
}

//====================================================================
// デストラクタ
//====================================================================
CItem::~CItem()
{

}

//====================================================================
// 生成
//====================================================================
CItem* CItem::Create(TYPE eType, const D3DXVECTOR3& pos)
{
	// インスタンス生成
	CItem* pItem = nullptr;

	if (pItem == nullptr)
	{
		switch (eType)
		{
		case CItem::TYPE_CROSS:		// 十字架
			pItem = new CCross;
			break;

		case CItem::TYPE_BIBLE:		// 聖書
			// pItem = new CBible;
			break;

		case CItem::TYPE_BOWABOWA:	// ぼわぼわ
			pItem = new CBowabowa;
			break;

		default:
			assert(false);
			break;
		}
	}

	// 位置
	pItem->SetPos(pos);

	return pItem;
}

//====================================================================
// 初期化
//====================================================================
HRESULT CItem::Init(char* pModelName)
{
	// 継承クラスの初期化
	CObjectX::Init(pModelName);

	//マップとのマトリックスの掛け合わせをオンにする
	SetMultiMatrix(true);

	return S_OK;
}

//====================================================================
// 終了
//====================================================================
void CItem::Uninit()
{
	// 継承クラスの終了
	CObjectX::Uninit();
}

//====================================================================
// 更新
//====================================================================
void CItem::Update()
{
	if (m_bMapScroll == true)
	{
		m_pos = CMapSystem::GetInstance()->GetGritPos(m_nMapWight, m_nMapHeight);
		m_pos.y = 0.0f;

#ifdef _DEBUG

		CEffect* pEffect = CEffect::Create();
		pEffect->SetPos(m_pos);
		pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
		pEffect->SetRadius(50.0f);

#endif // _DEBUG

		CObjectX::SetPos(m_pos);

		// 継承クラスの更新
		CObjectX::Update();
	}
	else
	{
		Move();
	}
}

//====================================================================
// 描画
//====================================================================
void CItem::Draw()
{
	// 継承クラスの描画
	CObjectX::Draw();
}

//====================================================================
// プレイヤーとの当たり判定
//====================================================================
bool CItem::CollisionPlayer()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();	// 種類を取得

			if(type != TYPE_PLAYER3D)
			{
				pObj = pObjNext;
				continue;
			}

			CPlayer* pPlayer = (CPlayer*)pObj;		// アイテムの情報の取得

			D3DXVECTOR3 pos = pObj->GetPos();
			D3DXVECTOR3 Size = pObj->GetSize();

			// 矩形の当たり判定
			if (useful::CollisionCircle(m_pos, pos, Size.x))
			{
				return true;
			}

			pObj = pObjNext;
		}
	}

	return false;
}
