//========================================
//
// 障害物(木)の処理[obstacleTree.h]
// Author：小原立暉
//
//========================================
#ifndef _OBSTACLE_TREE_H_
#define _OBSTACLE_TREE_H_

#include "obstacle.h"

//========================================
// 障害物(木)クラス
//========================================
class CObstacleTree : public CObstacle
{
public:

	CObstacleTree();		// コンストラクタ
	~CObstacleTree();		// デストラクタ

	// メンバ関数
	HRESULT Init(const EType type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:


};

#endif
