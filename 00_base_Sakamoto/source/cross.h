//============================================
//
//	十字架アイテムの処理 [cross.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CROSS_H_
#define _CROSS_H_

#include "main.h"
#include "item.h"

//前方宣言
class CObjGauge2D;

//オブジェクト十字架クラス
class CCross : public CItem
{
public:

	CCross(int nPriority = 3);
	~CCross();

	//サンプルの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_ACTION,		//行動
		STATE_MAX,
	};

	static CCross* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	bool CollisionPlayer() override;


private:
	void StateManager(void);	//状態管理

};
#endif