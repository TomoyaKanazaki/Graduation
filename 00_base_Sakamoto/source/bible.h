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

	//enum POS_TYPE
	//{
	//	POS_TYPE_LEFTUP = 0,	//左上
	//	POS_TYPE_RIGHTUP,		//右上
	//	POS_TYPE_LEFTDOWN,		//左下
	//	POS_TYPE_RIGHTDOWN,		//右下
	//	POS_TYPE_MAX,			//列挙の最大数
	//};

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void Move();

	void SetGrid(const GRID& pos) override;
	bool Hit(CPlayer* pPlayer) override;

	// 静的メンバ関数
	static CListManager<CBible>* GetList(void); // リスト取得

private:
	void StateManager(void);		//状態管理

	// 静的メンバ変数
	static CListManager<CBible>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CBible>::AIterator m_iterator; // イテレーター

	float m_fMove;

	// メンバ変数
	D3DXVECTOR3 m_posBase; // 移動の中心座標
	float m_fMoveTime; // 移動時間
};

#endif
