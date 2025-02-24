//============================================
//
//	レールの処理 [Rail.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _Rail_H_
#define _Rail_H_

#include "main.h"
#include "object.h"
#include "MapSystem.h"

//前方宣言
class CObjectX;		// オブジェクトX

//レールクラス
class CRail : public CObject
{
public:

	CRail(int nPriority = 3);
	~CRail();

	// レール位置の種類
	enum POSTYPE
	{
		POSTYPE_DOWN = 0,	// 下
		POSTYPE_RIGHT,		// 右
		POSTYPE_UP,			// 上
		POSTYPE_LEFT,		// 左
		POSTYPE_MAX,
		POSTYPE_NONE		// どこにも無し
	};

	// レールの位置状態列挙
	enum POSSTATE
	{
		POSSTATE_FIRST = 0,		// 1番目
		POSSTATE_SECOND,		// 2番目
		POSSTATE_MAX
	};

	static CRail* Create(CMapSystem::GRID grid, POSTYPE PosType0, POSTYPE PosType1);

	HRESULT Init(void);
	void Uninit(void);
	void SetNULL(void);
	void Update(void);
	void Draw(void);

	// レールの場所
	void PrevSet(POSTYPE Set);		// 前
	void NextSet(POSTYPE Set);		// 次

	void SetWightNumber(int Number) { m_nMapWidth = Number; }
	int GetWightNumber(void) { return m_nMapWidth; }
	void SetHeightNumber(int Number) { m_nMapHeight = Number; }
	int GetHeightNumber(void) { return m_nMapHeight; }

	void SetRailOK(int nCnt, bool Set) { m_bRail[nCnt] = Set; }
	bool GetRailOK(int nCnt) { return m_bRail[nCnt]; }
	int GetNextNumber(void) { return m_bNextNumber; }		// 次のモデルの番号

	// 静的メンバ関数
	static CListManager<CRail>* GetList(void); // リスト取得

private:

	CObjectX* m_pRailModel[POSSTATE_MAX];		// レールモデルの情報
	bool m_bRail[POSTYPE_MAX];					// レールの置ける位置の判定
	int m_bNextNumber;				// 次のレール番号

	int m_nMapWidth;	// マップの横番号
	int m_nMapHeight;	// マップの縦番号
								  

	CMapSystem::GRID m_Grid;			// グリッド情報
	POSTYPE m_PosType[POSSTATE_MAX];	// レール位置の種類

	// 静的メンバ変数
	static CListManager<CRail>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CRail>::AIterator m_iterator; // イテレーター

};
#endif