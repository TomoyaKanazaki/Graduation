//========================================
//
// 障害物(石)の処理[obstacleStone.h]
// Author：小原立暉
//
//========================================
#ifndef _OBSTACLE_STONE_H_
#define _OBSTACLE_STONE_H_

#include "obstacle.h"

//========================================
// 障害物(石)クラス
//========================================
class CObstacleStone : public CObstacle
{
public:

	CObstacleStone();		// コンストラクタ
	~CObstacleStone();		// デストラクタ

	// メンバ関数
	HRESULT Init(const EType type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE state) override;	// プレイヤーとの当たり判定

private:


};

#endif
