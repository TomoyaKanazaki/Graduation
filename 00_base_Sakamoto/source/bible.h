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

	enum POS_TYPE
	{
		POS_TYPE_LEFTUP = 0,	//左上
		POS_TYPE_RIGHTUP,		//右上
		POS_TYPE_LEFTDOWN,		//左下
		POS_TYPE_RIGHTDOWN,		//右下
		POS_TYPE_MAX,			//列挙の最大数
	};

	static CBible* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	bool CollisionPlayer() override;
	void Move() override;

	// 静的メンバ関数
	static CListManager<CBible>* GetList(void); // リスト取得


	//位置設定・取得
	void SetPosType(const POS_TYPE& TYPE) { m_PosType = TYPE; }
	POS_TYPE GetPosType(void) { return m_PosType; }

private:
	void StateManager(void);		//状態管理

		// 静的メンバ変数
	static CListManager<CBible>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CBible>::AIterator m_iterator; // イテレーター

	POS_TYPE m_PosType;
	float m_fMove;
};

#endif
