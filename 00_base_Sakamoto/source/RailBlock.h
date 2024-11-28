//============================================
//
//	レールブロックの処理 [RailBlock.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RAILBLOCK_H_
#define _RAILBLOCK_H_

#include "main.h"
#include "ObjectX.h"
#include "MapSystem.h"

class CRail;

//オブジェクトメッシュフィールドクラス
class CRailBlock : public CObjectX
{
public:
	CRailBlock(int nPriority = 3);
	CRailBlock(int nPriority, CMapSystem::GRID gridCenter);
	~CRailBlock();

	static CRailBlock* Create(CMapSystem::GRID gridCenter);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CMapSystem::GRID GetGrid(void) { return m_Grid; }	// グリッド取得

	/*static CRailBlock* Create(int nMapWight, int nMapHeight, bool Edit, int Max, int *nMove);

	HRESULT Init(int nMapWight, int nMapHeight, bool Edit, int Max = 0, int* nMove = 0);
	void Uninit(void);
	void Update(void);
	void Draw(void);*/

	void EditRailSet(int Number);
	void EditRailUpdate(void);
	void RailDelete();
	int GetRailMax() { return m_nMax; }
	int GetRailMove(int nCnt);

	// 静的メンバ関数
	static CListManager<CRailBlock>* GetList(void); // リスト取得

private:

	void Move(D3DXVECTOR3 *Pos);
	void RailCheck();
	void RailAddWrite();
	void RailSet(int Max, int* nMove);

	//CMapSystem::GRID m_StartGrid;	//開始時のグリット番号

	CMapSystem::GRID m_Grid;	//グリット番号

	bool bMoveOK[4];
	int m_nMax;
	int m_nMove[64];
	CRail* m_pTop;		// 先頭のレールへのポインタ
	CRail* m_pCur;		// 最後尾のレールへのポインタ

	// 静的メンバ変数
	static CListManager<CRailBlock>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CRailBlock>::AIterator m_iterator; // イテレーター

};
#endif