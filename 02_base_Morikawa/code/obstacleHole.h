//========================================
//
// 障害物(穴)の処理[obstacleHole.h]
// Author：小原立暉
//
//========================================
#ifndef _OBSTACLE_HOLE_H_
#define _OBSTACLE_HOLE_H_

#include "obstacle.h"

//========================================
// 障害物(穴)クラス
//========================================
class CObstacleHole : public CObstacle
{
public:

	CObstacleHole();		// コンストラクタ
	~CObstacleHole();		// デストラクタ

	// メンバ関数
	HRESULT Init(const EType type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:


};

#endif
