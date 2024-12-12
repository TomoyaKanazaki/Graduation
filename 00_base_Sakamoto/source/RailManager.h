//============================================
//
//	レールマネージャー [RailManager.h]
//	Author:Satone Shion
//
//============================================
#ifndef _RAILMANAGER_H_
#define _RAILMANAGER_H_

#include "main.h"
#include "objectX.h"

//前方宣言
class CObjGauge2D;

//オブジェクトプレイヤークラス
class CRailManager
{
public:

	CRailManager();
	~CRailManager();

	static CRailManager* Create();

	HRESULT Init(void);
	void Uninit(void);
	void SetNULL(void);
	void Update(void);
	void Draw(void);

	void SetWightNumber(int Number) { m_nMapWidth = Number; }
	int GetWightNumber(void) { return m_nMapWidth; }
	void SetHeightNumber(int Number) { m_nMapHeight = Number; }
	int GetHeightNumber(void) { return m_nMapHeight; }

	void SetPrevRailManager(CRailManager *Set) { m_pPrev = Set; }
	CRailManager* GetPrevRailManager(void) { return m_pPrev; }
	void SetNextRailManager(CRailManager* Set) { m_pNext = Set; }
	CRailManager* GetNextRailManager(void) { return m_pNext; }

	int GetNextNumber(void) { return m_bNextNumber; }

	// 静的メンバ関数
	static CListManager<CRailManager>* GetList(void); // リスト取得

private:

	CObjectX* m_pRailManagerModel[2];
	int m_bNextNumber;

	int m_nMapWidth;	// マップの横番号
	int m_nMapHeight;	// マップの縦番号
								   
	CRailManager* m_pPrev;		// 前のレールへのポインタ
	CRailManager* m_pNext;		// 次のレールへのポインタ

	// 静的メンバ変数
	static CListManager<CRailManager>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CRailManager>::AIterator m_iterator; // イテレーター

};
#endif