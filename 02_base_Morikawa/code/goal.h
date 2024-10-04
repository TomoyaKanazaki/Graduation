//========================================
//
// 障害物の処理[obstacle.h]
// Author：小原立暉
//
//========================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "model.h"

//========================================
// ゴールクラス
//========================================
class CGoal : public CModel
{
public:

	CGoal();		// コンストラクタ
	~CGoal();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CGoal* Create();

private:

};

#endif
