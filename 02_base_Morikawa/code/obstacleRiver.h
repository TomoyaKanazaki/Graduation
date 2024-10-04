//========================================
//
// 障害物(川)の処理[obstacleRiver.h]
// Author：小原立暉
//
//========================================
#ifndef _OBSTACLE_RIVER_H_
#define _OBSTACLE_RIVER_H_

#include "obstacle.h"

//========================================
// 障害物(川)クラス
//========================================
class CObstacleRiver : public CObstacle
{
public:

	CObstacleRiver();		// コンストラクタ
	~CObstacleRiver();		// デストラクタ

	// メンバ関数
	HRESULT Init(const EType type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:


};

#endif
