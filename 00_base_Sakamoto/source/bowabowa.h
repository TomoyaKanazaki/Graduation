//============================================
//
//	ボワボワの処理 [SampleObjX.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _BOWABOWA_H_
#define _BOWABOWA_H_

#include "main.h"
#include "item.h"

//前方宣言
class CObjGauge2D;

//オブジェクトボワボワクラス
class CBowabowa : public CItem
{
public:

	CBowabowa(int nPriority = 3);
	~CBowabowa();

	//サンプルの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_ACTION,		//行動
		STATE_MAX,
	};

	// 静的メンバ関数
	static CListManager<CBowabowa>* GetList(void); // リスト取得

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);
	void Take(void);

	bool Hit(CPlayer* pPlayer) override;

private:
	void StateManager(void);		//状態管理

	// 静的メンバ変数
	static CListManager<CBowabowa>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CBowabowa>::AIterator m_iterator; // イテレーター

};
#endif