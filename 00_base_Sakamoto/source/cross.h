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

	// 静的メンバ関数
	static CListManager<CCross>* GetList(void); // リスト取得

private:
	void StateManager(void);	//状態管理

	int m_nDeletCont;		// 削除カウンター

	// 静的メンバ変数
	static CListManager<CCross>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CCross>::AIterator m_iterator; // イテレーター

};
#endif