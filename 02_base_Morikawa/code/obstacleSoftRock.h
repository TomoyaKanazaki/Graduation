//========================================
//
// 障害物(柔らかい岩)の処理[obstacleSoftRock.h]
// Author：小原立暉
//
//========================================
#ifndef _OBSTACLE_SOFTROCK_H_
#define _OBSTACLE_SOFTROCK_H_

#include "obstacle.h"

//========================================
// 障害物(柔らかい岩)クラス
//========================================
class CObstacleSoftRock : public CObstacle
{
public:

	CObstacleSoftRock();		// コンストラクタ
	~CObstacleSoftRock();		// デストラクタ

	// メンバ関数
	HRESULT Init(const EType type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:


};

#endif
