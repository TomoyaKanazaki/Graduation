//========================================
//
// 障害物(石)の処理[obstacleStone.cpp]
// Author：小原立暉
//
//========================================
#include "calculation.h"
#include "obstacleStone.h"
#include "player.h"
#include "sound.h"

#include "model.h"

//========================================
//名前空間
//========================================
namespace
{
	const D3DXVECTOR3 SIZEMAX = D3DXVECTOR3(50.0f, 500.0f, 50.0f);		// サイズの最大値


	const D3DXVECTOR3 SIZEMIN = D3DXVECTOR3(-50.0f, 0.0f, -50.0f);		// サイズの最小値
}

//========================================
//コンストラクタ
//========================================
CObstacleStone::CObstacleStone() : CObstacle()
{//値をクリア
}

//========================================
//デストラクタ
//========================================
CObstacleStone::~CObstacleStone()
{
}

//========================================
// 初期化
//========================================
HRESULT CObstacleStone::Init(const EType type)
{
	if (FAILED(CObstacle::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CObstacleStone::Uninit(void)
{
	// 終了
	CObstacle::Uninit();
}

//========================================
// 更新
//========================================
void CObstacleStone::Update(void)
{
	// 更新
	CObstacle::Update();
}

//========================================
// 描画
//========================================
void CObstacleStone::Draw(void)
{
	// 描画
	CObstacle::Draw();
}

//========================================
// プレイヤーとの当たり判定
//========================================
bool CObstacleStone::Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE state)
{
	D3DXVECTOR3 max = rSize;
	D3DXVECTOR3 min = D3DXVECTOR3(-rSize.x, 0.0f, -rSize.z);

	// モデルが NULL じゃない場合
	if (m_pModel == nullptr) { assert(false); return false; }

	if (UtilFunc::Collision::IsAABBCollidingWithBox
	(
		MyLib::AABB(min, max),
		MyLib::Matrix(rMtx),
		MyLib::AABB(SIZEMIN, SIZEMAX),
		m_pModel->GetMtxWorld())
		)
	{
 		if (state == CPlayer::STATE_ROLL)
		{ // 転がり中の場合、抜ける

			// サウンド情報取得
			CSound* pSound = CManager::GetInstance()->GetSound();

			// サウンド再生
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE_CLASH);

			// 終了処理
			Uninit();

			// false を返す
			return false;
		}
		else
		{ // 上記以外

			// true を返す
			return true;
		}
	}

	// 当たり判定を返す
	return false;
}
