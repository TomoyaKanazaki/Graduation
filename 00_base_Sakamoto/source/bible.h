//============================================
//
//	聖書の処理 [bible.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _BIBLE_H_
#define _BIBLE_H_

#include "main.h"
#include "item.h"

//前方宣言
class CObjGauge2D;

//オブジェクト聖書クラス
class CBible : public CItem
{
public:

	CBible(int nPriority = 3);
	~CBible();

	static CBible* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	bool CollisionPlayer() override;

	// 静的メンバ関数
	static CListManager<CBible>* GetList(void); // リスト取得

private:
	void StateManager(void);		//状態管理

		// 静的メンバ変数
	static CListManager<CBible>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CBible>::AIterator m_iterator; // イテレーター

};

#endif
