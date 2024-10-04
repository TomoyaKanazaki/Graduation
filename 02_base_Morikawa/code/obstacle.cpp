//========================================
//
// 障害物の処理[obstacle.cpp]
// Author：小原立暉
//
//========================================
#include "obstacle.h"
#include "model.h"
#include "calculation.h"

#include "obstacleTree.h"
#include "obstacleRiver.h"
#include "obstacleStone.h"
#include "obstacleHole.h"
#include "obstacleSoftRock.h"

//========================================
//名前空間
//========================================
namespace
{
	const char* MODEL[] =		// モデルのパス
	{
		"data\\MODEL\\obstacle\\wood00.x",		// 川
		"data\\MODEL\\obstacle\\stone.x",		// 石
		"data\\MODEL\\obstacle\\hole.x",		// 穴
		"data\\MODEL\\obstacle\\wood00.x",		// 柔らかい岩
	};

	const D3DXVECTOR3 SIZEMAX[] =	// サイズの最大値
	{
		D3DXVECTOR3(60.0f, 0.0f, 60.0f),		// 川
		D3DXVECTOR3(60.0f, 500.0f, 60.0f),		// 石
		D3DXVECTOR3(40.0f, 0.0f, 40.0f),		// 穴
		D3DXVECTOR3(60.0f, 0.0f, 60.0f),		// 柔らかい岩
	};

	const D3DXVECTOR3 SIZEMIN[] =	// サイズの最小値
	{
		D3DXVECTOR3(-60.0f, 0.0f, -60.0f),		// 川
		D3DXVECTOR3(-60.0f, 0.0f, -60.0f),		// 石
		D3DXVECTOR3(-40.0f, 0.0f, -40.0f),		// 穴
		D3DXVECTOR3(-60.0f, 0.0f, -60.0f),		// 柔らかい岩
	};
}

//========================================
// 静的メンバ変数宣言
//========================================
CListManager<CObstacle> CObstacle::m_List = {};	// リスト

//========================================
//コンストラクタ
//========================================
CObstacle::CObstacle() : CStageObj(),
m_type(TYPE_RIVER)
{//値をクリア
}

//========================================
//デストラクタ
//========================================
CObstacle::~CObstacle()
{
}

//========================================
// 初期化
//========================================
HRESULT CObstacle::Init(void)
{
	// リストに追加
	m_List.Regist(this);

	// モデル生成
	m_pModel = CModel::Create(MODEL[m_type]);

	if (FAILED(CStageObj::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CObstacle::Uninit(void)
{
	// リストから削除
	m_List.Delete(this);

	// 終了
	CStageObj::Uninit();
}


//==========================================================================
// 削除
//==========================================================================
void CObstacle::Kill()
{
	// リストから削除
	m_List.Delete(this);

	// 終了
	CStageObj::Kill();
}

//========================================
// 更新
//========================================
void CObstacle::Update(void)
{
	// 更新
	CStageObj::Update();
}

//========================================
// プレイヤーとの当たり判定
//========================================
bool CObstacle::Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE /*state*/)
{
	D3DXVECTOR3 max = rSize;
	D3DXVECTOR3 min = D3DXVECTOR3(-rSize.x, 0.0f, -rSize.z);

	// モデルが NULL じゃない場合
	if (m_pModel == nullptr) { assert(false); return false; }

	// 当たり判定を返す
	return UtilFunc::Collision::IsAABBCollidingWithBox(MyLib::AABB(min, max), MyLib::Matrix(rMtx), MyLib::AABB(SIZEMIN[m_type], SIZEMAX[m_type]), m_pModel->GetMtxWorld());
}

//========================================
// 描画
//========================================
void CObstacle::Draw(void)
{
	// 描画
	CStageObj::Draw();
}

//========================================
// 生成処理
//========================================
CObstacle* CObstacle::Create(const EType type, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	CObstacle* pObstacle = nullptr;

	switch (type)
	{
	case CObstacle::TYPE_RIVER:

		pObstacle = new CObstacleRiver;

		break;

	case CObstacle::TYPE_STONE:

		pObstacle = new CObstacleStone;

		break;

	case CObstacle::TYPE_HOLE:

		pObstacle = new CObstacleHole;

		break;

	case CObstacle::TYPE_SOFTROCK:

		pObstacle = new CObstacleSoftRock;

		break;

	default:
		assert(false);
		break;
	}

	// 位置・向き・種類設定
	pObstacle->m_type = type;
	pObstacle->SetPos(rPos);
	pObstacle->SetRot(rRot);

	// ポインタを返す
	return pObstacle;
}
