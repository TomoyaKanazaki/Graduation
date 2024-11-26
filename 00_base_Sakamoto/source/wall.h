//============================================
//
//	壁モデルの処理 [wall.h]
//	Author:Satone Shion
//
//============================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
#include "objectX.h"
#include "MapSystem.h"

//壁クラス
class CWall : public CObjectX
{
public:

	CWall(int nPriority = 3);
	CWall(int nPriority, CMapSystem::GRID gridCenter);
	~CWall();

	static CWall* Create(CMapSystem::GRID gridCenter);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CMapSystem::GRID GetGrid(void) { return m_Grid; }	// グリッド取得

	// 静的メンバ関数
	static CListManager<CWall>* GetList(void); // リスト取得

private:

	CMapSystem::GRID m_Grid;	//グリット番号

	// 静的メンバ変数
	static CListManager<CWall>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CWall>::AIterator m_iterator; // イテレーター

};
#endif